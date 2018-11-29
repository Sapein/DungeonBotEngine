#ifndef S_QDDS
#include <stddef.h>
#define S_QDDS

enum heap_type {MIN, MAX};

/* QDDS_CreateBinaryHeap
 * Args: stride (size_t), length (size_t)
 * Returns: Binary Heap (void *)
 *
 * This function returns a 'binary heap' pointer with the given stride.
 * If there is an error it returns NULL.
 * NOTE: You will need to free this with QDDS_FreeBinaryHeap.
 */
void *QDDS_CreateBinaryHeap(size_t stride, size_t length);

/* QDDS_AddItem
 * Args: stride (size_t), heap_type (enum) heap (void *), item (void *), ItemCmp (signed int (*)(void *, void *))
 * Returns: Binary Heap (void *)
 *
 * This function returns a 'binary heap' pointer with the item added. Stride is required
 *  as stride will be unknown. This can use a heap as allocated by CreateBinaryHeap or
 *  you can merely pass in an array.
 * You must give it a comparision call-back.
 *
 * ItemCmp must return an integer as follows (-2 = Uncomparable, -1 = First item is < Second Item
 *                                             0 = Equality ; 1 = First item > Second Item)
 */
void *QDDS_AddItem(size_t stride, size_t length, enum heap_type type, void *heap, void *item, signed int (*ItemCmp)(void *, void *));

/* QDDS_GetItem
 * Args: heap (void *)
 * Returns: item (void *)
 *
 * This function returns the first item in the heap.
 * It expects the user to know the proper type and cast to it
 */
void *QDDS_GetItem(void *heap);

/* QDDS_FreeBinaryHeap
 * Args: heap (void *)
 * Returns: None
 *
 * This function frees a Binary Heap created through QDDS_CreateBinaryHeap()
 */
void QDDS_FreeBinaryHeap(void *heap);
#endif
