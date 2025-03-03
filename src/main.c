#include "database.h"
#include "defines.h"
#include "io-kmark.h"
#include "tui.h"
#include "typedefs.h"
#include <getopt.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct global_t *global;

// ----------------------------------------------------
// TUI
// ----------------------------------------------------
void printMenu() {
  printf("\nBookmark Manager\n");
  printf("1. Add bookmark\n");
  printf("2. List all bookmarks\n");
  printf("3. Search by tag\n");
  printf("4. Delete bookmark\n");
  printf("5. Exit\n");
  printf("Enter your choice: ");
}

// ----------------------------------------------------
// Initilaization and Shutdown
// ----------------------------------------------------
bool init() {
  global = malloc(sizeof(struct global_t));
  ASSERT(global != NULL, "failed to alloc global");
  checkOrCreateConfigFolderLinux(global);

  if (initializeDb(&global->db, global->dir.db) != SQLITE_OK) {
    return false;
  }

  global->close = false;

  tui_init(global);

  return true;
}

void shutdown() {
  if (global->dir.config) {
    free(global->dir.config);
    global->dir.config = NULL;
  }
  if (global->dir.db) {
    free(global->dir.db);
    global->dir.db = NULL;
  }
  printf("Goodbye :)\n");
}

void commandLineParser(int argc, char **argv) {
  int option;
  while ((option = getopt(argc, argv, "ali")) != -1) {
    switch (option) {
    case 'a': {
      struct bookmark_t bm;
      creatBookmark(global->db, &bm);
      exit(EXIT_SUCCESS);
    }
    case 'l': {
      listAllBookmarks(global->db);
      exit(EXIT_SUCCESS);
    }
    case 'i': {
      printf("Home dir: %s\n", global->dir.home);
      printf("Config dir: %s\n", global->dir.config);
      printf("Database location: %s\n", global->dir.db);
      exit(EXIT_SUCCESS);
    }
    }
  }
}

int main(int argc, char **argv) {
  ASSERT(init(), "%s: failed to init kmark", argv[0]);
  atexit(shutdown);

  global->windows[0] =
      window_newWindow(4, 2, global->term.cols - 9, global->term.rows - 12);
  window_changeContent(global->windows[0], "Hello, World!");
  window_appendContent(global->windows[0], " Appended content");

  while (global->close == false) {
    tui_render(global->windows, 1);
    char ch = getch();
    if (ch == 'q')
      global->close = true;
  }

  tui_shutdown(global->windows, 1);
  exit(EXIT_SUCCESS);
}

int test(int argc, char **argv) {
  ASSERT(init(), "%s: failed to init kmark", argv[0]);
  atexit(shutdown);
  commandLineParser(argc, argv);

  int choice;
  while (1) {
    printMenu();
    getIntInput(&choice);
    getchar();

    struct bookmark_t bm;
    char tag[100];
    int id;

    printf(COL_CYAN);
    printf("\n");
    switch (choice) {
    case BM_ADD:
      creatBookmark(global->db, &bm);
      break;

    case BM_LIST:
      listAllBookmarks(global->db);
      break;

    case BM_SEARCH_BY_TAG:
      printf("Enter tag to search: ");
      getLineInput(tag, sizeof(tag));
      tag[strcspn(tag, "\n")] = 0;

      searchByTag(global->db, tag);
      break;

    case BM_DELETE:
      printf("Enter bookmark ID to delete: ");
      getIntInput(&id);
      getchar();

      deleteBookmark(global->db, id);
      break;

    case BM_EXIT:
      sqlite3_close(global->db);
      exit(EXIT_SUCCESS);

    default:
      printf("Invalid choice. Please try again.\n");
    }
    printf(COL_RESET);
  }

  exit(EXIT_SUCCESS);
}
