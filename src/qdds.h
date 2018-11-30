#ifndef S_QDDS
#include <stddef.h>
#define S_QDDS

enum QDDS_BinaryHeapErrors {BH_NONE, INVALID_HEAP, INVALID_ITEM_OUT, INVALID_TYPE, INVALID_CB};
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

/* QDDS_AddHeapItem
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
void *QDDS_AddHeapItem(size_t stride, size_t length, enum heap_type type, void *heap, void *item, signed int (*ItemCmp)(void *, void *));

/* QDDS_GetHeapRootItem
 * Args: heap (void *)
 * Returns: item (void *)
 *
 * This function returns the first item in the heap.
 * It expects the user to know the proper type and cast to it
 */
void *QDDS_GetHeapRootItem(void *heap);

/* QDDS_PopHeapRootItem
 * Args: stride (size_t), length (size_t), type (heap_type), heap (void *), ItemCmp (signed int (*)(void *, void *))
 * Returns: item (void *)
 *
 * This function returns the first item in the heap, and then re-heapifies it
 * It expects the user to know what the returned type is.
 *
 * Please note the returned item is a COPY of the original, and as such pointers to the original are no longer valid.
 * You MUST free the returned item after you are done with it, or it will cause a memory leak.
 */
void *QDDS_PopHeapRootItem(size_t stride, size_t length, enum heap_type type, void *heap , signed int (*ItemCmp)(void *, void *));

/* QDDS_PopHeapRootItem_s
 * Args: stride (size_t), length (size_t), type (heap_type), heap (void *), item (void *) [out]
 *  ItemCmp (signed int (*)(void *, void *))
 * Returns: int (0 on success)
 *
 * This function is a 'safe' version of QDDS_PopHeadRootItem(), without having the library allocate or free anything.
 * It expects the user to know what the returned type is.
 *
 * Please note that the returned item will be a copy of the original.
 */
int QDDS_PopHeapRootItem_s(size_t stride, size_t length, enum heap_type type, void *heap, void *item,
                           signed int (*ItemCmp)(void *, void *));


/* QDDS_FreeBinaryHeap
 * Args: heap (void *)
 * Returns: None
 *
 * This function frees a Binary Heap created through QDDS_CreateBinaryHeap()
 */
void QDDS_FreeBinaryHeap(void *heap);
#endif
