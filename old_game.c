#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define WIDTH 80
#define HEIGHT 25

void init(int **matrix);
void logic(int **matrix);
void out(int **matrix);
int live_count(int **matrix, int i, int j);

int main()
{
    int running = 1;
    int **matrix = (int **)malloc(HEIGHT * sizeof(int *));
    for (int i = 0; i < HEIGHT; i++)
    {
        matrix[i] = (int *)malloc(WIDTH * sizeof(int));
    }
    init(matrix);
    out(matrix);
    usleep(100000);
    system("clear");

    while (running)
    {
        //int c;
        logic(matrix);
        out(matrix);
        usleep(100000);
        system("clear");
        // c = getchar();
        // if (c == 'q' || c == 'Q') {
        //     running = 0;
        // }
    }
    return 0;
}

void init(int **matrix)
{
    FILE *file_ptr;
    file_ptr = fopen("game_of_life.txt", "r");
    int a[HEIGHT], b[WIDTH];
    int i = 0, size_a = 0;
    while (fscanf(file_ptr, "%d%d", &a[i], &b[i]) != EOF)
    {
        printf("%d %d", a[i], b[i]);
        i++;
        size_a++;
    }
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            matrix[i][j] = ' ';
            // if ((i == 10 && j == 10) || (i == 11 && j == 10) || (i == 11 && j == 11)
            // || (i == 12 && j == 10) || (i == 12 && j == 13))
            for (int x = 0; x < size_a; x++)
            {
                if (a[x] == i && b[x] == j)
                {
                    matrix[i][j] = '*';
                }
            }
        }
    }
}

void logic(int **matrix)
{
    int **copy_matrix = (int **)malloc(HEIGHT * sizeof(int *));
    for (int i = 0; i < HEIGHT; i++)
    {
        copy_matrix[i] = (int *)malloc(WIDTH * sizeof(int));
    }
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            int count = live_count(matrix, i, j);

            if (matrix[i][j] == '*' && (count < 2 || count > 3))
            {
                copy_matrix[i][j] = ' ';
            }
            else if (matrix[i][j] == ' ' && count == 3)
            {
                copy_matrix[i][j] = '*';
            }
            else
            {
                copy_matrix[i][j] = matrix[i][j];
            }
        }
    }
    for (int i = 0; i < HEIGHT; i ++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            matrix[i][j] = copy_matrix[i][j];
        }
    }
}

void out(int **matrix)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            printf("%c", matrix[i][j]);
        }
        printf("\n");
    }
}

int live_count(int **matrix, int i, int j)
{
    int count = 0;
    int left = (j == 0) ? WIDTH-1 : j-1;
    int right = (j == WIDTH-1) ? 0 : j+1;
    int up = (i == 0) ? HEIGHT-1 : i-1;
    int down = (i == HEIGHT-1) ? 0 : i+1;
    count += (matrix[up][left] =='*') ? 1 : 0;
    count += (matrix[up][j] == '*') ? 1 : 0;
    count += (matrix[up][right] == '*') ? 1 : 0;
    count += (matrix[i][right] == '*') ? 1 : 0;
    count += (matrix[i][left] == '*') ? 1 : 0;
    count += (matrix[down][left] == '*') ? 1 : 0;
    count += (matrix[down][j] == '*') ? 1 : 0;
    count += (matrix[down][right] == '*') ? 1 : 0;
    return count;
}