#include "life.h"

void world_error(int error, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  sleep(1);
  va_end(args);
  exit(error);
}

void world_init(world_t* world, int width, int height, bool randInit){
  if (!world) world_error(127, "world_init: world doesn't exists\n");
  if (TEST(world->status, INITIALIZED)) world_error(127, "world-init: world already initialized\n");

  world->cells  = (cell_t **)malloc(height * sizeof(cell_t *));
  if (!world->cells) world_error(127, "world_init: unable to allocate world\n");

  for(int y = 0; y < height; y++){
    cell_t** row = &world->cells[y];
    *row = (cell_t *)malloc(width * sizeof(cell_t));
    if (!*row) world_error(127, "world_init: unable to allocate row[%d]\n", y);

    for(int x = 0; x < width; x++){
      (*row)[x] = randInit > 0 ? rand() % 2 : 0;
    }
  }

  world->w = width;
  world->h = height;
  world->status |= INITIALIZED;

  // glider
  //(*world)[0][2] = alive;
  //(*world)[1][2] = alive;
  //(*world)[2][2] = alive;
  //(*world)[2][1] = alive;
  //(*world)[1][0] = alive;
}


void world_update(world_t* world, int width, int height)
{
  if (!world) world_error(127, "world_update: wrong arguments value\n");
  if (TEST(world->status, UNINITIALIZED)) world_error(127, "world_update: world not initialized\n");

  world_t newWorld;
  world_init(&newWorld, width, height, false);

  for(int y = 0; y < height; y++){
    for(int x = 0; x < width; x++){
      if (y < world->h && x < world->w){
        world_life(world, &newWorld, x, y);
      } else {
        cell_t c = rand() % 2;
        newWorld.cells[y][x] = c;
        world_draw_at(x, y , c ? CHLIFE : CHDEAD);
      }
    }
  }

  world_free(world);
  *world = newWorld;
}

void world_life(world_t *r, world_t *w, cell_t x, cell_t y)
{
  if(!r || !w) world_error(127, "world_life: wrong arguments value\n");
  if (TEST(r->status, UNINITIALIZED)) world_error(127, "world_life: world reader not initialized\n");
  if (TEST(w->status, UNINITIALIZED)) world_error(127, "world_life: world writer not initialized\n");

  // neighbours
  // ----------
  // x-1,y-1 | x,y-1 | x+1, y-1
  // x-1,y   | x,y   | x+1, y
  // x-1,y+1 | x,y+1 | x+1, y+1
#define neighbour(x, y) (((x) < 0) || ((y) < 0) || ((x) >= r->w) || ((y) >= r->h) ? 0 : (r->cells[y][x]) )

  cell_t count =
      neighbour(x-1, y-1) +  neighbour(x, y-1) +  neighbour(x+1, y-1)
    + neighbour(x-1, y  ) +  /*current(x, y)*/    neighbour(x+1,    y)
    + neighbour(x-1, y+1) +  neighbour(x, y+1) +  neighbour(x+1, y+1);

  /*
   * 1. Any live cell with fewer than two live neighbours dies (referred to as underpopulation or exposure[1]).
   * 2. Any live cell with more than three live neighbours dies (referred to as overpopulation or overcrowding).
   * 3. Any live cell with two or three live neighbours lives, unchanged, to the next generation.
   * 4. Any dead cell with exactly three live neighbours will come to life.
   */

    if (count < 2 || count > 3){
      w->cells[y][x] = dead;
    } else if(count == 3){
      w->cells[y][x] = alive;
    } else {
      w->cells[y][x] = r->cells[y][x];
    }

}

void world_choise(world_t *world, cell_t state)
{
  if (!world) world_error(127, "world_choise: wrong arguments value\n");
  if (TEST(world->status, UNINITIALIZED)) world_error(127, "world_choise: world not initialized\n");

  for(int y = 0; y < world->h; y++){
    for(int x = 0; x < world->w; x++){
      world->cells[y][x] = state;
    }
  }
}

void world_alive(world_t *world)
{
  world_choise(world, alive);
}

void world_dead(world_t *world)
{
  world_choise(world, dead);
}

void world_draw_at(int x, int y, char ch)
{
  mvaddch(y+2, x+2, ch);
}

void world_draw(world_t* world)
{
  if (!world) world_error(127, "world_draw: wrong argument value\n");
  if (TEST(world->status, UNINITIALIZED)) world_error(127, "world_draw: world not initialized\n");

  for(int y = 0; y < world->h; y++){
    for(int x = 0; x < world->w; x++){
      // attrset(COLOR_PAIR(rand() % 6));
      world_draw_at(x, y, world->cells[y][x] == alive ? CHLIFE : CHDEAD);
    }
  }
}

void world_free(world_t* world)
{
  if (!world) world_error(127, "world_free: wrong argument value\n");
  if (TEST(world->status, UNINITIALIZED)) world_error(127, "world_free: world not initialized\n");

  for(int y = 0; y < world->h; y++){
    free(world->cells[y]);
  }
  free(world->cells);

  world->status &= ~(INITIALIZED);
}
