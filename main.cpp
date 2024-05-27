#include <iostream>
#include <cstring>
#include <unistd.h>

#define ROWS 16
#define COLS 32

// Grids to hold the current and next state of the game
int front[ROWS][COLS] = {0};
int back[ROWS][COLS] = {0};

void print_glider() {
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            std::cout << (front[y][x] ? "#" : ".");
        }
        std::cout << "\n";
    }
}

// Handles toroidal array indexing (wrap-around behaviour)
int mod(int a, int b) {
    return (a % b + b) % b;
}

// Counts living neighbors of a cell
int count_neighbors(int cx, int cy) {
    int neighbors = 0;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            // Skips the cell itself (centered one)
            if (!(dx == 0 && dy == 0)) {
                int x = mod(cx + dx, COLS);
                int y = mod(cy + dy, ROWS);

                if (front[y][x]) neighbors += 1;
            }
        }
    }
    return neighbors;
}

// Computes the next gen
void next() {
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS;  ++x) {
            int neighbors = count_neighbors(x, y);
            back[y][x] = front[y][x] ? (neighbors == 2 || neighbors == 3) : neighbors == 3;
        }
    }
}

int main() {
    front[0][1] = 1;
    front[1][2] = 1;
    front[2][0] = 1;
    front[2][1] = 1;
    front[2][2] = 1;

    for(;;) {
        print_glider();
        next();
        std::memcpy(front, back , sizeof(front));
        std::cout << "\033[" << ROWS << "A";
        usleep(100000);
    }
    return 0;
}
