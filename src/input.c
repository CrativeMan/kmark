#include "input.h"
#include <stdio.h>
#include <stdlib.h>

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