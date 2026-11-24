#include "task_manager.h"

typedef struct {
    guint id;
    gint priority;
    tm_task_func_t func;
    gpointer user_data;
} task_t;

struct task_manager_s {
    GQueue* queue;
    guint next_id;
};

static gint compare_tasks(gconstpointer a, gconstpointer b, gpointer user_data)
{
    const task_t* ta = a;
    const task_t* tb = b;
    if (ta->priority != tb->priority) {
        return tb->priority - ta->priority;
    }
    return (gint)ta->id - (gint)tb->id;
}

task_manager_t* TM_Create(void)
{
    task_manager_t* tm = g_new0(task_manager_t, 1);
    tm->queue = g_queue_new();
    return tm;
}

void TM_Destroy(task_manager_t* tm)
{
    if (!tm) {
        return;
    }
    g_queue_free_full(tm->queue, g_free);
    g_free(tm);
}

guint TM_Push(task_manager_t* tm, gint priority, tm_task_func_t func, gpointer user_data)
{
    g_return_val_if_fail(tm && func, 0);
    task_t* task = g_new0(task_t, 1);
    task->id = ++tm->next_id;
    task->priority = priority;
    task->func = func;
    task->user_data = user_data;
    g_queue_insert_sorted(tm->queue, task, compare_tasks, NULL);
    return task->id;
}

guint TM_RunAll(task_manager_t* tm)
{
    g_return_val_if_fail(tm, 0);
    guint count = 0;
    task_t* task;
    while ((task = g_queue_pop_head(tm->queue))) {
        task->func(task->user_data);
        g_free(task);
        count++;
    }
    return count;
}
