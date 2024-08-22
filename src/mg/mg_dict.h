#ifndef _MG_DICT_H_
#define _MG_DICT_H_

#include "mg_defs.h"

// 71 or 73
#define		MG_SDICT_SIZE	101

typedef struct _TAG_mg_sdict_entry		mg_sdict_entry_t;
typedef struct _TAG_mg_sdict			mg_sdict_t;
typedef struct _TAG_mg_pdict_entry		mg_pdict_entry_t;
typedef struct _TAG_mg_pdict			mg_pdict_t;

// string -> ptr hash table
MG_EXTERN u32		mg_sdict_hashkey(char *str);
MG_EXTERN void		mg_sdict_create(mg_sdict_t *ht);
MG_EXTERN void		mg_sdict_destroy(mg_sdict_t *ht);
MG_EXTERN b32		mg_sdict_insert(mg_sdict_t *ht, char *key, void *data);
MG_EXTERN b32		mg_sdict_lookup(mg_sdict_t *ht, char *key, void *data);
MG_EXTERN void		mg_sdict_print(mg_sdict_t *ht);

// ptr -> ptr hash table
MG_EXTERN u32		mg_pdict_hashkey(char *str);
MG_EXTERN void		mg_pdict_create(mg_pdict_t *ht);
MG_EXTERN void		mg_pdict_destroy(mg_pdict_t *ht);
MG_EXTERN b32		mg_pdict_insert(mg_pdict_t *ht, void *key, void *data);
MG_EXTERN b32		mg_pdict_lookup(mg_pdict_t *ht, void *key, void *data);
MG_EXTERN void		mg_pdict_print(mg_pdict_t *ht);

struct _TAG_mg_sdict_entry
{
	void		*data;
	char		key[256];
	b32			used;
};

struct _TAG_mg_sdict
{
	u32								cnt;
	mg_handle_t						mutex;
	mg_array(mg_sdict_entry_t)		entries[MG_SDICT_SIZE];
};

struct _TAG_mg_pdict_entry
{
	void		*data,
				*key;
	b32			used;
};

struct _TAG_mg_pdict
{
	u32								cnt;
	mg_handle_t						mutex;
	mg_array(mg_pdict_entry_t)		entries[MG_SDICT_SIZE];
};

#ifdef MG_IMPL

u32
mg_sdict_hashkey(char *str)
{
	u32		hash = 5381;
	s32		c;


	while (c = *str++)
	{
		hash = (hash * 33) + c;
	}

	return (hash % MG_SDICT_SIZE);
}

void
mg_sdict_create(mg_sdict_t *ht)
{
	u32		i;


	memset(ht->entries, 0, sizeof(mg_sdict_entry_t *) * MG_SDICT_SIZE);
	ht->cnt = 0;
	ht->mutex = mg_mutex_create();

	for(i = 0; i < MG_SDICT_SIZE; i++)
	{
		mg_array_reserve(ht->entries[i], 10 * sizeof(mg_sdict_entry_t));
		memset(ht->entries[i], 0, 10 * sizeof(mg_sdict_entry_t));
	}
}

b32
mg_sdict_insert(mg_sdict_t *ht,
			 	char *key,
			 	void *data)
{
	u32						hash;
	mg_sdict_entry_t		new;


	mg_mutex_lock(ht->mutex);

	hash = mg_sdict_hashkey(key);
	ht->cnt++;

	new.data = data;
	strcpy(new.key, key);
	new.used = TRUE;

	mg_array_push(ht->entries[hash], new);

	mg_mutex_unlock(ht->mutex);

	return (TRUE);
}

b32
mg_sdict_lookup(mg_sdict_t *ht,
			 	char *key,
			 	void **data)
{
	b32		ret = FALSE;
	u32		hash,
			i,
			size;


	hash = mg_sdict_hashkey(key);
	size = mg_array_size(ht->entries[hash]);

	for (i = 0; i < size; i++)
	{
		if (!strcmp(key, ht->entries[hash][i].key))
		{
			*data = ht->entries[hash][i].data;
			ret = TRUE;
			break;
		}
	}

	return (ret);
}

void
mg_sdict_print(mg_sdict_t *ht)
{
	u32		i, j,
			size,
			cnt = 0;


	for (i = 0; i < MG_SDICT_SIZE; i++)
	{
		if (ht->entries[i]->used)
		{
			size = mg_array_size(ht->entries[i]);
			printf("[%d]:", i);

			for (j = 0; j < size; j++)
			{
				printf(" [%s,0x%p]", ht->entries[i][j].key, ht->entries[i][j].data);
			}

			printf("\n");
			cnt++;
		}
	}
	printf("load: %d / %d\n", cnt, ht->cnt);
}

void
mg_sdict_destroy(mg_sdict_t *ht)
{
	u32		i;


	for (i = 0; i < MG_SDICT_SIZE; i++)
	{
		if (ht->entries[i])
		{
			mg_array_free(ht->entries[i]);
		}
	}
}

u32		
mg_pdict_hashkey(char *str)
{
}

void		
mg_pdict_create(mg_pdict_t *ht)
{
}

void		
mg_pdict_destroy(mg_pdict_t *ht)
{
}

b32		
mg_pdict_insert(mg_pdict_t *ht, 
				void *key, 
				void *data)
{
}

b32		
mg_pdict_lookup(mg_pdict_t *ht, 
				void *key, 
				void *data)
{
}

void		
mg_pdict_print(mg_pdict_t *ht)
{
}

#endif // MG_IMPL

#endif // _MG_DICT_H_

