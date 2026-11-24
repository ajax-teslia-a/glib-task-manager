#include <glib.h>
#include "task_manager.h"

static void print_task(gpointer user_data) { g_print("task: %s\n", (const char*)user_data); }

int main(void)
{
    task_manager_t* tm = TM_Create();
    TM_Push(tm, 1, print_task, "low priority");
    TM_Push(tm, 10, print_task, "high priority");
    TM_Push(tm, 5, print_task, "medium priority");
    guint done = TM_RunAll(tm);
    g_print("executed %u tasks\n", done);
    TM_Destroy(tm);
    return 0;
}
