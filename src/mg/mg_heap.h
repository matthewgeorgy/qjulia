#ifndef _MG_HEAP_H_
#define _MG_HEAP_H_

// Linear heap
typedef struct _TAG_mg_linheap
{
	u8				*memory;
	u32				size,
					offset;
	mg_handle_t		mutex;
} mg_linheap_t;

b32			mg_linheap_create(mg_linheap_t *heap, u32 initial_size);
b32			mg_linheap_destroy(mg_linheap_t *heap);
void		*mg_linheap_alloc(mg_linheap_t *heap, u32 size);
void		mg_linheap_free(mg_linheap_t *heap);

// Block heap

typedef struct _TAG_mg_blkheap
{
	u8		*memory;
	u32		blk_size,
			blk_cnt;
	b8		*free_list;
} mg_blkheap_t;

b32 		mg_blkheap_create(mg_blkheap_t *heap, u32 blk_size, u32 blk_cnt);
b32 		mg_blkheap_destroy(mg_blkheap_t *heap);
void		*mg_blkheap_alloc(mg_blkheap_t *heap);
void 		mg_blkheap_free(mg_blkheap_t *heap, void *ptr);

#ifdef MG_IMPL

b32
mg_linheap_create(mg_linheap_t *heap,
			   u32 initial_size)
{
	u32		size;


	if (!initial_size)
	{
		size = 16384; // default heap size is 16KB
	}
	else
	{
		size = initial_size;
	}

	heap->memory = (u8 *)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
	heap->size = size;
	heap->offset = 0;
	heap->mutex = mg_mutex_create();

	return (heap->memory != NULL);
}

b32
mg_linheap_destroy(mg_linheap_t *heap)
{
	b32		ret;


	ret = VirtualFree(heap->memory, 0, MEM_RELEASE);
	mg_mutex_destroy(heap->mutex);

	return (ret);
}

void *
mg_linheap_alloc(mg_linheap_t *heap,
			  u32 size)
{
	void		*ptr;
	u32			alignment;


	mg_mutex_lock(heap->mutex);

	if (size + heap->offset >= heap->size)
	{
		ptr = NULL;
	}
	else
	{
		ptr = heap->memory + heap->offset;
		heap->offset += size;
		alignment = heap->offset % 8;
		heap->offset += (8 - alignment);
	}

	mg_mutex_unlock(heap->mutex);

	return (ptr);
}

void
mg_linheap_free(mg_linheap_t *heap)
{
	mg_mutex_lock(heap->mutex);
	heap->offset = 0;
	mg_mutex_unlock(heap->mutex);
}

b32
mg_blkheap_create(mg_blkheap_t *heap,
			 	  u32 blk_size,
			 	  u32 blk_cnt)
{
	heap->memory = VirtualAlloc(NULL, blk_size * blk_cnt, MEM_COMMIT, PAGE_READWRITE);
	heap->blk_size = blk_size;
	heap->blk_cnt = blk_cnt;
	heap->free_list = mg_alloc(blk_cnt); // TODO(matthew): append this array to the main memory
	memset(heap->free_list, 1, blk_cnt);

	return (heap->memory != NULL);
}

void *
mg_blkheap_alloc(mg_blkheap_t *heap)
{
	void		*ptr = NULL;


	for (u32 i = 0; i < heap->blk_cnt; i++)
	{
		if (heap->free_list[i])
		{
			ptr = heap->memory + heap->blk_size  * i;
			heap->free_list[i] = 0;

			break;
		}
	}

	return (ptr);
}

void
mg_blkheap_free(mg_blkheap_t *heap,
		   		void *ptr)
{
	u32		idx;
	u8		*diff;


	diff = (u8 *)ptr - heap->memory;
	idx = (u32)diff / heap->blk_size;
	heap->free_list[idx] = 1;
}

#endif // MG_IMPL

#endif // _MG_HEAP_H_

