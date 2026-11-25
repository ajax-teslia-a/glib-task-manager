/*
 * task_manager_test.c
 *
 * Tests for the task manager (src/task_manager.c).
 *
 * Tests:
 *   Test_RunAll_EmptyQueue_ReturnsZero
 *   Test_RunAll_ExecutesInPriorityOrder
 *   Test_SamePriority_KeepsInsertionOrder
 */

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include "cmocka.h"

// ================== Example ==================
// #include "task_manager.h"
//
// static task_manager_t* tm;
// static int order[8];
// static int order_len;
//
// static int Setup(void** state)
// {
//     (void)state;
//     tm = TM_Create();
//     order_len = 0;
//     return 0;
// }
//
// static int Teardown(void** state)
// {
//     (void)state;
//     TM_Destroy(tm);
//     tm = NULL;
//     return 0;
// }
//
// static void record(gpointer user_data) { order[order_len++] = GPOINTER_TO_INT(user_data); }
//
// static void Test_RunAll_EmptyQueue_ReturnsZero(void** state)
// {
//     (void)state;
//     assert_int_equal(TM_RunAll(tm), 0);
// }
//
// static void Test_RunAll_ExecutesInPriorityOrder(void** state)
// {
//     (void)state;
//     TM_Push(tm, 1, record, GINT_TO_POINTER(1));
//     TM_Push(tm, 10, record, GINT_TO_POINTER(10));
//     TM_Push(tm, 5, record, GINT_TO_POINTER(5));
//
//     assert_int_equal(TM_RunAll(tm), 3);
//     assert_int_equal(order[0], 10);
//     assert_int_equal(order[1], 5);
//     assert_int_equal(order[2], 1);
// }
//
// static void Test_SamePriority_KeepsInsertionOrder(void** state)
// {
//     (void)state;
//     TM_Push(tm, 3, record, GINT_TO_POINTER(1));
//     TM_Push(tm, 3, record, GINT_TO_POINTER(2));
//     TM_Push(tm, 3, record, GINT_TO_POINTER(3));
//
//     TM_RunAll(tm);
//     assert_int_equal(order[0], 1);
//     assert_int_equal(order[1], 2);
//     assert_int_equal(order[2], 3);
// }

int main(void)
{
    // const struct CMUnitTest tests[] = {
    //     cmocka_unit_test_setup_teardown(Test_RunAll_EmptyQueue_ReturnsZero, Setup, Teardown),
    //     cmocka_unit_test_setup_teardown(Test_RunAll_ExecutesInPriorityOrder, Setup, Teardown),
    //     cmocka_unit_test_setup_teardown(Test_SamePriority_KeepsInsertionOrder, Setup, Teardown),
    // };
    // return cmocka_run_group_tests(tests, NULL, NULL);
    return 0;
}
