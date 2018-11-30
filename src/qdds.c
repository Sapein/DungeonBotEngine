#include <stdlib.h>
#include <string.h>
#include "qdds.h"

void *QDDS_CreateBinaryHeap(size_t stride, size_t length){
    void *heap = NULL;
    if(stride > 0){
        heap = calloc(length, stride);
    }
    return heap;
}

void *QDDS_AddHeapItem(size_t stride, size_t length, enum heap_type type, void *heap, void *item, signed int (*ItemCmp)(void *, void *)){
    void *_heap = heap;
    char *current = NULL;
    int index = (length - 1 );
    if(heap != NULL && item != NULL && ItemCmp != NULL){
        current = ((char *)_heap + (index * stride));
        memcpy(current, item, stride);
        if(index % 2 == 0) index = (index / 2) - 1;
        if(index % 2 == 1) index = (index / 2);
        current = ((char *)_heap + (index * stride));
        if(type == MIN){
            while(index > 0 || ItemCmp(item, current) < 0){
                memcpy((current + (((index * 2) + 2) * stride)), current, stride);
                memcpy(current, item, stride);
                if(index % 2 == 0) index = (index / 2) - 1;
                if(index % 2 == 1) index = (index / 2);
                current = ((char *)_heap + (index * stride));
            }
        }else if(type == MAX){
            while(index > 0 || ItemCmp(item, current) > 0){
                memcpy((current + (((index * 2) + 2) * stride)), current, stride);
                memcpy(current, item, stride);
                if(index % 2 == 0) index = (index / 2) - 1;
                if(index % 2 == 1) index = (index / 2);
                current = ((char *)_heap + (index * stride));
            }
        }else{
            _heap = NULL;
        }
    }
    return _heap;
}

void *QDDS_GetHeapRootItem(void *heap){
    void *item = NULL;
    if(heap != NULL){
        item = heap;
    }
    return item;
}

int QDDS_PopHeapRootItem_s(size_t stride, size_t length, enum heap_type type, void *heap, void *item,
                           signed int (*ItemCmp)(void *, void *)){
    enum QDDS_BinaryHeapErrors err = BH_NONE;
    void *_heap = heap;
    char *current_left = NULL;
    char *current_right = NULL;
    char *last = NULL;
    int index = (length - 1);
    int left_child = 0;
    int right_child = 0;
    int last_node = index;
    int left_cmp = 0;
    int right_cmp = 0;
    if(heap != NULL && item != NULL && ItemCmp != NULL){
        if(type != MIN && type != MAX){
            err = INVALID_TYPE;
            goto end;
        }
        memcpy(item, heap, stride * sizeof(unsigned char));
        memset(heap, 0, stride * sizeof(unsigned char));
        while(memcmp(heap, (char *)heap + (index * stride), stride * sizeof(unsigned char)) == 0 && index >= 0){
            /* Find the actual last node in the array */
            index--;
        }

        if(index <= 0){
            goto end;
        }

        last_node = index;
        memcpy(heap, (char *)heap + (last_node * stride), stride * sizeof(unsigned char));
        last = (char *)heap + (last_node * stride);

        /* Now we begin the 'Bubble Down' Process */
        index = 0; /* Begin on the 'left' */
        left_child = index * 2 + 1;
        right_child = index * 2 + 2;
        current_left = (char *)heap + (stride * left_child);
        current_right = (char *)heap + (stride * right_child);
        if(type == MIN){
            while(index < last_node && ((left_cmp = ItemCmp(item, current_left)) > 0 ||
                                        (right_cmp = ItemCmp(item, current_right)) > 0)){
                if(left_cmp > 0){
                    /* If we are in the left cmp */
                    memcpy(heap, current_left, stride * sizeof(unsigned char));
                    memcpy(current_left, last, stride * sizeof(unsigned char));
                    index = left_child;
                }else if(right_cmp > 0){
                    /* If we are in the right cmp */
                    memcpy(heap, current_right, stride * sizeof(unsigned char));
                    memcpy(current_right, last, stride * sizeof(unsigned char));
                    index = right_child;
                }
                left_child = (index * 2) + 1;
                right_child = (index * 2) + 2;
                current_left = (char *)heap + (stride * left_child);
                current_right = (char *)heap + (stride * right_child);
            }
        }else if(type == MAX){
            while(index < last_node && ((left_cmp = ItemCmp(item, current_left)) < 0 ||
                                        (right_cmp = ItemCmp(item, current_right)) < 0)){
                if(left_cmp < 0){
                    /* If we are in the left cmp */
                    memcpy(heap, current_left, stride * sizeof(unsigned char));
                    memcpy(current_left, last, stride * sizeof(unsigned char));
                    index = left_child;
                }else if(right_cmp < 0){
                    /* If we are in the right cmp */
                    memcpy(heap, current_right, stride * sizeof(unsigned char));
                    memcpy(current_right, last, stride * sizeof(unsigned char));
                    index = right_child;
                }
                left_child = (index * 2) + 1;
                right_child = (index * 2) + 2;
                current_left = (char *)heap + (stride * left_child);
                current_right = (char *)heap + (stride * right_child);
            }
        }else{
            err = INVALID_TYPE;
        }
    }else{
        if(heap == NULL){
            err = INVALID_HEAP;
        }else if(item == NULL){
            err = INVALID_ITEM_OUT;
        }else if(ItemCmp == NULL){
            err = INVALID_CB;
        }
    }
end:
    return err;
}

void *QDDS_PopHeapRootItem(size_t stride, size_t length, enum heap_type type, void *heap, signed int (*ItemCmp)(void *, void *)){
    void *item = calloc(1, stride);
    if(item != NULL){
        if(QDDS_PopHeapRootItem_s(stride, length, type, heap, item, ItemCmp) != 0){
            free(item);
            item = NULL;
        }
    }
    return item;
}

void QDDS_FreeBinaryHeap(void *heap){
    free(heap);
}

