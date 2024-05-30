/**
 * @file maze.c
 * @author (YOUR NAME)
 * @brief Code for the maze game for COMP1921 Assignment 2
 * NOTE - You can remove or edit this file however you like - this is just a provided skeleton code
 * which may be useful to anyone who did not complete assignment 1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 // defines for max and min permitted dimensions
#define MAX_DIM 100
#define MIN_DIM 5

// defines for the required autograder exit codes
#define EXIT_SUCCESS 0
#define EXIT_ARG_ERROR 1
#define EXIT_FILE_ERROR 2
#define EXIT_MAZE_ERROR 3

typedef struct __Coord
{
    int x;
    int y;
} coord;

typedef struct __Maze
{
    char** map;
    int height;
    int width;
    coord start;
    coord end;
} maze;

int create_maze(maze* this, int height, int width)
{
    this->height = height;
    this->width = width;
    this->map = malloc(height * sizeof(char*));
    if (!this->map) return 1;
    for (int i = 0; i < height; i++)
    {
        this->map[i] = malloc(width * sizeof(char));
        if (!this->map[i]) return 1;
    }
    return 0;
}

void free_maze(maze* this)
{
    for (int i = 0; i < this->height; i++)
    {
        free(this->map[i]);
    }
    free(this->map);
}

int get_width(FILE* file)
{
    int width = 0;
    char ch;
    while ((ch = fgetc(file)) != '\n' && ch != EOF)
    {
        width++;
    }
    return (width >= MIN_DIM && width <= MAX_DIM) ? width : 0;
}

int get_height(FILE* file)
{
    int height = 0;
    char ch;
    rewind(file);
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n') height++;
    }
    return (height >= MIN_DIM && height <= MAX_DIM) ? height : 0;
}

int read_maze(maze* this, FILE* file)
{
    rewind(file);
    for (int i = 0; i < this->height; i++)
    {
        if (fgets(this->map[i], this->width + 2, file) == NULL)
        {
            return 1;
        }
        for (int j = 0; j < this->width; j++)
        {
            if (this->map[i][j] == 'S')
            {
                this->start.x = j;
                this->start.y = i;
            }
            if (this->map[i][j] == 'E')
            {
                this->end.x = j;
                this->end.y = i;
            }
        }
    }
    return 0;
}

void print_maze(maze* this, coord* player)
{
    printf("\n");
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            if (player->x == j && player->y == i)
            {
                printf("X");
            }
            else
            {
                printf("%c", this->map[i][j]);
            }
        }
        printf("\n");
    }
}

void move(maze* this, coord* player, char direction)
{
    coord new_pos = *player;
    switch (direction)
    {
    case 'W': case 'w': new_pos.y--; break;
    case 'A': case 'a': new_pos.x--; break;
    case 'S': case 's': new_pos.y++; break;
    case 'D': case 'd': new_pos.x++; break;
    }

    if (new_pos.x >= 0 && new_pos.x < this->width &&
        new_pos.y >= 0 && new_pos.y < this->height &&
        this->map[new_pos.y][new_pos.x] != '#')
    {
        *player = new_pos;
    }
}

int has_won(maze* this, coord* player)
{
    return (player->x == this->end.x && player->y == this->end.y);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <mazefile path>\n", argv[0]);
        return EXIT_ARG_ERROR;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file)
    {
        perror("Error opening file");
        return EXIT_FILE_ERROR;
    }

    maze this_maze;
    int width = get_width(file);
    int height = get_height(file);

    if (width == 0 || height == 0 || create_maze(&this_maze, height, width))
    {
        fclose(file);
        return EXIT_MAZE_ERROR;
    }

    if (read_maze(&this_maze, file))
    {
        fclose(file);
        free_maze(&this_maze);
        return EXIT_MAZE_ERROR;
    }
    fclose(file);

    coord player = this_maze.start;
    char command;
    while (1)
    {
        scanf(" %c", &command);
        if (command == 'Q' || command == 'q') break;
        if (command == 'M' || command == 'm')
        {
            print_maze(&this_maze, &player);
        }
        else
        {
            move(&this_maze, &player, command);
            if (has_won(&this_maze, &player))
            {
                printf("Congratulations! You've reached the exit!\n");
                free_maze(&this_maze);
                return EXIT_SUCCESS;
            }
        }
    }

    free_maze(&this_maze);
    return EXIT_SUCCESS;
}
