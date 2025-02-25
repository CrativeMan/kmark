#include "tui.h"
#include "defines.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

void tui_init(struct global_t *global) {
  initscr();
  raw();
  keypad(stdscr, TRUE);
  global->term.rows = LINES;
  global->term.cols = COLS;
}

void tui_render(struct window_t *windows[], u8 nWindows) {
  for (int i = 0; i < nWindows; i++) {
    window_renderWindow(windows[i]);
  }
}

void tui_shutdown(struct window_t *windows[], u8 nWindows) {
  for (int i = 0; i < nWindows; i++) {
    window_killWindow(windows[i]);
  }
  endwin();
}

struct window_t *window_newWindow(int x, int y, int len, int height) {
  struct window_t *window = malloc(sizeof(struct window_t));
  window->contents = malloc(sizeof(*(window->contents)));
  memset(window->contents, 0, sizeof(*(window->contents)));
  window->pos.x = x;
  window->pos.y = y;
  window->size.x = len;
  window->size.y = height;
  return window;
}

void window_changeContent(struct window_t *window, char *content) {
  free(window->contents);
  window->contents = malloc(strlen(content) + 1);
  strcpy(window->contents, content);
}

void window_appendContent(struct window_t *window, char *content) {
  const size_t len1 = strlen(window->contents);
  const size_t len2 = strlen(content);
  char *result = malloc(len1 + len2 + 1);
  memcpy(result, window->contents, len1);
  memcpy(result + len1, content, len2 + 1);
  free(window->contents);
  window->contents = result;
}

void window_renderWindow(struct window_t *window) {
  int startx = window->pos.x;
  int starty = window->pos.y;
  int endx = startx + window->size.x;
  int endy = starty + window->size.y;

  // Draw the box
  mvhline(starty, startx, 0, window->size.x); // Top
  mvhline(endy, startx, 0, window->size.x);   // Bottom
  mvvline(starty, startx, 0, window->size.y); // Left
  mvvline(starty, endx, 0, window->size.y);   // Right
  mvaddch(starty, startx, ACS_ULCORNER);      // Top-left corner
  mvaddch(starty, endx, ACS_URCORNER);        // Top-right corner
  mvaddch(endy, startx, ACS_LLCORNER);        // Bottom-left corner
  mvaddch(endy, endx, ACS_LRCORNER);          // Bottom-right corner

  // Draw the content in the middle of the box
  int contentx = startx + (window->size.x / 2) - (strlen(window->contents) / 2);
  int contenty = starty + (window->size.y / 2);
  mvprintw(contenty, contentx, "%s", window->contents);
}

void window_killWindow(struct window_t *window) {
  free(window->contents);
  free(window);
}
