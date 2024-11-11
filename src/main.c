#include <stdio.h>
#include <SDL2/SDL.h>
#include "game_object.h"
#include <time.h>
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

#define FRAMES 60
#define WINDOW_SIZE 1000
#define MAP_SIZE 10000
#define FIREFLIES 300

int game_is_running; 
int frame;

//Game objects have properties (x, y, size, vx, vy, speed)
game_object player;
game_object firefly[FIREFLIES];

void setup(){
    game_is_running=1;
    player.x = MAP_SIZE/2;
    player.y = MAP_SIZE/2;
    player.size = 25;
    player.vx=0;
    player.vy=0;
    player.speed=10;
    player.health=10;
    srand(time(0));
    //DECLARE FIREFLIES
    for(int i=0; i<FIREFLIES;i++){
        int rx = rand(); 
        int ry = rand(); 
        firefly[i].x=rx%(MAP_SIZE-WINDOW_SIZE)+(WINDOW_SIZE/2);
        firefly[i].y=ry%(MAP_SIZE-WINDOW_SIZE)+(WINDOW_SIZE/2);
        firefly[i].vy=0;
        firefly[i].vx=0;
        firefly[i].size=10;
        firefly[i].health=1;
    }
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
    if(player.x+player.vx-(WINDOW_SIZE/2)<=0 || 
        player.y+player.vy-(WINDOW_SIZE/2)<=0 || 
        player.x+player.vx+(WINDOW_SIZE/2)>MAP_SIZE || 
        player.y+player.vy+(WINDOW_SIZE/2)>MAP_SIZE)
    {}else{
        player.x=player.x+player.vx;
        player.y=player.y+player.vy;
    }
    //MOVE SOME OF THE FIREFLIES
    if(frame%4==0){
        for(int i=0; i<FIREFLIES; i++){
            int rx = rand();
            int ry = rand();
            if(rx%3==0){
                firefly[i].x=(firefly[i].x+((ry%3)-1)*4);
                firefly[i].y=(firefly[i].y-((ry%3)-1)*4);
            }
        }    
    } 
}

void render(){
    int vx = player.x-(WINDOW_SIZE/2); 
    int vy = player.y-(WINDOW_SIZE/2);
    //BACKGROUND
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 10, 0, 25,255);

    //PLAYER 
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,/*R*/255, /*G*/255, /*B*/255, /*OPACITY*/255);
    SDL_Rect rect = {WINDOW_SIZE/2,WINDOW_SIZE/2,player.size,player.size};  
    SDL_RenderFillRect(renderer ,&rect);	

    //ENEMIES
    SDL_Rect rectangles[FIREFLIES];
    for(int i=0; i<FIREFLIES; i++){
        int rx = rand();
        int opacity=rx%255;
        int fx = firefly[i].x-vx;
        int fy = firefly[i].y-vy;
        //                               R    G  B   O 
        if(fx<=0 || fx>=vx+WINDOW_SIZE || fy<=0 || fy>=vy+WINDOW_SIZE){
        }else{
            SDL_SetRenderDrawColor(renderer,255, opacity, 0, opacity);
            rectangles[i].x=fx;
            rectangles[i].y=fy;
            rectangles[i].h=firefly[i].size;
            rectangles[i].w=firefly[i].size;
            SDL_RenderFillRect(renderer ,&rectangles[i]);	
        }
    }
    //BORDER
    SDL_SetRenderDrawColor(renderer,255,0,255,100); 
    
    //DRAW THE BORDERS
    int bordersize = 10;
    int thorizontalBorderX, thorizontalBorderY; // -
    int bhorizontalBorderX, bhorizontalBorderY; // -
    int lverticalBorderX, lverticalBorderY; // |
    int rverticalBorderX, rverticalBorderY;  // |
    
    //VERTICAL BORDER
    if(vx<WINDOW_SIZE/2){
        //LEFT VERTICAL BORDER 
        SDL_SetRenderDrawColor(renderer,255,255,255,127);
        lverticalBorderX=(WINDOW_SIZE/2-vx);
        lverticalBorderY=0;
        SDL_Rect leftBorder={lverticalBorderX, lverticalBorderY, bordersize, WINDOW_SIZE};
        SDL_RenderFillRect(renderer,&leftBorder);
    }

    /* RIGHT VERTICAL BORDER 
    if(vx+WINDOW_SIZE>MAP_SIZE-(WINDOW_SIZE)/2){
        rverticalBorderX=MAP_SIZE-(WINDOW_SIZE/2);
        rverticalBorderY=0;
        SDL_Rect rightBorder={rverticalBorderX, rverticalBorderY, bordersize, WINDOW_SIZE};
        SDL_RenderFillRect(renderer,&rightBorder);
    }
    */

    //Present To The Screen
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]){
    game_is_running = 1;
    frame=1;
    setup(); 
    initialize_window();

    //GAME LOOP
    while (game_is_running == 1) {
        if(frame>200){
            frame=1;
        }
        frame++;
        handle_input();
        update();
        render();
        SDL_Delay(1000/FRAMES);
    }

    destroy_window(); 
    return 0;
}

