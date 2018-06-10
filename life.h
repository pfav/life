#ifndef __life_h__
#define __life_h__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>
#include <time.h>

typedef enum { dead = false, alive } state_t;
typedef int cell_t;

typedef struct {
  cell_t **cells;
  int w, h;
  uint8_t status;
} world_t;

#define UNINITIALIZED (0x01)
#define INITIALIZED   (0x02)

#define CHLIFE ('o')
#define CHDEAD (' ')

#define TEST(a, b) ((a&b)==b)

#define MIN_WIN_SIZE (4)

__attribute__((noreturn)) void world_error(int error, const char *fmt, ...);

extern void world_init(world_t* world, int width, int height, bool randInitialized);
extern void world_update(world_t* world, int width, int height);
extern void world_life(world_t* r, world_t *w, cell_t x, cell_t y);
extern void world_choise(world_t* world, cell_t state);
extern void world_dead(world_t* world);
extern void world_alive(world_t* world);
extern void world_draw_at(int x, int y, char ch);
extern void world_draw(world_t* world);
extern void world_free(world_t* world);


#define dim(h, w) do { \
    getmaxyx(stdscr, (w), (h)); \
    w -= 4; h -= 4; \
    if(w <= MIN_WIN_SIZE || h <= MIN_WIN_SIZE) exit(127); \
  } while(0)


#endif
