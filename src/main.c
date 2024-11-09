#include <stdio.h>
#include <SDL2/SDL.h>
#include "game_object.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

#define FRAMES 30
#define WINDOW_SIZE 800

int game_is_running; 

//Game objects have properties (x, y, size, vx, vy, speed)
game_object player;
game_object enemy;

void setup(){
    game_is_running=1;
    player.x = 40;
    player.y = 40;
    player.size = 40;
    player.vx=0;
    player.vy=0;
    player.speed=10;
}

void initialize_window(){
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("BEST GAME EVER",	//window title
                       0,				//initial x position
                       0,				//initial y position
                       WINDOW_SIZE,				//width (pixels)	
                       WINDOW_SIZE,				//height (pixels)
                         0);				//flags
    //RENDER SETUP
    renderer = SDL_CreateRenderer(window, -1, 0); 
}

void destroy_window(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit(); 
}

void handle_input(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game_is_running = 0;
        } 
        switch (event.type){
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_LEFT:
                        player.vx=-player.speed;
                        break;
                    case SDLK_RIGHT:
                        player.vx=player.speed;
                        break;
                    case SDLK_UP:
                        player.vy=-player.speed;
                        break;
                    case SDLK_DOWN:
                        player.vy=player.speed;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym){
                    case SDLK_LEFT:
                        if(player.vx<0){player.vx=0;}
                        break;
                    case SDLK_RIGHT:
                        if(player.vx>1){player.vx=0;}
                        break;
                    case SDLK_UP:
                        if(player.vy<0){player.vy=0;}
                        break;
                    case SDLK_DOWN:
                        if(player.vy>0){player.vy=0;}
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void update(){
    player.x=player.x+player.vx;
    player.y=player.y+player.vy;
}

void render(){
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect = {player.x,player.y,player.size,player.size};  
    SDL_RenderFillRect(renderer ,&rect);	
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]){
    game_is_running = 1;
    setup(); 
    initialize_window();

    //GAME LOOP
    while (game_is_running == 1) {
        handle_input();
        update();
        render();
        SDL_Delay(1000/FRAMES);
    }

    destroy_window(); 
    return 0;
}

