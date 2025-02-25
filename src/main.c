#include "database.h"
#include "defines.h"
#include "io-kmark.h"
#include "typedefs.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct global_t global;

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
  checkOrCreateConfigFolderLinux(&global);

  if (initializeDb(&global.db, global.dirs.db) != SQLITE_OK) {
    return false;
  }

  return true;
}

void shutdown() {
  free(global.dirs.config);
  free(global.dirs.db);
  printf("Goodbye\n");
}

void commandLineParser(int argc, char **argv) {
  int option;
  while ((option = getopt(argc, argv, "ali")) != -1) {
    switch (option) {
    case 'a': {
      struct bookmark_t bm;
      creatBookmark(global.db, &bm);
      shutdown();
      exit(EXIT_SUCCESS);
    }
    case 'l': {
      listAllBookmarks(global.db);
      shutdown();
      exit(EXIT_SUCCESS);
    }
    case 'i': {
      printf("Home dir: %s\n", global.dirs.home);
      printf("Config dir: %s\n", global.dirs.config);
      printf("Database location: %s\n", global.dirs.db);
      shutdown();
      exit(EXIT_SUCCESS);
    }
    }
  }
}

int main(int argc, char **argv) {
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
      creatBookmark(global.db, &bm);
      break;

    case BM_LIST:
      listAllBookmarks(global.db);
      break;

    case BM_SEARCH_BY_TAG:
      printf("Enter tag to search: ");
      getLineInput(tag, sizeof(tag));
      tag[strcspn(tag, "\n")] = 0;

      searchByTag(global.db, tag);
      break;

    case BM_DELETE:
      printf("Enter bookmark ID to delete: ");
      getIntInput(&id);
      getchar();

      deleteBookmark(global.db, id);
      break;

    case BM_EXIT:
      sqlite3_close(global.db);
      shutdown();
      exit(EXIT_SUCCESS);

    default:
      printf("Invalid choice. Please try again.\n");
    }
    printf(COL_RESET);
  }

  return 0;
}