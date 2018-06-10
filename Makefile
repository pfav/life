ROOTPROJ = $(shell pwd)
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic $(shell ncurses5-config --cflags)
LDFLAGS = -L$(ROOTPROJ) $(shell ncurses5-config --libs)

life : app.c app.h liblife.a
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -llife

liblife.a : liblife.o
	ar crs $@ $^

liblife.o : life.c life.h
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf life *.o *.a
