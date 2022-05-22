/*
 Created by Taj Telesford on 5/22/22.
 
 This is going to be my implementation on using the linked list for snake game.
 I'm a fresh computer science major attending Texas State University and I'm just getting
 into data structures and algorithms. I hope whoever is looking at my code understands
 my though process. The only tutorials I will be using to actually build this project
 are videos on what the functionality of SLD2 can provide me.
 
 Hope you have fun playing my game too :)
 */


#include "SDL2/SDL.h"
#include <iostream>

#include <random>

#define WIDTH 1000
#define HEIGHT 1000
#define TITLE "SNAKE GAME"


bool COLLISION = false;
int dir = 0;
int MOVEMENT_SPEED = 10;
int SCORE;
bool ate = false;

enum Directions{
    UP,
    DOWN,
    RIGHT,
    LEFT
};


//Struct for the food
struct Food{
    SDL_Rect rect;
};

//randomizing the food item
Food randomFood( Food x ){
    int max = 300, min = 0;
    
    x.rect.x = rand() % max + min;
    x.rect.x -= ( x.rect.x % 10 );
    
    x.rect.y = rand() % max + min;
    x.rect.y -= ( x.rect.y % 10 );
    
    if( SCORE > 0 ){
        x.rect = { x.rect.x, x.rect.y, 10, 10 };
        return x;
    }
    else{
        x.rect = { 300, 300, 10, 10 };
        return x;
    }
    
}

 struct Snake{
     
     bool isNew;
    //This is the representation of the body of the snake
    SDL_Rect rect;
    //This points to the next segment of the snake in the body
    struct Snake *nextSeg;
     
};



struct Snake* head = NULL;
struct Snake* iter = NULL;



//appends nodes to the Snake
void appendSnake( ){
    iter = head;
    struct Snake* body = ( Snake *)malloc(sizeof(struct Snake));
    if(!body){
        printf(" Memory Error ");
        return;
    }
    
    while(iter->nextSeg != NULL ){
        iter = iter->nextSeg;
    }
    iter->nextSeg = body;
    body->isNew = true;
    
    body->rect.x = iter->rect.x;
    body->rect.y = iter->rect.y;
    body->rect.w = 10;
    body->rect.h = 10;

    body->nextSeg = NULL;
    
}

void update( struct Snake* head, struct Snake* prev ){
    struct Snake* update_iter;
    update_iter = head;
    
    if( update_iter->nextSeg != NULL ){
        update(update_iter->nextSeg, update_iter);
    }
    if( update_iter->isNew == true ){
        update_iter->isNew = false;
    }
    else{
        update_iter->rect.x = prev->rect.x;
        update_iter->rect.y = prev->rect.y;
    }
    
}




//Sets up the head node in the linked list
void initalize_Head(){
    
    SDL_Rect starting_pos = { 250, 250, 10, 10 };
    
    head = ( struct Snake *)malloc(sizeof(struct Snake));
    head->isNew = false;
    head->rect = starting_pos;
    head->nextSeg = NULL;
}

//Checking if the head has touched any part of the Snake or hits the border
void checkCollisions(){
    iter = head->nextSeg;

        while( iter != NULL  ){
            
            if( head->rect.x == iter->rect.x && head->rect.y == iter->rect.y ){
                COLLISION = true;
                std::cout << " GAME OVER " << std::endl;
                break;
                
            }
            else{
                iter = iter->nextSeg;
            }
        }
    
        if( head->rect.x > WIDTH || head->rect.y > HEIGHT ){
            COLLISION = true;
            std::cout << " GAME OVER " << std::endl;
    
            }
        else if ( head->rect.x < 0 || head->rect.y < 0 ){
            COLLISION = true;
            std::cout << " GAME OVER " << std::endl;
         
            }
}

void ifSnakeAte( Food x ){
    if( head->rect.x == x.rect.x && head->rect.y == x.rect.y ){
        ate = true;
    }
    
}

void move(int dir){
    
    update(head, head);
    
    switch (dir) {
        case UP:
            if( dir != DOWN ){
                head->rect.y -= MOVEMENT_SPEED;
                break;
            }
            else{
                head->rect.y += MOVEMENT_SPEED;
            }
            break;
        case DOWN:
            if( dir != UP ){
                head->rect.y += MOVEMENT_SPEED;
                break;
            }
            else{
                head->rect.y -= MOVEMENT_SPEED;
            }
            break;
        case RIGHT:
            if(dir != LEFT){
                head->rect.x += MOVEMENT_SPEED;
                break;
            }
            else{
                head->rect.x -= MOVEMENT_SPEED;
            }
            break;
        case LEFT:
            if(dir != RIGHT){
                head->rect.x -= MOVEMENT_SPEED;
                break;
            }
            else{
                head->rect.y += MOVEMENT_SPEED;
            }
            break;
    
    }
    checkCollisions();
}


int main(int argc, const char * argv[]) {
    int GAME_DELAY = 75;
    Food food;
    SDL_Rect starting_pos = { 250, 250, 10, 10 };

    // Checking the Instalization of SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == 0 ){
        std::cout<< "SDL has been initializaed..." << std::endl;
    }
    else{
        std::cout << "SDL has failed.. Errors: " << SDL_GetError() << std::endl;
    }
    
    
    //Creating Window
    SDL_Window *window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    
    //initializing head of the Snake
    initalize_Head();
    
    head = (struct Snake *)malloc(sizeof(Snake));
    head->isNew = false;
    head->rect = starting_pos;
    head->nextSeg = NULL;
    
    bool isRunning = true;
    SDL_Event e;
    
    //Adding food
    food = randomFood(food);
    
    //Main Game Loop
    while( isRunning ){
        
        if( COLLISION == true ){
            isRunning = false;
        }
        //std::cout << head->rect.x << std::endl;
        //Check Input
        while( SDL_PollEvent(&e) ){
            if( e.type == SDL_QUIT ) { isRunning = false; }
            if( e.type == SDL_KEYDOWN ){
                if( dir != DOWN )
                    if(e.key.keysym.sym == SDLK_UP) { dir = UP; }
                if( dir != UP )
                    if(e.key.keysym.sym == SDLK_DOWN) { dir = DOWN; }
                if( dir != LEFT )
                    if(e.key.keysym.sym == SDLK_RIGHT) { dir = RIGHT; }
                if( dir != RIGHT )
                    if(e.key.keysym.sym == SDLK_LEFT) { dir = LEFT; }
            }
        }
        
        
        
        //Changing directions
        move(dir);
        ifSnakeAte(food);
        
        // Checking if the head of the Snake intersects with the food
        if(ate == true){
            SCORE++;
            appendSnake();
            ate = false;
            food = randomFood(food);
            GAME_DELAY -= 2;
            printf( "SCORE: %d\n", SCORE );
        }
        
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &head->rect);
        
        
        iter = head->nextSeg;
        while( iter != NULL ){
            SDL_RenderFillRect(renderer, &iter->rect);
            iter = iter->nextSeg;
        }
        
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &food.rect);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(GAME_DELAY);
        
       
        
    }
    
    
    return 0;
}
