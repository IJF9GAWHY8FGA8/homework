#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WALL '#'
#define PATH ' '
#define START 'S'
#define END 'E'

typedef struct {
    int x, y;
} Point;

void shuffle(Point* array, int n) {
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            Point t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

void generateMaze(char** maze, int width, int height) {
    Point directions[] = { {0, 2}, {2, 0}, {0, -2}, {-2, 0} };
    Point stack[width * height];
    int stackSize = 0;

    // Initialize maze with walls
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            maze[y][x] = WALL;
        }
    }

    // Start position
    int startX = 1, startY = 1;
    maze[startY][startX] = START;
    stack[stackSize++] = (Point){ startX, startY };

    srand(time(NULL));

    while (stackSize > 0) {
        Point current = stack[stackSize - 1];
        shuffle(directions, 4);
        int found = 0;

        for (int i = 0; i < 4; i++) {
            int nx = current.x + directions[i].x;
            int ny = current.y + directions[i].y;
            if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && maze[ny][nx] == WALL) {
                maze[ny][nx] = PATH;
                maze[current.y + directions[i].y / 2][current.x + directions[i].x / 2] = PATH;
                stack[stackSize++] = (Point){ nx, ny };
                found = 1;
                break;
            }
        }
        if (!found) {
            stackSize--;
        }
    }

    maze[height - 2][width - 2] = END;
}

void saveMaze(const char* filename, char** maze, int width, int height) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fputc(maze[y][x], file);
        }
        fputc('\n', file);
    }

    fclose(file);
}

char** allocateMaze(int width, int height) {
    char** maze = malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        maze[i] = malloc(width * sizeof(char));
    }
    return maze;
}

void freeMaze(char** maze, int height) {
    for (int i = 0; i < height; i++) {
        free(maze[i]);
    }
    free(maze);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <filename> <width> <height>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* filename = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);

    if (width < 3 || height < 3 || width % 2 == 0 || height % 2 == 0) {
        fprintf(stderr, "Width and height must be odd numbers greater than or equal to 3.\n");
        return EXIT_FAILURE;
    }

    char** maze = allocateMaze(width, height);
    generateMaze(maze, width, height);
    saveMaze(filename, maze, width, height);
    freeMaze(maze, height);

    return EXIT_SUCCESS;
}
