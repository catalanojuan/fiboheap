#include <stdio.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "fiboheap.h"

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

void get_several_max_test(void) {
    Fiboheap heap = create_heap(MAX);
    insert_key(heap, 3);
    insert_key(heap, 7);
    insert_key(heap, 2);
    insert_key(heap, 13);
    insert_key(heap, 14);
    insert_key(heap, 21);
    insert_key(heap, 8);
    insert_key(heap, 9);
    insert_key(heap, 4);
    insert_key(heap, 11);
    
    CU_ASSERT_EQUAL(pop_max(heap), 21);
    CU_ASSERT_EQUAL(pop_max(heap), 14);
    CU_ASSERT_EQUAL(pop_max(heap), 13);
    CU_ASSERT_EQUAL(pop_max(heap), 11);

    destroy(heap);
}

void get_several_min_test(void) {
    Fiboheap heap = create_heap(MIN);
    insert_key(heap, 3);
    insert_key(heap, 7);
    insert_key(heap, 2);
    insert_key(heap, 13);
    insert_key(heap, 14);
    insert_key(heap, 21);
    insert_key(heap, 8);
    insert_key(heap, 9);
    insert_key(heap, 4);
    insert_key(heap, 11);
    
    CU_ASSERT_EQUAL(pop_min(heap), 2);
    CU_ASSERT_EQUAL(pop_min(heap), 3);
    CU_ASSERT_EQUAL(pop_min(heap), 4);
    CU_ASSERT_EQUAL(pop_min(heap), 7);

    destroy(heap);
}

void get_one_max_test(void) {
    Fiboheap heap = create_heap(MAX);
    
    insert_key(heap, 11);
    
    CU_ASSERT_EQUAL(11, pop_max(heap));
    destroy(heap);
}

void get_one_min_test(void) {
    Fiboheap heap = create_heap(MIN);
    
    insert_key(heap, 2);
    
    CU_ASSERT_EQUAL(2, pop_min(heap));
    destroy(heap);
}

int main ( void ){
    CU_pSuite pSuite = NULL;

    if ( CUE_SUCCESS != CU_initialize_registry() )
        return CU_get_error();

    pSuite = CU_add_suite( "fiboheap_test_suite", init_suite, clean_suite );
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if((NULL == CU_add_test(pSuite, "get_one_max_test", get_one_max_test)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if((NULL == CU_add_test(pSuite, "get_several_max_test", get_several_max_test)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if((NULL == CU_add_test(pSuite, "get_one_min_test", get_one_min_test)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if((NULL == CU_add_test(pSuite, "get_several_min_test", get_several_min_test)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");

    CU_cleanup_registry();
    return CU_get_error();
}
