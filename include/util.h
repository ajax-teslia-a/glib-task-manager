#pragma once
#include <glib.h>

static inline gboolean string_is_valid(const gchar *str) { return (str != NULL && *str != '\0'); }
