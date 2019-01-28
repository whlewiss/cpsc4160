#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "frameGenerator.h"

const std::string TITLE = "Hayden Lewis' ......";
const std::string NAME = "whlewis";

const int WIDTH = 640;
const int HEIGHT = 480;

void drawCircle(SDL_Renderer* renderer, SDL_Point center, int radius, SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w; // horizontal offset
      int dy = radius - h; // vertical offset
      if ((dx*dx + dy*dy) <= (radius * radius)) {
        SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
      }
    }
  }
}

void drawTriangle(SDL_Renderer* renderer,
   SDL_Point point1, SDL_Point point2, SDL_Point point3){

  SDL_RenderDrawLine(renderer, point1.x, point1.y, point2.x, point2.y);
  SDL_RenderDrawLine(renderer, point2.x, point2.y, point3.x, point3.y);
  SDL_RenderDrawLine(renderer, point3.x, point3.y, point1.x, point1.y);
}

void writeName(SDL_Renderer* renderer) {
  TTF_Init();
  TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 24);
  if (font == NULL) {
    throw std::string("error: font not found");
  }
  SDL_Color textColor = {0xff, 0, 0, 0};
  SDL_Surface* surface =
    TTF_RenderText_Solid(font, TITLE.c_str(), textColor);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {20, HEIGHT-40, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

void drawHead(SDL_Renderer *renderer){

  SDL_Point center = {320, 240};
  SDL_Color red = {255,0,0,255};
  SDL_Color egg = {200, 250, 250, 255};
  SDL_Color black = {0,0,0,255};

  SDL_Point leftHorn = {260, 70};
  SDL_SetRenderDrawColor(renderer, 200, 250, 250, 255);
  drawCircle(renderer, leftHorn, 40, egg);

  leftHorn.x+=20;
  leftHorn.y-=25;
  drawCircle(renderer, leftHorn, 50, black);

  SDL_Point c1 = {240, 60}, c2 = {220, 60}, c3 = {240, 70};


  SDL_SetRenderDrawColor(renderer, 200, 250, 250, 255);
  SDL_Rect headBlock = {270, 100, 100, 70};

  SDL_Point headSmooth = {270, 100};
  for(int i = 0; i<100; i++){
    drawCircle(renderer, headSmooth, 10, egg);
    headSmooth.y+=70;
    drawCircle(renderer, headSmooth, 10, egg);
    headSmooth.y-=70;
    headSmooth.x+=1;
  }
  headSmooth = {270, 100};
  for(int i = 0; i<70; i++){
    drawCircle(renderer, headSmooth, 10, egg);
    headSmooth.x+=100;
    drawCircle(renderer, headSmooth, 10, egg);
    headSmooth.x-=100;
    headSmooth.y+=1;
  }

  SDL_SetRenderDrawColor(renderer, 200, 250, 250, 255);
  SDL_RenderFillRect(renderer, &headBlock);

  SDL_Point eye1 = {295, 135};
  for(int i = 0; i < 13; i++){
    drawCircle(renderer, eye1, 15, black);
    eye1.y++;
  }
  eye1.y = 135;
  eye1.x = 345;
  for(int i = 0; i < 13; i++){
    drawCircle(renderer, eye1, 15, black);
    eye1.y++;
  }

  SDL_Rect leftHornTip = {220, 40, 20, 20};
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(renderer, &leftHornTip);

  SDL_Point t1 = {220, 60}, t2 = {240, 60}, t3 = {230, 70};
  drawTriangle(renderer, t1, t2, t3);
}

void drawGrid(SDL_Renderer *renderer){

  for(int i = 20; i < WIDTH; i+=20){
    SDL_RenderDrawLine(renderer, i, 0, i, HEIGHT);
  }
  for(int i = 20; i < HEIGHT; i+=20){
    SDL_RenderDrawLine(renderer, 0, i, WIDTH, i);
  }
}


int main(void) {
  try {
    if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) {
      std::cout << "Failed to initialize SDL2" << std::endl;
      return EXIT_FAILURE;
    }
    SDL_Window* window = SDL_CreateWindow(
      TITLE.c_str(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WIDTH,
      HEIGHT,
      SDL_WINDOW_SHOWN
    );
    // Apparently we can't use hardware acceleration with
    // SDL_GetWindowSurface
    SDL_Renderer* renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_SOFTWARE
    );

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    drawGrid(renderer);

    SDL_Point center = {320, 240};
    SDL_Color red = {255,0,0,255};
    SDL_Color egg = {200, 250, 250, 255};
    SDL_Color black = {0,0,0,255};

    drawHead(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    writeName(renderer);
    SDL_RenderPresent(renderer);
    FrameGenerator frameGen(renderer, window, WIDTH, HEIGHT, NAME);
    frameGen.makeFrame();

    SDL_Event event;
    const Uint8* keystate;
    while ( true ) {
      keystate = SDL_GetKeyboardState(0);
      if (keystate[SDL_SCANCODE_ESCAPE]) { break; }
      if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          break;
        }
      }
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  }
  catch (const std::string& msg) { std::cout << msg << std::endl; }
  catch (...) {
    std::cout << "Oops, someone threw an exception!" << std::endl;
  }
  return EXIT_SUCCESS;
}
