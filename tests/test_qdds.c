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
}

Test(qdds_heap, add_min){
    int arr_test[ARR_SIZE] = {0};
    int a = 1;
    if(QDDS_AddItem(sizeof(int), ARR_SIZE, MIN, arr_test, (void *)&a, *IntCmp) != NULL){
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
    if(QDDS_AddItem(sizeof(int), ARR_SIZE, MAX, arr_test, (void *)&a, *IntCmp) != NULL){
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
    if(QDDS_AddItem(sizeof(int), ARR_SIZE, MIN, arr_test, (void *)&a, *IntCmp) != NULL){
        cr_assert_eq(*((int *)QDDS_GetItem(arr_test)), 0);
    }else{
        cr_assert_fail();
    }
}

Test(qdds_heap, get_max){
    int arr_test[ARR_SIZE] = {0};
    int a = 1;
    if(QDDS_AddItem(sizeof(int), ARR_SIZE, MAX, arr_test, (void *)&a, *IntCmp) != NULL){
        cr_assert_eq(*((int *)QDDS_GetItem(arr_test)), 1);
    }else{
        cr_assert_fail();
    }
}
