#pragma once
#include "tag.h"
#include "task.h"

typedef struct
{
    GSequence *tasks;        // owns Task* instances
    GHashTable *task_by_id;  // maps task id to sequence iter
    guint next_task_id;
    tag_index_t tags;  // maps tags to ids bidirectionally
} task_db_t;

task_db_t *TaskDB_Create(void);
void TaskDB_Delete(task_db_t *db);

const task_t *TaskDB_Add(task_db_t *db, gint priority, const gchar *title);
gboolean TaskDB_Remove(task_db_t *db, guint id);
const task_t *TaskDB_GetById(task_db_t *db, guint id);
