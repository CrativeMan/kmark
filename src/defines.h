#pragma once

#include "typedefs.h"
#include <sqlite3.h>
#include <stdbool.h>

typedef enum {
  BM_NO_TAG = 0,
  BM_WEBSITE,
  BM_BLOG,
  BM_VIDEO,
} bookMarkTag;

typedef enum {
  BM_ADD = 1,
  BM_LIST,
  BM_SEARCH_BY_TAG,
  BM_DELETE,
  BM_EXIT
} choice;

struct date_t {
  u8 D;
  u8 M;
  i16 Y;
};

struct bookmark_t {
  char title[100];
  char url[500];
  char description[200];
  char tags[200];
  struct date_t date;
  i8 nTags;
};

struct dir {
  const char *home;
  char *config;
  char *db;
};

struct term {
  u16 cols;
  u16 rows;
};

struct global_t {
  sqlite3 *db;
  struct window_t *windows[1];
  struct dir dir;
  struct term term;
  bool close;
};
