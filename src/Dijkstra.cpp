#include "Dijkstra.h"
#include "bits/stdc++.h"
#include "SDL.h"
#include "unistd.h";

using namespace std;

Dijkstra::Dijkstra()
{
    memset(grid, 0, sizeof(grid));
    run();
    runDijkstra();
    printPath();
    while(true);
    // Place the grid cursor in the middle of the screen.
}

void Dijkstra::run(){
    SDL_Rect grid_cursor = {
        .x = (grid_width - 1) / 2 * grid_cell_size,
        .y = (grid_height - 1) / 2 * grid_cell_size,
        .w = grid_cell_size,
        .h = grid_cell_size,
    };

    SDL_Window *window;
    if (SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window,
                                    &renderer) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Create window and renderer: %s", SDL_GetError());
        return;
    }

    SDL_SetWindowTitle(window, "SDL Grid");

    SDL_bool quit = SDL_FALSE;
    SDL_bool mouse_active = SDL_FALSE;
    SDL_bool mouse_hover = SDL_FALSE;

    while (!quit) {
        SDL_Event event;
        bool b = 1;
        while (SDL_PollEvent(&event)) {
            if(!b) break;
            switch (event.type) {
                if(!b) break;
                case SDL_MOUSEBUTTONDOWN:
                    grid_cursor.x = (event.motion.x / grid_cell_size) * grid_cell_size;
                    grid_cursor.y = (event.motion.y / grid_cell_size) * grid_cell_size;
                    cout << grid_cursor.x/grid_cell_size << " " << grid_cursor.y/grid_cell_size << endl;
                    if(event.button.button == SDL_BUTTON_LEFT){
                        SDL_SetRenderDrawColor(renderer, grid_cursor_color.r,
                               grid_cursor_color.g, grid_cursor_color.b,
                               grid_cursor_color.a);
                        SDL_RenderFillRect(renderer, &grid_cursor);
                        SDL_RenderPresent(renderer);
                        grid[grid_cursor.x/grid_cell_size][grid_cursor.y/grid_cell_size] = 1;
                    }
                    else if(event.button.button ==SDL_BUTTON_RIGHT){
                        if(click==0){
                            SDL_SetRenderDrawColor(renderer, 204, 70, 37, 255);
                            startX = grid_cursor.x/grid_cell_size;
                            startY = grid_cursor.y/grid_cell_size;
                            SDL_RenderFillRect(renderer, &grid_cursor);
                            SDL_RenderPresent(renderer);
                        }
                        else if(click==1){
                            SDL_SetRenderDrawColor(renderer, 62, 204, 37, 255);
                            endX = grid_cursor.x/grid_cell_size;
                            endY = grid_cursor.y/grid_cell_size;
                            SDL_RenderFillRect(renderer, &grid_cursor);
                            SDL_RenderPresent(renderer);
                        }
                        click++;
                        if(click==2){
                            return;
                        }
                    }
                    if(!b) break;
                    break;
                case SDL_QUIT:
                    quit = SDL_TRUE;
                    break;
                }
        }

        // Draw grid background.
        SDL_SetRenderDrawColor(renderer, grid_background.r, grid_background.g,
                               grid_background.b, grid_background.a);

        // Draw grid lines.
        SDL_SetRenderDrawColor(renderer, grid_line_color.r, grid_line_color.g,
                               grid_line_color.b, grid_line_color.a);

        for (int x = 0; x < 1 + grid_width * grid_cell_size;
             x += grid_cell_size) {
            SDL_RenderDrawLine(renderer, x, 0, x, window_height);
        }

        for (int y = 0; y < 1 + grid_height * grid_cell_size;
             y += grid_cell_size) {
            SDL_RenderDrawLine(renderer, 0, y, window_width, y);
        }

        // Draw grid cursor.
        SDL_SetRenderDrawColor(renderer, grid_cursor_color.r,
                               grid_cursor_color.g, grid_cursor_color.b,
                               grid_cursor_color.a);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Dijkstra::valid(int x, int y){
    if(x>=0&&y>=0&&x<20&&y<20&&grid[x][y]==0){
        return true;
    }
    return false;
}

void Dijkstra::runDijkstra(){
    int x = startX;
    int y = startY;
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            dist[i][j] = 99999;
            prev[i][j] = {-1, -1};
        }
    }
    priority_queue<pair<int, pair<int, int>>> pq;
    pq.push({0, {x, y}});
    dist[x][y]=0;
    while(!pq.empty()){
        int curX = pq.top().second.first;
        int curY = pq.top().second.second;
        pq.pop();
        if(make_pair(curX, curY) != make_pair(startX, startY) && make_pair(curX, curY) != make_pair(endX, endY)){
            SDL_Rect grid_cursor = {
                .x = curX*grid_cell_size,
                .y = curY*grid_cell_size,
                .w = grid_cell_size,
                .h = grid_cell_size,
            };
            SDL_SetRenderDrawColor(renderer, 37, 204, 196, 255);
            SDL_RenderFillRect(renderer, &grid_cursor);
            SDL_RenderPresent(renderer);
        }
        if(curX==endX&&curY==endY){
            break;
        }
        for(auto z : moves){
            int nx = curX+z[0];
            int ny = curY+z[1];
            if(valid(nx, ny)&&dist[nx][ny]>dist[curX][curY]+1){
                dist[nx][ny] = dist[curX][curY]+1;
                prev[nx][ny] = make_pair(curX, curY);
                cout << nx << " " << ny << " " << curX << " "<< curY << endl;
                pq.push({-1*dist[nx][ny], {nx, ny}});
            }
        }
        usleep(10000);
    }
}

void Dijkstra::printPath(){
    if(dist[endX][endY]!=99999){
        pair<int, int> n = prev[endX][endY];
        int x = n.first;
        int y = n.second;
        while(make_pair(x, y) != make_pair(startX, startY)){
            cout << "!" << x << " " << y << endl;
            SDL_Rect grid_cursor = {
                .x = x*grid_cell_size,
                .y = y*grid_cell_size,
                .w = grid_cell_size,
                .h = grid_cell_size,
            };
            SDL_SetRenderDrawColor(renderer, 215, 252, 3, 255);
            SDL_RenderFillRect(renderer, &grid_cursor);
            SDL_RenderPresent(renderer);
            n = prev[x][y];
            x = n.first;
            y = n.second;
            usleep(10000);
        }
    }
}

Dijkstra::~Dijkstra()
{
    //dtor
}
