#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "bits/stdc++.h"
#include "SDL.h"

using namespace std;

class Dijkstra
{
    public:
        Dijkstra();
        void runDijkstra();
        bool valid(int x, int y);
        void printPath();
        void run();
        virtual ~Dijkstra();

    protected:

    private:
        int grid[20][20];
        int startX;
        int startY;
        int endX;
        int endY;
        int moves[4][2] = {{-1,0}, {1, 0}, {0, 1}, {0, -1}};
        int dist[20][20];
        pair<int, int> prev[20][20];
        int grid_cell_size = 30;
        int grid_width = 20;
        int grid_height = 20;
        int window_width = (grid_width * grid_cell_size) + 1;
        int window_height = (grid_height * grid_cell_size) + 1;
        SDL_Color grid_background = {22, 22, 22, 255}; // Barely Black
        SDL_Color grid_line_color = {44, 44, 44, 255}; // Dark grey
        SDL_Color grid_cursor_ghost_color = {44, 44, 44, 255};
        SDL_Color grid_cursor_color = {255, 255, 255, 255}; // White
        int click = 0;
        SDL_Renderer *renderer;
};

#endif // DIJKSTRA_H
