#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BACKGROUND_COLOR (Color){51, 51, 51, 255}
#define RED_COLOR (Color){200, 50, 50, 255}
#define GREEN_COLOR (Color){50, 200, 50, 255}

#define FRAME_RATE 60
#define TICK_LENGTH 0.01f

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define MAX_TUBES 3
#define TUBE_WIDTH 65
#define TUBE_SPACE 160

#define BIRD_SIZE 50
#define BIRD_OFFSET 50

#define GAME_SPEED 2

typedef struct {
	int y1;
	int y2;
	int x;
} Tube;

typedef struct {
	int pos_y;
	Tube tubes[MAX_TUBES];
	int tube_front;
} Game;

int random_int(int a, int b) {
	int end = a > b ? a : b;
	int begin = a > b ? b : a;

	return begin + (rand() % (end - begin));

}

void reset_tube(Tube* tube, int x_offset) {
	int y = random_int(WINDOW_HEIGHT / 2 - TUBE_SPACE, WINDOW_HEIGHT / 2 + TUBE_SPACE);

	tube->x = WINDOW_WIDTH + x_offset;
	tube->y2 = y + TUBE_SPACE / 2;
	tube->y1 = y - TUBE_SPACE / 2;
}

void move_tube(Tube *tube) {
}

void physics_tick(Game *game) {
	for(int i = 0; i < MAX_TUBES; i++) {
		Tube *tube = &game->tubes[i];
		tube->x -= GAME_SPEED;
		if(tube->x < -TUBE_WIDTH) {
			reset_tube(tube, 0);
			game->tube_front++;
			game->tube_front %= MAX_TUBES;
		}
	}	
}

void render_tube(Tube *tube) {
	DrawRectangle(tube->x, 0, TUBE_WIDTH, tube->y1, GREEN_COLOR);
	DrawRectangle(tube->x, tube->y2, TUBE_WIDTH, WINDOW_HEIGHT - tube->y2, GREEN_COLOR);	
}

void render(Game *game) {
	BeginDrawing();
		ClearBackground(BACKGROUND_COLOR);

		for(int i = 0; i < MAX_TUBES; i++) {
			
			render_tube(&game->tubes[i]);
		}
	EndDrawing();
}



void reset_game(Game *game) {
	*game = (Game) {
		.pos_y = WINDOW_HEIGHT - BIRD_SIZE,
		.tubes = {0},
		.tube_front = 0
	};
	
	int offset = WINDOW_WIDTH / MAX_TUBES + TUBE_WIDTH / 2;

	for(int i = 0; i < MAX_TUBES; i++) {
		reset_tube(&game->tubes[i], i * offset);
	}
}


int main() {
	
	Game game = {0};
	reset_game(&game);

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Flappy Bird");
	
	SetTargetFPS(FRAME_RATE);
	
	float delta_time = 0.f;
	while(!WindowShouldClose()) {
		delta_time += GetFrameTime();
		while(delta_time > TICK_LENGTH) {
			delta_time -= TICK_LENGTH;
			physics_tick(&game);
		}

		render(&game);
	}

	return 0;
}
