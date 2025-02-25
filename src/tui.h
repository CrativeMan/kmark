#pragma once
#include "typedefs.h"

/*
 * char *contents - the text to be displayed in the window's bounds
 * v2i *size - vector of ints for the size of the window
 * v2i *pos - vector for the position of the window in the terminal
 */
struct window_t {
  char *contents;
  v2i size;
  v2i pos;
};

struct window_t *window_newWindow(int x, int y, int len, int height);
void window_renderWindow(struct window_t *window);
void window_killWindow(struct window_t *window);
