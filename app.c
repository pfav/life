#include "app.h"

static inline void app_terminate(void)
{
  curs_set(1);
  endwin();
}

static inline void app_init(void)
{
  srand(time(NULL));
  initscr();
  noecho();
  cbreak();
  raw();

  nodelay(stdscr, true);
  curs_set(0);
  if(has_colors())
    start_color();

  keypad(stdscr, true);

  attron(A_BOLD);
  init_pair(0, COLOR_WHITE, COLOR_BLACK);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);

  mousemask(ALL_MOUSE_EVENTS, NULL);
}


int main(){
  world_t world;
  int width = 0, height = 0;
  int _delay = 100;

  app_init();
  atexit(app_terminate);

  memset(&world, 0x0, sizeof(world_t));
  dim(width, height);
  world_init(&world, width, height, true);

  bool in_pause = false;
  while(true){
    if (!in_pause){
      clear();
      attrset(COLOR_PAIR(0));
      box(stdscr, '*', '*');
      world_draw(&world);
      refresh();

      dim(width, height);
      world_update(&world, width, height);
    }

    int ch = getch();
    switch (ch){
    case KEY_MOUSE:
      {
        MEVENT event;

        if(getmouse(&event) == OK) {
           int y = event.y-2;
           int x = event.x-2;
           cell_t* cell = &world.cells[y][x];
           *cell = *cell == alive ? dead : alive;
           world_draw_at(x, y, *cell == alive ? CHLIFE : CHDEAD);
           refresh();
        }
      } break;
    case 'r' :
      {
        world_free(&world);
        world_init(&world, width, height, true);
      } break;
    case 'p': { in_pause = !in_pause; } break;
    case 'f': { world_alive(&world); } break;
    case 'c': { world_dead(&world); } break;
    case 'q': { goto done; } break;
    case 'j': { _delay -= 2; if (_delay < 0) _delay = 0; } break;
    case 'k': { _delay += 2; if (_delay < 0) _delay = 0; } break;
    }

    napms(in_pause ? 100 : _delay);
  }

done:

  world_free(&world);
  return EXIT_SUCCESS;
}
