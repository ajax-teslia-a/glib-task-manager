#include "task_db.h"
#include "util.h"

static gboolean task_db_is_valid(const task_db_t *db) { return (db != NULL && db->tasks != NULL && db->task_by_id != NULL); }

static gint compare_tasks(gconstpointer a, gconstpointer b, gpointer user_data)
{
    const task_t *task_a = (const task_t *)a;
    const task_t *task_b = (const task_t *)b;  // new task to insert

    // treat a new task with the same prio as a bigger one to keep tasks sorted in insertion order
    if (task_a->priority <= task_b->priority) {
        return -1;
    } else {
        return 1;
    }
}

/**
 *************************************************************************************************
 * @brief Add task to the tasks db, bind id for quick access
 * @param ...
 * @retval Valid task_t pointer to the created task, NULL otherwise
 *************************************************************************************************
 **/
const task_t *TaskDB_Add(task_db_t *db, gint priority, const gchar *title)
{
    g_return_val_if_fail(task_db_is_valid(db), NULL);
    g_return_val_if_fail(string_is_valid(title), NULL);

    task_t *task = Task_New(db->next_task_id, priority, title);
    GSequenceIter *it = g_sequence_insert_sorted(db->tasks, task, compare_tasks, NULL);
    g_hash_table_insert(db->task_by_id, GUINT_TO_POINTER(db->next_task_id), it);
    db->next_task_id++;
    return task;
}

gboolean TaskDB_Remove(task_db_t *db, guint id)
{
    g_return_val_if_fail(task_db_is_valid(db), FALSE);

    GSequenceIter *it = g_hash_table_lookup(db->task_by_id, GUINT_TO_POINTER(id));
    if (!it) {
        return FALSE;
    }

    Tag_RemoveAllFromTask(&db->tags, g_sequence_get(it));
    g_hash_table_remove(db->task_by_id, GUINT_TO_POINTER(id));
    g_sequence_remove(it);
    return TRUE;
}

const task_t *TaskDB_GetById(task_db_t *db, guint id)
{
    g_return_val_if_fail(task_db_is_valid(db), NULL);

    GSequenceIter *it = g_hash_table_lookup(db->task_by_id, GUINT_TO_POINTER(id));
    if (!it) {
        return NULL;
    }
    return (task_t *)g_sequence_get(it);
}

task_db_t *TaskDB_Create(void)
{
    task_db_t *tdb = g_new0(task_db_t, 1);
    tdb->tasks = g_sequence_new(Task_Free);
    tdb->task_by_id = g_hash_table_new(g_direct_hash, g_direct_equal);
    Tag_IndexInit(&tdb->tags);

    return tdb;
}

void TaskDB_Delete(task_db_t *db)
{
    g_return_if_fail(task_db_is_valid(db));

    Tag_IndexDestroy(&db->tags);
    g_hash_table_destroy(db->task_by_id);
    g_sequence_free(db->tasks);
    g_free(db);
}
