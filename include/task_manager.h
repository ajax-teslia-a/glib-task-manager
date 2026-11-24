#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <glib.h>

typedef struct task_manager_s task_manager_t;

typedef void (*tm_task_func_t)(gpointer user_data);

task_manager_t* TM_Create(void);
void TM_Destroy(task_manager_t* tm);

// Queues a task; higher priority runs first. Returns task id.
guint TM_Push(task_manager_t* tm, gint priority, tm_task_func_t func, gpointer user_data);
// Runs all queued tasks in priority order. Returns number of executed tasks.
guint TM_RunAll(task_manager_t* tm);

#endif  // TASK_MANAGER_H
