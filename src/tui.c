#include "tui.h"

struct window_t *window_newWindow(int x, int y, int len, int height) {
  struct window_t *window;
  window->contents = NULL;
  window->pos.x = x;
  window->pos.y = y;
  window->size.x = len;
  window->size.y = height;
  return window;
}

void window_renderWindow(struct window_t *window) {}

void window_killWindow(struct window_t *window) { free(window->contents); }
