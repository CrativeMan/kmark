#pragma once

#include "defines.h"
#include <stddef.h>

void getLineInput(char *buffer, size_t bufferSize);
void getIntInput(int *value);
int checkOrCreateConfigFolderLinux(struct global_t *global);