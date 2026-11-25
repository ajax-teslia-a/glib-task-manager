#pragma once
#include <glib.h>

typedef struct
{
    guint id;
    gchar *title;
    gint priority;
    GPtrArray *tags;    // gchar*
    gint64 created_at;  // g_get_monotonic_time()
} task_t;

task_t *Task_New(guint id, gint priority, const gchar *title);
void Task_Free(gpointer data);

gboolean Task_IsValid(const task_t *task);
