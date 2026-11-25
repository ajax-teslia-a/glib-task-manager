#include "task.h"
#include "util.h"

gboolean Task_IsValid(const task_t *task) { return (task != NULL && task->tags != NULL && string_is_valid(task->title) == TRUE); }

task_t *Task_New(guint id, gint priority, const gchar *title)
{
    task_t *t = g_new0(task_t, 1);
    t->id = id;
    t->title = g_strdup(title);
    t->priority = priority;
    t->tags = g_ptr_array_new();
    t->created_at = g_get_monotonic_time();
    return t;
}

void Task_Free(gpointer data)
{
    task_t *t = (task_t *)data;
    if (!t) {
        return;
    }

    g_free(t->title);
    g_ptr_array_free(t->tags, TRUE);
    g_free(t);
}
