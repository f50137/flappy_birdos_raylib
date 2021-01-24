#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MIN(X, Y) (X) > (Y) ? (Y) : (X)
#define MAX(X, Y) (X) < (Y) ? (Y) : (X)


#define BACKGROUND_COLOR (Color){51, 51, 51, 255}
#define RED_COLOR (Color){200, 50, 50, 255}
#define GREEN_COLOR (Color){50, 200, 50, 255}

#define FRAME_RATE 60
#define TICK_LENGTH 10

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define MAX_TUBES 3
#define TUBE_WIDTH 65
#define TUBE_SPACE 230

#define BIRD_SIZE 50
#define BIRD_OFFSET 100

#define GAME_SPEED 4
#define GRAVITY 1
#define JUMP_VEL 18
#define BIRD_MAX_VEL 18

typedef struct {
	int y1;
	int y2;
	int x;
} Tube;

typedef struct {
	int pos_y;
	int vel;
} Bird;

typedef struct {
	Bird bird;
	Tube tubes[MAX_TUBES];
	int tube_front;
	bool game_over;
	int score;
} Game;

int random_int(int a, int b) {
	int end   = MAX(a, b);
	int begin = MIN(a, b);

	return begin + (rand() % (end - begin));
}

void reset_tube(Tube* tube, int x_offset) {
	int y = random_int(WINDOW_HEIGHT / 2 - TUBE_SPACE, WINDOW_HEIGHT / 2 + TUBE_SPACE);

	tube->x = WINDOW_WIDTH + x_offset;
	tube->y2 = y + TUBE_SPACE / 2;
	tube->y1 = y - TUBE_SPACE / 2;
}


void reset_game(Game *game) {
	*game = (Game) {
		.bird = (Bird) {
			.pos_y = WINDOW_HEIGHT - BIRD_SIZE,
			.vel = 0
		},
		.tubes = {0},
		.tube_front = 0,
		.game_over = true,
		.score = 0
	};
	
	int offset = (WINDOW_WIDTH - TUBE_WIDTH * (MAX_TUBES - 1)) / MAX_TUBES + TUBE_WIDTH;

	for(int i = 0; i < MAX_TUBES; i++) {
		reset_tube(&game->tubes[i], i * offset);
	}
}

void physics_tick(Game *game) {
	for(int i = 0; i < MAX_TUBES; i++) {
		Tube *tube = &game->tubes[i];
		if(!game->game_over)
		tube->x -= GAME_SPEED;
		if(tube->x < -TUBE_WIDTH) {
			reset_tube(tube, 0);

			game->tube_front++;
			game->tube_front %= MAX_TUBES;

			game->score++;
		}
	}
	
	Bird *bird = &game->bird;
	
	bird->vel += GRAVITY;
	bird->vel = MIN(bird->vel, BIRD_MAX_VEL);

	if(IsKeyPressed(265) || IsKeyPressed(32)) {
		bird->vel = -JUMP_VEL;
		game->game_over = false;
	}

	bird->pos_y += bird->vel;

	if(WINDOW_HEIGHT - bird->pos_y < BIRD_SIZE) {
		bird->pos_y = WINDOW_HEIGHT - BIRD_SIZE;
		bird->vel = 0;		
	}

	Tube *front_tube = &game->tubes[game->tube_front];

	if(front_tube->x < BIRD_OFFSET + BIRD_SIZE && front_tube->x + TUBE_WIDTH > BIRD_OFFSET) {
		if(bird->pos_y < front_tube->y1 || bird->pos_y + BIRD_SIZE > front_tube->y2) {
			printf("Your score was: %d\n", game->score);
			reset_game(game);
		}
	}
}

void render_bird(Bird *bird) {
	DrawRectangle(BIRD_OFFSET, bird->pos_y, BIRD_SIZE, BIRD_SIZE, RED_COLOR);
}

void render_tube(Tube *tube) {
	DrawRectangle(tube->x, 0, TUBE_WIDTH, tube->y1, GREEN_COLOR);
	DrawRectangle(tube->x, tube->y2, TUBE_WIDTH, WINDOW_HEIGHT - tube->y2, GREEN_COLOR);	
}

void render_score(int score) {
	char score_string[23];

	sprintf(score_string, "%d", score);

	DrawText(score_string, 23, 23, 23, WHITE);
}

void render(Game *game) {
	BeginDrawing();
		ClearBackground(BACKGROUND_COLOR);

		for(int i = 0; i < MAX_TUBES; i++) {
			
			render_tube(&game->tubes[i]);
		}
		render_bird(&game->bird);
		render_score(game->score);
		//DrawFPS(23, 23);
	EndDrawing();
}



int main() {	
	Game game = {0};
	reset_game(&game);

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Flappy Bird");
	
	SetTargetFPS(FRAME_RATE);
	
	int delta_time = 0.f;
	while(!WindowShouldClose()) {
		delta_time += (int)(GetFrameTime() * 1000);
		while(delta_time > TICK_LENGTH) {
			delta_time -= TICK_LENGTH;
			physics_tick(&game);
		}

		render(&game);
	}

	return 0;
}
