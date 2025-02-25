#pragma once

#include "defines.h"
#include <sqlite3.h>

int initializeDb(sqlite3 **db, const char *db_file);

void creatBookmark(sqlite3 *db, struct bookmark_t *bm);
int listAllBookmarks(sqlite3 *db);
int searchByTag(sqlite3 *db, const char *tag);
int deleteBookmark(sqlite3 *db, int id);