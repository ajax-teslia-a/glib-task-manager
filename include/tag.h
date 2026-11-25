#pragma once
#include "task.h"

typedef struct
{
    guint id;
    gchar *name;
} tag_t;

typedef struct
{
    // a tag_t* must be removed from tasks_by_tag and tags_by_task BEFORE it is removed from tags_by_name
    GHashTable *tags_by_name;  // name (string) -> tag_t*; owns the tag_t
    GHashTable *tasks_by_tag;  // tag_t* -> set of task ids; owns the set
    GHashTable *tags_by_task;  // task id -> set of tag_t*; owns the set
    guint next_tag_id;
} tag_index_t;

void Tag_IndexInit(tag_index_t *tag_db);
void Tag_IndexDestroy(tag_index_t *tag_db);

gboolean Tag_AddToTask(tag_index_t *tag_db, task_t *task, const char *tag_name);
gboolean Tag_RemoveFromTask(tag_index_t *tag_db, task_t *task, const char *tag_name);
gboolean Tag_RemoveAllFromTask(tag_index_t *tag_db, task_t *task);
GPtrArray *Tag_GetAllTasks(tag_index_t *tag_db, const char *tag_name);
