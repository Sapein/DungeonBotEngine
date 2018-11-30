#include <criterion/criterion.h>
#include "../src/qdds.h"
#define ARR_SIZE 3
#define ARR_INIT {0, 0, 0}

int IntCmp(void *first, void *second){
    int a = *((int *)first);
    int b = *((int *)second);
    if(a < b) return -1;
    if(a == b) return 0;
    if(a > b) return 1;
    return -2;
}

Test(qdds_heap, create){
    void *v_arr_test = NULL;
    int *arr_test = NULL;
    v_arr_test = QDDS_CreateBinaryHeap(sizeof(int), ARR_SIZE);
    arr_test = (int *)v_arr_test;
    for(int i = 0; i >= ARR_SIZE; i++){
        cr_assert_eq(arr_test[i], 0);
    }
    arr_test = NULL;
    free(v_arr_test);
}

Test(qdds_heap, add_min){
    int arr_test[ARR_SIZE] = {0};
    int a = 1;
    if(QDDS_AddHeapItem(sizeof(int), ARR_SIZE, MIN, arr_test, (void *)&a, *IntCmp) != NULL){
        cr_assert_eq(arr_test[ARR_SIZE - 1], a);
        for(int i = 0; i < ARR_SIZE - 1; i++){
            cr_assert_eq(arr_test[i], 0);
        }
    }else{
        cr_assert_fail();
    }
}

Test(qdds_heap, add_max){
    int arr_test[ARR_SIZE] = {0};
    int a = 1;
    if(QDDS_AddHeapItem(sizeof(int), ARR_SIZE, MAX, arr_test, (void *)&a, *IntCmp) != NULL){
        cr_assert_eq(arr_test[0], a);
        for(int i = 1; i < ARR_SIZE; i++){
            cr_assert_eq(arr_test[i], 0);
        }
    }else{
        cr_assert_fail();
    }
}

Test(qdds_heap, get_min){
    int arr_test[ARR_SIZE] = {0};
    int a = 1;
    if(QDDS_AddHeapItem(sizeof(int), ARR_SIZE, MIN, arr_test, (void *)&a, *IntCmp) != NULL){
        cr_assert_eq(*((int *)QDDS_GetHeapRootItem(arr_test)), 0);
    }else{
        cr_assert_fail();
    }
}

Test(qdds_heap, get_max){
    int arr_test[ARR_SIZE] = {0};
    int a = 1;
    if(QDDS_AddHeapItem(sizeof(int), ARR_SIZE, MAX, arr_test, (void *)&a, *IntCmp) != NULL){
        cr_assert_eq(*((int *)QDDS_GetHeapRootItem(arr_test)), 1);
    }else{
        cr_assert_fail();
    }
}

Test(qdds_heap, pop_max){
    int arr_test[ARR_SIZE] = {0};
    int a = 1;
    if(QDDS_AddHeapItem(sizeof(int), ARR_SIZE, MAX, arr_test, (void *)&a, *IntCmp) != NULL){
        cr_assert_eq(*((int *)QDDS_PopHeapRootItem(sizeof(int), ARR_SIZE, MAX, arr_test, *IntCmp)), 1);
    }else{
        cr_assert_fail();
    }
}

Test(qdds_heap, pop_max_err){
    int arr_test[ARR_SIZE] = {0};
    int a = 1;
    if(QDDS_AddHeapItem(sizeof(int), ARR_SIZE, MAX, arr_test, (void *)&a, *IntCmp) != NULL){
        cr_assert_null(QDDS_PopHeapRootItem(sizeof(int), ARR_SIZE, MAX, arr_test, NULL));
        cr_assert_null(QDDS_PopHeapRootItem(sizeof(int), ARR_SIZE, MAX, NULL, *IntCmp));
        cr_assert_null(QDDS_PopHeapRootItem(sizeof(int), ARR_SIZE, 100, arr_test, *IntCmp));
        cr_assert_null(QDDS_PopHeapRootItem(sizeof(int), ARR_SIZE, 100, NULL, NULL));
    }else{
        cr_assert_fail();
    }
}


Test(qdds_heap, pop_max_s){
    int arr_test[ARR_SIZE] = {0};
    int a = 1;
    int b = 0;
    if(QDDS_AddHeapItem(sizeof(int), ARR_SIZE, MAX, arr_test, (void *)&a, *IntCmp) != NULL){
        cr_assert_eq(QDDS_PopHeapRootItem_s(sizeof(int), ARR_SIZE, MAX, arr_test, &b, *IntCmp), BH_NONE);
        cr_assert_eq(b, 1);
        cr_assert_eq(a, b);
        a = 2;
        cr_assert_eq(a, 2);
        cr_assert_eq(b, 1);
        cr_assert_neq(a, b);
    }else{
        cr_assert_fail();
    }
}

Test(qdds_heap, pop_max_err_s){
    int arr_test[ARR_SIZE] = {0};
    int a = 1;
    int b = 0;
    if(QDDS_AddHeapItem(sizeof(int), ARR_SIZE, MAX, arr_test, (void *)&a, *IntCmp) != NULL){
        cr_assert_eq(QDDS_PopHeapRootItem_s(sizeof(int), ARR_SIZE, 100, arr_test, &b, *IntCmp), INVALID_TYPE);
        cr_assert_eq(QDDS_PopHeapRootItem_s(sizeof(int), ARR_SIZE, MAX, NULL, &b, *IntCmp), INVALID_HEAP);
        cr_assert_eq(QDDS_PopHeapRootItem_s(sizeof(int), ARR_SIZE, MAX, arr_test, NULL, *IntCmp), INVALID_ITEM_OUT);
        cr_assert_eq(QDDS_PopHeapRootItem_s(sizeof(int), ARR_SIZE, MAX, arr_test, &b, NULL), INVALID_CB);
        cr_assert_neq(QDDS_PopHeapRootItem_s(sizeof(int), ARR_SIZE, 100, NULL, NULL, NULL), BH_NONE);
    }else{
        cr_assert_fail();
    }
}
