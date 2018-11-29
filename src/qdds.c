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

void *QDDS_AddItem(size_t stride, size_t length, enum heap_type type, void *heap, void *item, signed int (*ItemCmp)(void *, void *)){
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

void *QDDS_GetItem(void *heap){
    void *item = NULL;
    if(heap != NULL){
        item = heap;
    }
    return item;
}

void QDDS_FreeBinaryHeap(void *heap){
    free(heap);
}

