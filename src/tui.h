#pragma once

#include "defines.h"
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

void tui_init(struct global_t *global);
void tui_render(struct window_t *windows[], u8 nWindows);
void tui_shutdown(struct window_t *windows[], u8 nWindows);

struct window_t *window_newWindow(int x, int y, int len, int height);
void window_changeContent(struct window_t *window, char *contents);
void window_appendContent(struct window_t *window, char *content);
void window_renderWindow(struct window_t *window);
void window_killWindow(struct window_t *window);