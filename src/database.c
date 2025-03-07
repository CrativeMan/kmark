#include "database.h"
#include "io-kmark.h"
#include <stdio.h>
#include <string.h>

/* ----------------------------------------------------
  Database interaction
 ----------------------------------------------------*/

int initializeDb(sqlite3 **db, const char *db_file) {
  int rc = sqlite3_open(db_file, db);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", db_file);
    sqlite3_close(*db);
    return rc;
  }

  const char *sql_create_table_code = "CREATE TABLE IF NOT EXISTS bookmarks ("
                                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                      "title TEXT NOT NULL,"
                                      "url TEXT NOT NULL UNIQUE,"
                                      "tags TEXT,"
                                      "description TEXT,"
                                      "date_added TEXT NOT NULL);";

  char *err_msg = NULL;
  rc = sqlite3_exec(*db, sql_create_table_code, 0, 0, &err_msg);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(*db);
    return rc;
  }

  printf("Database initialized successfully\n");
  return SQLITE_OK;
}

int addBookmark(sqlite3 *db, struct bookmark_t bookmark) {
  sqlite3_stmt *stmt;
  const char *sql =
      "INSERT INTO bookmarks (title, url, tags, description, date_added) "
      "VALUES (?, ?, ?, ?, datetime('now'));";

  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Prepare failed: %s\n", sqlite3_errmsg(db));
    return rc;
  }

  sqlite3_bind_text(stmt, 1, bookmark.title, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, bookmark.url, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, bookmark.tags, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, bookmark.description, -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
  } else {
    printf("Bookmark added successfully!\n");
  }

  sqlite3_finalize(stmt);
  return rc;
}

int callback(void *data, int argc, char **argv, char **col_name) {
  (void)data;
  printf("Found bookmark:\n");

  for (int i = 0; i < argc; i++) {
    printf("  %s: %s\n", col_name[i], argv[i] ? argv[i] : "NULL");
  }

  printf("\n");
  return 0;
}

int listAllBookmarks(sqlite3 *db) {
  char *err_msg = NULL;
  const char *sql = "SELECT * FROM bookmarks ORDER BY date_added DESC;";

  int rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
  }

  return rc;
}

int searchByTag(sqlite3 *db, const char *tag) {
  char *err_msg = NULL;
  char sql[1000];

  snprintf(sql, sizeof(sql),
           "SELECT * FROM bookmarks WHERE tags LIKE '%%%s%%';", tag);

  int rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
  }

  return rc;
}

int deleteBookmark(sqlite3 *db, int id) {
  sqlite3_stmt *stmt;
  const char *sql = "DELETE FROM bookmarks WHERE id = ?;";

  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Prepare failed: %s\n", sqlite3_errmsg(db));
    return rc;
  }

  sqlite3_bind_int(stmt, 1, id);

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
  } else {
    printf("Bookmark deleted successfully!\n");
  }

  sqlite3_finalize(stmt);
  return rc;
}

void creatBookmark(sqlite3 *db, struct bookmark_t *bm) {
  printf("Enter title: ");
  getLineInput(bm->title, sizeof(bm->title));
  bm->title[strcspn(bm->title, "\n")] = 0;

  printf("Enter URL: ");
  getLineInput(bm->url, sizeof(bm->url));
  bm->url[strcspn(bm->url, "\n")] = 0;

  printf("Enter tags (comma separated): ");
  getLineInput(bm->tags, sizeof(bm->tags));
  bm->tags[strcspn(bm->tags, "\n")] = 0;

  printf("Enter description: ");
  getLineInput(bm->description, sizeof(bm->description));
  bm->description[strcspn(bm->description, "\n")] = 0;

  addBookmark(db, *bm);
}
