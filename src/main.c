#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>

#define WIN_HEIGHT 1080
#define	WIN_WIDTH 1920
#define CELL_SIZE 5
#define CELLS_PER_ROW ((int)(WIN_WIDTH / CELL_SIZE))
#define CELLS_PER_COLUMN ((int)(WIN_HEIGHT / CELL_SIZE))

SDL_Window *window;
SDL_Texture *texture;
SDL_Renderer *renderer;

int pitch;
Uint8 *pixels;
_Bool choice;
int16_t value;

struct cell{
	int x;
	int y;
	_Bool alive;
};
//bruh
enum { amountOfCells = (int)((WIN_WIDTH * WIN_HEIGHT) / CELL_SIZE) };

struct cell cells[amountOfCells];
struct cell cellsNew[amountOfCells];
static void applyRules(struct cell *cells);
static void initGrid();
static void renderGrid(struct cell *cells);
static int countNeighbours(struct cell *cell, struct cell cells);
static void resetTexture();
static void DrawCell(int x, int y);
static void Quit();
static _Bool Init();
int main(int argc, const char **argv) {
	srand(time(NULL));
	pitch = WIN_WIDTH * 3;
	if(!Init()) return EXIT_FAILURE;
	texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_STREAMING,WIN_WIDTH ,WIN_HEIGHT);

	initGrid();

	while(true){
		SDL_RenderClear(renderer);
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			switch(event.type){

				case SDL_QUIT:
					Quit();
					return 1;

				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE){
						Quit();
						return 1; 
					}
			}
		}
		SDL_LockTexture(texture,NULL, (void**)&pixels, &pitch);

		resetTexture();
		applyRules(cells);
		renderGrid(cells);

		SDL_UnlockTexture(texture);	
		SDL_RenderCopy(renderer,texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
	return 0;
}
static void applyRules(struct cell *cells){
	memcpy(cellsNew,cells,sizeof(cellsNew));
	for(int i = 0;i<amountOfCells;++i){
		int neighbours = countNeighbours(cells,cells[i]);
		if(cells[i].alive && neighbours > 3) cellsNew[i].alive = false;
		else if(cells[i].alive && neighbours < 2) cellsNew[i].alive = false;
		else if(cells[i].alive && (neighbours == 3 || neighbours == 2)) cellsNew[i].alive = true;
		else if(cells[i].alive == false && neighbours == 3) cellsNew[i].alive = true;
	}
	memcpy(cells,cellsNew,sizeof(cellsNew));
}
static int countNeighbours(struct cell *cells,struct cell cell){	
	//printf("\n");
	int neighbours = 0;
	for(int i = 1;i<=3 ;i++){
		int offSetY = (i == 1) ? -1: ((i == 2) ? 0 : 1);
		for(int j = 1;j<=3;j++){
			int offSetX = (j == 1) ? -1: ((j == 2) ? 0 : 1);
			int cellIndex = ((cell.y * CELLS_PER_ROW) + cell.x) + (offSetY * CELLS_PER_ROW) + offSetX;
			if(!((cellIndex > amountOfCells) || (cellIndex <  0))){
			//printf("offSetX: %i, offSetY: %i, X: %i ,Y:%i, cellIndex: %i, alive: %d, neighbours: %i\n",offSetX ,offSetY,cell.x,cell.y,cellIndex,cell.alive,neighbours);
				if(cells[cellIndex].alive){
					if(!(offSetX == 0 && offSetY == 0)){
						if(!((cell.x % CELLS_PER_ROW) == 0 || (cell.y % CELLS_PER_COLUMN) == 0)){
							neighbours++;
						}
					}
				}
			}
		}
	}
	//printf("\n");
	return neighbours;
}
static void renderGrid(struct cell *cells){
	for(int j=0;j<CELLS_PER_COLUMN;j++){
		for(int i = 0;i<CELLS_PER_ROW;i++){
			int cellIndex = (int)((j * CELLS_PER_ROW + i));
			if(cells[cellIndex].alive){
					DrawCell(cells[cellIndex].x,cells[cellIndex].y);
			}
		}
	}
}
static void initGrid(){
	for(int i=0;i<amountOfCells;i++){
		cells[i].y = (int)(i / (int)(WIN_WIDTH / CELL_SIZE));
		cells[i].x = i - (cells[i].y * (int)(WIN_WIDTH / CELL_SIZE));
		cells[i].alive = ((bool)(rand() % 20)) ? false : true; 
		//cells[i].alive = ((bool)(rand() % 1)) ? true : true; 
	}
}
static void resetTexture(){
	for(int i=0;i<(WIN_WIDTH * WIN_HEIGHT) * 4 ;i++) *(pixels + i) = 0;
}
static void DrawCell(int x, int y){
	for(int localY = 0;localY<CELL_SIZE;localY++){
		for(int localX = 0;localX<CELL_SIZE;localX++){
			for(int i=0;i<3;i++) *(pixels + (((((y * CELL_SIZE) * WIN_WIDTH) + (x * CELL_SIZE)) + (localY * WIN_WIDTH + localX)) * 4) + i) = 255;
		}
	}
}

static void Quit(){
	printf("Quitting SDL...\n");
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

static _Bool Init() {
	printf("Initializing...\n");
	if(!(window = SDL_CreateWindow("Game of life | Erizo", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_FULLSCREEN))) {
	fprintf(stderr, "Failed to init SDL window: %s\n", SDL_GetError());
	return false;
	}
	printf("Window Initialized\n");

	if(!(renderer = SDL_CreateRenderer(window, -1 ,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))){
	fprintf(stderr, "Failed to init SDL renderer: %s\n", SDL_GetError());
	return false;
	}  
	printf("Renderer Initialized\n");

	return true;
}
