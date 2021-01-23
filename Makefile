
CFLAGS= -Wall -Wextra -pedantic -std=c11
LIBS= -lraylib -lm -lpthread -lX11 -lrt -ldl
flappy_bird: main.c
	gcc $(CFLAGS) main.c -o flappy_bird $(LIBS)
