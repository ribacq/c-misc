#include <stdlib.h>
#include <ncurses.h>

//JGOL: Conwai’s Game of Life in 70 lines of C-code

typedef enum{TOGGLE, NEXT, QUIT, LEFT, DOWN, UP, RIGHT} CTRLS_name;
const int CTRLS[7] = {'e', ' ', 'q', 't', 's', 'r', 'n'};

int main(int argc, char *argv[]){
	//Init curses mode
	initscr();

	//Colors
	if(!has_colors()){
		endwin();
		printf("Your terminal does not support color.\n");
		exit(1);
	}
	start_color();
	init_pair(1,	COLOR_RED,	COLOR_BLACK);
	init_pair(2,	COLOR_BLACK,	COLOR_RED);
	
	//Attributes and window
	noecho();
	WINDOW *main_win = newwin(LINES, COLS, 0, 0);
	nodelay(main_win, TRUE);

	//Board memory allocation
	int layer = 0;
	int i, j;
	int ***cells = (int ***) calloc(LINES, sizeof(int **));
	for(i = 0; i < LINES; i++){
		cells[i] = (int **) calloc(COLS, sizeof(int *));
		for(j = 0; j < COLS; j++)
			cells[i][j] = (int *) calloc(2, sizeof(int));
	}

	//Main loop
	int cur_y = LINES / 2;
	int cur_x = COLS / 2;
	int ch = CTRLS[NEXT];
	int neighs;
	int step = 0;
	while(ch != CTRLS[QUIT]){
		if(ch == CTRLS[TOGGLE]){
			//Set or unset current cell
			cells[cur_y][cur_x][layer] = !cells[cur_y][cur_x][layer];
			mvwchgat(main_win, cur_y, cur_x, 1, 0, 1 + cells[cur_y][cur_x][layer], NULL);
		}else if(ch == CTRLS[LEFT])	cur_x = (cur_x - 1 + COLS) % COLS; //Cursor motion
		else if(ch == CTRLS[DOWN])	cur_y = (cur_y + 1) % LINES;
		else if(ch == CTRLS[UP])	cur_y = (cur_y - 1 + LINES) % LINES;
		else if(ch == CTRLS[RIGHT])	cur_x = (cur_x + 1) % COLS;
		else if(ch == CTRLS[NEXT]){
			//Advance to next step
			mvwprintw(main_win, 0, 0, "Game of Life, step %d", step++);
			for(i = 0; i < LINES; i++){
				for(j = 0; j < COLS; j++){
					neighs = cells[(i - 1 + LINES) % LINES][(j - 1 + COLS) % COLS][layer] +
						cells[(i - 1 + LINES) % LINES][j][layer] +
						cells[(i - 1 + LINES) % LINES][(j + 1) % COLS][layer] +
						cells[i][(j - 1 + COLS) % COLS][layer] +
						cells[i][(j + 1) % COLS][layer] +
						cells[(i + 1) % LINES][(j - 1 + COLS) % COLS][layer] +
						cells[(i + 1) % LINES][j][layer] +
						cells[(i + 1) % LINES][(j + 1) % COLS][layer];
					cells[i][j][!layer] = (neighs <= 3) && (neighs >= (3 - cells[i][j][layer]));
					mvwchgat(main_win, i, j, 1, 0, 1 + cells[i][j][!layer], NULL);
				}
			}
			layer = !layer;
		}
		wmove(main_win, cur_y, cur_x);
		ch = wgetch(main_win);
		wrefresh(main_win);
	}

	//Board memory deallocation
	for(i = 0; i < LINES; i++){
		for(j = 0; j < COLS; j++)
			free(cells[i][j]);
		free(cells[i]);
	}
	free(cells);
	
	//End curses mode
	delwin(main_win);
	endwin();
	
	//End of program
	return EXIT_SUCCESS;
}

