#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25

void input_matrix(char **matr, int *n) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (scanf("%s", &matr[i][j]) != 1) {
                *n = -1;
                return;
            }
        }
    }
}

void output_matrix(char **matr, int *all_count) {
    clear();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
    for (int i = 0; i < WIDTH + 3; i++) {
        attron(COLOR_PAIR(1));
        printw("_");
        attroff(COLOR_PAIR(1));
    }
    printw("\n");
    for (int i = 0; i < HEIGHT; i++) {
        attron(COLOR_PAIR(1));
        printw("| ");
        attroff(COLOR_PAIR(1));
        for (int j = 0; j < WIDTH; j++) {
            attron(COLOR_PAIR(2));
            printw("%c", (matr[i][j] == '0') ? ' ' : matr[i][j]);
            attroff(COLOR_PAIR(2));
        }
        attron(COLOR_PAIR(1));
        printw(" |");
        attroff(COLOR_PAIR(1));
        if (i < HEIGHT - 1) printw("\n");
    }
    printw("\n");
    for (int i = 0; i < WIDTH + 3; i++) {
        attron(COLOR_PAIR(1));
        printw("¯");
        attroff(COLOR_PAIR(1));
    }
    printw("\n");
    printw("Press W (speed down), S (speed up), Q (exit).\n");
    printw("LIFE: %d", *all_count);
    refresh();
}

int live_neighboor_count(char **matr, int i, int j) {
    int count = 0;

    int left = (j == 0) ? WIDTH - 1 : j - 1;
    int right = (j == WIDTH - 1) ? 0 : j + 1;
    int up = (i == 0) ? HEIGHT - 1 : i - 1;
    int down = (i == HEIGHT - 1) ? 0 : i + 1;

    count += (matr[up][left] == '*') ? 1 : 0;
    count += (matr[up][j] == '*') ? 1 : 0;
    count += (matr[up][right] == '*') ? 1 : 0;
    count += (matr[i][right] == '*') ? 1 : 0;
    count += (matr[i][left] == '*') ? 1 : 0;
    count += (matr[down][left] == '*') ? 1 : 0;
    count += (matr[down][j] == '*') ? 1 : 0;
    count += (matr[down][right] == '*') ? 1 : 0;

    return count;
}

char dot_is_live(int neighboor_count, char state) {
    char live = '0';
    if (state == '0' && neighboor_count == 3)
        live = '*';
    else if (state == '*' && (neighboor_count == 2 || neighboor_count == 3))
        live = '*';
    else
        live = '0';
    return live;
}

void logic(char **matrix, int *all_count) {
    *all_count = 0;
    char temp[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int live_neigboors = live_neighboor_count(matrix, i, j);
            temp[i][j] = dot_is_live(live_neigboors, matrix[i][j]);
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            matrix[i][j] = temp[i][j];
            if (temp[i][j] == '*') {
                *all_count = *all_count + 1;
            }
        }
    }
}

void change_speed(int state, int *speed) {
    if (state == 1) *speed += 2000;
    if (state == 2) *speed -= 2000;
}

int main() {
    int running = 1;
    char *matr[HEIGHT];
    for (int i = 0; i < HEIGHT; i++) {
        matr[i] = (char *)malloc(WIDTH * sizeof(char));
    }
    int n = 1;
    input_matrix(matr, &n);
    if (n <= 0) {
        printf("n/a");
        return 0;
    }

    int speed = 80000;

    if (freopen("/dev/tty", "r", stdin)) initscr();
    noecho();
    nodelay(stdscr, TRUE);

    int c, all_count = 0;
    while (running) {
        c = getch();

        usleep(speed);

        output_matrix(matr, &all_count);
        logic(matr, &all_count);

        if (c == 'q' || c == 'Q') {
            running = 0;
        }

        if (c == 'W' || c == 'w') change_speed(1, &speed);

        if (c == 'S' || c == 's') change_speed(2, &speed);
        if (all_count == 0) {
            running = 0;
        }
    }
    endwin();
    printf("The game is end");
}
