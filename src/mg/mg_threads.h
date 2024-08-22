#ifndef _MG_THREADS_H
#define _MG_THREADS_H

#include "mg_defs.h"

// Thread
MG_EXTERN mg_handle_t		mg_thread_create(LPTHREAD_START_ROUTINE threadproc, void *param);
MG_EXTERN b32				mg_thread_destroy(mg_handle_t thread);

// Mutex
MG_EXTERN mg_handle_t		mg_mutex_create();
MG_EXTERN void				mg_mutex_destroy(mg_handle_t mutex);
MG_EXTERN void				mg_mutex_lock(mg_handle_t mutex);
MG_EXTERN b32				mg_mutex_trylock(mg_handle_t mutex);
MG_EXTERN void				mg_mutex_unlock(mg_handle_t mutex);

// Handle
MG_EXTERN u32				mg_handle_wait(u32 cnt, mg_handle_t *handles, b32 wait_all, u32 wait_amount);


#ifdef MG_IMPL

mg_handle_t
mg_thread_create(LPTHREAD_START_ROUTINE threadproc,
				 void *param)
{
	mg_handle_t		thread;


	thread = CreateThread(NULL, 0, threadproc, param, 0, NULL);

	return (thread);
}

b32
mg_thread_destroy(mg_handle_t thread)
{
	return (CloseHandle(thread));
}

mg_handle_t
mg_mutex_create()
{
	CRITICAL_SECTION		*mutex;


	mutex = (CRITICAL_SECTION *)mg_alloc(sizeof(*mutex));
	InitializeCriticalSection(mutex);

	return (mutex);
}

void
mg_mutex_destroy(mg_handle_t mutex)
{
	DeleteCriticalSection((LPCRITICAL_SECTION)mutex);
	mg_free(mutex);
}

void
mg_mutex_lock(mg_handle_t mutex)
{
	EnterCriticalSection((LPCRITICAL_SECTION)mutex);
}

b32
mg_mutex_trylock(mg_handle_t mutex)
{
	return (TryEnterCriticalSection((LPCRITICAL_SECTION)mutex));
}

void
mg_mutex_unlock(mg_handle_t mutex)
{
	LeaveCriticalSection((LPCRITICAL_SECTION)mutex);
}

u32
mg_handle_wait(u32 cnt,
			   mg_handle_t *handles,
			   b32 wait_all,
			   u32 wait_amount)
{
	return (WaitForMultipleObjects(cnt, handles, wait_all, wait_amount));
}

#endif // MG_IMPL

#endif // _MG_THREADS_H

