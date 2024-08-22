#include "mg_defs.h"

typedef struct _TAG_mg_array_header
{
    u64     capacity;   // Total number of elements
    u64     size;       // Number of elements in array containing data
} mg_array_header_t;

// Wrapper macro
#define mg_array(__type) \
    __type *

// Returns the header of the array
#define mg_array_head(__array) \
    ((mg_array_header_t *)(__array) - 1)

// Returns the size of the array
#define mg_array_size(__array) \
    (__array == NULL ? 0 : mg_array_head(__array)->size)

// Returns the capacity of the array
#define mg_array_capacity(__array) \
    (__array == NULL ? 0 : mg_array_head(__array)->capacity)

// Returns the length of the array (size * sizeof(type))
#define mg_array_len(__array) \
    (mg_array_size(__array) * (sizeof(*(__array))))

// Returns whether the array is full
#define mg_array_full(__array) \
    ((mg_array_size(__array)) == (mg_array_capacity(__array)))

// Returns whether the array is empty
#define mg_array_empty(__array) \
    (mg_array_init((void **)&(__array), sizeof(*(__array)), (mg_array_size(__array) == 0)))

// Frees the array
#define mg_array_free(__array) \
    (free(mg_array_head(__array)))

// Returns whether the array needs to grow by a given size
#define mg_array_need_grow(__array, __n) \
    ((__array) == 0 || mg_array_size(__array) + (__n) > mg_array_capacity(__array))

// Grows the array (doubles in size)
#define mg_array_grow(__array) \
    mg_array_resize((__array), sizeof(*(__array)), mg_array_capacity(__array) ? mg_array_capacity(__array) * 2 : 1)

// Grows the array by a given size
#define mg_array_grow_size(__array, __sz) \
    mg_array_resize((__array), sizeof(*(__array)), mg_array_capacity(__array) ? mg_array_capacity(__array) + (__sz) : 1)

// Pushes data to the back of the array
#define mg_array_push(__array, __data)                                       \
    do                                                                      \
    {                                                                       \
        mg_array_init((void **)&(__array), sizeof(*(__array)));              \
        if (!(__array) || ((__array) && mg_array_need_grow(__array, 1)))     \
        {                                                                   \
            *((void **)&(__array)) = mg_array_grow(__array);                 \
        }                                                                   \
        (__array)[mg_array_size(__array)] = (__data);                        \
        mg_array_head(__array)->size++;                                      \
    } while (0)

// Pops data off the array (decrements the size) and returns it
#define mg_array_pop(__array) \
    ((mg_array_size(__array) > 0) ? (__array)[(--mg_array_head(__array)->size)] : (__array)[(mg_array_head(__array)->size)])

// Clears all the elements in the array, (simply sets size to 0)
#define mg_array_clear(__array)                  \
    do                                          \
    {                                           \
        if (__array)                            \
            mg_array_head(__array)->size = 0;    \
    } while (0)

// Reserves internal space in the array for __amount, non-initialized elements
#define mg_array_reserve(__array, __amount)                                                  \
    do                                                                                      \
    {                                                                                       \
        if ((!__array))                                                                     \
            mg_array_init((void **)&(__array), sizeof(*(__array)));                          \
        if ((!__array) || (usize)__amount > mg_array_capacity(__array))                      \
        {                                                                                   \
            *((void **)&(__array)) = mg_array_resize(__array, sizeof(*(__array)), __amount); \
        }                                                                                   \
    } while (0)

// Shrinks array such that capacity = size (no excess memory usage for unused elements)
#define mg_array_shrink(__array) \
    *((void **)&(__array)) = mg_array_resize(__array, sizeof(*(__array)), mg_array_size(__array));

// Inserts data into the array at the specified position (0 indexing)
// NOTE: DOES NOT WORK AT INDEX 0 (ie, cannot insert at start of array)
#define mg_array_insert(__array, __val, __pos)                                                       \
    do                                                                                              \
    {                                                                                               \
        u32 mg__arr_iter;                                                                          \
        if ((mg_array_need_grow(__array, 1)))                                                        \
            *((void **)&(__array)) = mg_array_grow_size(__array, 1);                                 \
        for (mg__arr_iter = mg_array_size(__array) - 1; mg__arr_iter >= (__pos); mg__arr_iter--)  \
        {                                                                                           \
            (__array)[mg__arr_iter + 1] = (__array)[mg__arr_iter];                                \
        }                                                                                           \
        (__array)[(__pos)] = (__val);                                                               \
        mg_array_head(__array)->size++;                                                              \
    } while (0);

// Removes data from the array at the specified position and realigns elements
#define mg_array_remove(__array, __pos)                                                          \
    do                                                                                          \
    {                                                                                           \
        u32 mg__arr_iter;                                                                      \
        for (mg__arr_iter = (__pos); mg__arr_iter < mg_array_size(__array); mg__arr_iter++)   \
        {                                                                                       \
            (__array)[mg__arr_iter] = (__array)[mg__arr_iter + 1];                            \
        }                                                                                       \
        mg_array_head(__array)->size--;                                                          \
    } while (0);

// Initializes the array
static void **
mg_array_init(void **arr,
             usize val_size)
{
    mg_array_header_t        *data;


    if (*arr == NULL)
    {
        data = (mg_array_header_t *)mg_alloc(val_size + sizeof(mg_array_header_t));

        if (data)
        {
            data->size = 0;
            data->capacity = 1;
            *arr = ((mg_array_header_t *)data + 1);

            return arr;
        }
    }

    return NULL;
}

// Resizes the array
static void *
mg_array_resize(void *arr,
               usize sz,
               usize amt)
{
    usize                   capacity;
    mg_array_header_t        *data;


    if (arr)
        capacity = amt;
    else
        capacity = 0;

    // Create new array with header + desired size (or 0 if arr is empty)
    data = (mg_array_header_t *)mg_realloc(arr ? mg_array_head(arr) : 0, capacity * sz + sizeof(mg_array_header_t));

    if (data)
    {
        if (!arr)
            data->size = 0;

        data->capacity = (u32)capacity;
        data = (mg_array_header_t *)data + 1;
    }

    return data;
}
