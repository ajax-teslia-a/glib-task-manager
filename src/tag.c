#include "tag.h"
#include "util.h"

static gboolean tag_index_is_valid(const tag_index_t *tag_db)
{
    return (tag_db != NULL && tag_db->tags_by_name != NULL && tag_db->tags_by_task != NULL && tag_db->tasks_by_tag != NULL);
}

static gboolean tag_is_valid(const tag_t *tag) { return (tag != NULL && string_is_valid(tag->name) == TRUE); }

static tag_t *tag_new(guint id, const gchar *name)
{
    tag_t *t = g_new0(tag_t, 1);
    t->id = id;
    t->name = g_strdup(name);
    return t;
}

static void tag_free(gpointer data)
{
    tag_t *t = (tag_t *)data;
    if (!t) {
        return;
    }

    g_free(t->name);
    g_free(t);
}

static tag_t *tag_get(tag_index_t *tag_db, const gchar *name)
{
    g_return_val_if_fail(tag_index_is_valid(tag_db), NULL);
    g_return_val_if_fail(string_is_valid(name), NULL);

    return g_hash_table_lookup(tag_db->tags_by_name, name);
}

static tag_t *tag_get_or_create(tag_index_t *tag_db, const gchar *name)
{
    g_return_val_if_fail(tag_index_is_valid(tag_db), NULL);
    g_return_val_if_fail(string_is_valid(name), NULL);

    tag_t *tag = tag_get(tag_db, name);
    if (!tag) {
        tag = tag_new(tag_db->next_tag_id++, name);
        if (!tag) {
            return NULL;
        }

        g_hash_table_insert(tag_db->tags_by_name, tag->name, tag);
    }
    return tag;
}

/**
 *************************************************************************************************
 * @brief Free the tag. Callers must first drop it from tasks_by_tag and tags_by_task.
 * @param ...
 * @retval g_hash_table_remove result
 *************************************************************************************************
 **/
static gboolean tag_delete(tag_index_t *tag_db, const gchar *name)
{
    g_return_val_if_fail(tag_index_is_valid(tag_db), FALSE);
    g_return_val_if_fail(string_is_valid(name), FALSE);

    return g_hash_table_remove(tag_db->tags_by_name, name);
}

/**
 *************************************************************************************************
 * @brief Add tag to the task and update tags array. Create tag if it does not exist.
 * @param ...
 * @retval TRUE if tag was successfully attached, FALSE otherwise
 *************************************************************************************************
 **/
gboolean Tag_AddToTask(tag_index_t *tag_db, task_t *task, const gchar *tag_name)
{
    g_return_val_if_fail(tag_index_is_valid(tag_db), FALSE);
    g_return_val_if_fail(Task_IsValid(task), FALSE);
    g_return_val_if_fail(string_is_valid(tag_name), FALSE);

    tag_t *tag = tag_get_or_create(tag_db, tag_name);
    if (!tag) {
        return FALSE;
    }

    gpointer task_id_ptr = GUINT_TO_POINTER(task->id);
    GHashTable *task_set = g_hash_table_lookup(tag_db->tasks_by_tag, tag);
    if (!task_set) {
        task_set = g_hash_table_new(g_direct_hash, g_direct_equal);
        g_hash_table_insert(tag_db->tasks_by_tag, tag, task_set);
    }
    g_hash_table_add(task_set, task_id_ptr);

    GHashTable *tag_set = g_hash_table_lookup(tag_db->tags_by_task, task_id_ptr);
    if (!tag_set) {
        tag_set = g_hash_table_new(g_direct_hash, g_direct_equal);
        g_hash_table_insert(tag_db->tags_by_task, task_id_ptr, tag_set);
    }

    if (g_hash_table_add(tag_set, tag) == TRUE) {  // ignore duplicates
        g_ptr_array_add(task->tags, tag->name);
    }

    return TRUE;
}

/**
 *************************************************************************************************
 * @brief Detaches tag from a task. Deletes tags with no bindings
 * @param ...
 * @retval TRUE if tag was removed or does not exist, FALSE otherwise
 *************************************************************************************************
 **/
gboolean Tag_RemoveFromTask(tag_index_t *tag_db, task_t *task, const gchar *tag_name)
{
    g_return_val_if_fail(tag_index_is_valid(tag_db), FALSE);
    g_return_val_if_fail(Task_IsValid(task), FALSE);
    g_return_val_if_fail(string_is_valid(tag_name), FALSE);

    tag_t *tag = tag_get(tag_db, tag_name);
    if (!tag) {
        return TRUE;  // nothing to remove
    }

    g_ptr_array_remove_fast(task->tags, tag->name);  // disrupts tag order (ok in our case)

    GHashTable *tag_set = g_hash_table_lookup(tag_db->tags_by_task, GUINT_TO_POINTER(task->id));
    if (tag_set) {
        g_hash_table_remove(tag_set, tag);
        if (g_hash_table_size(tag_set) == 0) {
            g_hash_table_remove(tag_db->tags_by_task, GUINT_TO_POINTER(task->id));
        }
    }

    GHashTable *task_set = g_hash_table_lookup(tag_db->tasks_by_tag, tag);
    if (task_set) {
        g_hash_table_remove(task_set, GUINT_TO_POINTER(task->id));
        if (g_hash_table_size(task_set) == 0) {
            g_hash_table_remove(tag_db->tasks_by_tag, tag);  // do not store detached tags
            tag_delete(tag_db, tag_name);
        }
    }

    return TRUE;
}

typedef struct
{
    task_t *task;
    tag_index_t *tag_db;
} task_to_tag_binding;

static void task_remove_tag_bindings(gpointer data, gpointer user_data)
{
    const task_to_tag_binding *binding = user_data;
    tag_index_t *tag_db = binding->tag_db;
    task_t *task = binding->task;
    tag_t *tag = tag_get(tag_db, (gchar *)data);
    if (!tag) {
        return;
    }

    GHashTable *task_set = g_hash_table_lookup(tag_db->tasks_by_tag, tag);
    if (task_set) {
        g_hash_table_remove(task_set, GUINT_TO_POINTER(task->id));
        if (g_hash_table_size(task_set) == 0) {
            g_hash_table_remove(tag_db->tasks_by_tag, tag);  // do not store detached tags
            tag_delete(tag_db, tag->name);
        }
    }
}

/**
 *************************************************************************************************
 * @brief Detaches all tags from a task. Deletes tags with no bindings
 * @param ...
 * @retval TRUE if tags were removed or do not exist, FALSE otherwise
 *************************************************************************************************
 **/
gboolean Tag_RemoveAllFromTask(tag_index_t *tag_db, task_t *task)
{
    g_return_val_if_fail(tag_index_is_valid(tag_db), FALSE);
    g_return_val_if_fail(Task_IsValid(task), FALSE);

    g_hash_table_remove(tag_db->tags_by_task, GUINT_TO_POINTER(task->id));

    task_to_tag_binding binding = {
        .task = task,
        .tag_db = tag_db,
    };
    g_ptr_array_foreach(task->tags, task_remove_tag_bindings, &binding);
    g_ptr_array_set_size(task->tags, 0);

    return TRUE;
}

/**
 *************************************************************************************************
 * @brief Return all task_id bind to tag_name as GPtrArray
 * @param ...
 * @retval Filled GPtrArray pointer, NULL otherwise
 * @note Function allows to automate memory management: g_autoptr(GPtrArray) ids = Tag_GetAllTasks(...)
 *************************************************************************************************
 **/
GPtrArray *Tag_GetAllTasks(tag_index_t *tag_db, const gchar *tag_name)
{
    g_return_val_if_fail(tag_index_is_valid(tag_db), NULL);
    g_return_val_if_fail(string_is_valid(tag_name), NULL);

    const tag_t *tag = g_hash_table_lookup(tag_db->tags_by_name, tag_name);
    if (!tag) {
        return NULL;
    }

    GHashTable *task_set = g_hash_table_lookup(tag_db->tasks_by_tag, tag);
    if (!task_set || g_hash_table_size(task_set) == 0) {
        return NULL;
    }

    return g_hash_table_get_values_as_ptr_array(task_set);
}

void Tag_IndexInit(tag_index_t *tag_db)
{
    if (!tag_db) {
        return;
    }

    tag_db->tags_by_name = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, tag_free);
    tag_db->tasks_by_tag = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, (GDestroyNotify)g_hash_table_destroy);
    tag_db->tags_by_task = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, (GDestroyNotify)g_hash_table_destroy);
}

void Tag_IndexDestroy(tag_index_t *tag_db)
{
    g_return_if_fail(tag_index_is_valid(tag_db));

    g_hash_table_destroy(tag_db->tags_by_name);
    g_hash_table_destroy(tag_db->tasks_by_tag);
    g_hash_table_destroy(tag_db->tags_by_task);
}
