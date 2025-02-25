#include "io-kmark.h"
#include "defines.h"
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*
  returns - char* RESULT HAS TO BE FREE'D
*/
char *concat(const char *s1, const char *s2) {
  const size_t len1 = strlen(s1);
  const size_t len2 = strlen(s2);
  char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
  // in real code you would check for errors in malloc here
  memcpy(result, s1, len1);
  memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
  return result;
}

void getLineInput(char *buffer, size_t bufferSize) {
  if (buffer == NULL || bufferSize == 0) {
    fprintf(stderr, "Invalid buffer of buffer size found.\n");
    exit(EXIT_FAILURE);
  }

  char *rc = fgets(buffer, bufferSize, stdin);
  if (rc == NULL) {
    fprintf(stderr, "Error reading line input.\n");
    exit(EXIT_FAILURE);
  }
}

void getIntInput(int *value) {
  if (value == NULL) {
    fprintf(stderr, "Invalid pointer for integer input.\n");
    exit(EXIT_FAILURE);
  }

  int rc = scanf("%d", value);
  if (rc != 1) {
    fprintf(stderr, "Error reading integer input.\n");
    exit(EXIT_FAILURE);
  }
}

int checkOrCreateConfigFolderLinux(struct global_t *global) {
  struct passwd *pw = getpwuid(getuid());
  const char *homedir = pw->pw_dir;
  global->dir.home = homedir;
  char *configdir = concat(homedir, "/.config/kmark");
  global->dir.config = malloc(strlen(configdir) + 1);
  strcpy(global->dir.config, configdir);

  struct stat stats;
  stat(configdir, &stats);

  int rv;
  if (S_ISDIR(stats.st_mode))
    rv = 1;
  else {
    // create dir if not exist
    rv = 0;
    mkdir(configdir, 0700);
  }

  char *location = concat(global->dir.config, "/bookmarks.db");
  global->dir.db = malloc(strlen(location) + 1);
  strcpy(global->dir.db, location);

  free(configdir);
  free(location);
  return rv;
}