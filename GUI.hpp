#include <bits/stdc++.h>
#include <SDL2/SDL.h>

struct Inputs {
  bool exit = false;
};

class GUI {
  public:
  GUI()
  {
    this->renderer = NULL;
    this->window = NULL;
  }

  int Init(char* name, int width, int height, int pixSize)
  {
    this->width = width;
    this->height = height;
    this->pixSize = pixSize;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
      printf("Failed to Init SDL %s", SDL_GetError());
      return -1;
    }

    this->window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width * pixSize, height * pixSize, SDL_WINDOW_SHOWN);
    if (!window) {
      printf("Failed to make window %s", SDL_GetError());
      return -1;
    }

    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
      printf("Failed to make renderer %s", SDL_GetError());
      return -1;
    }

    return 0;
  }

  int Draw(uint8_t* FrameBuffer, int size)
  {
    SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
    SDL_RenderClear(renderer);

    SDL_Rect pixel;
    pixel.w = pixSize;
    pixel.h = pixSize;

    printf("Drawing");

    for (int i = 0; i < size; i++) {
      switch (FrameBuffer[i]) {
      case 0: { // Black Color, no need to draw anything
        continue;
      }
      case 1: { // Grey 1 Color
        SDL_SetRenderDrawColor(renderer, GREY1.r, GREY1.g, GREY1.b, GREY1.a);
        break;
      }
      case 2: { // Grey 2 Color
        SDL_SetRenderDrawColor(renderer, GREY2.r, GREY2.g, GREY2.b, GREY2.a);
        break;
      }
      case 3: { // White Color
        SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);
        break;
      }
      default: {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        printf("Invalid pixel color");
      }
      }

      pixel.x = (i % width) * pixSize;
      pixel.y = (i / width) * pixSize;
      SDL_RenderFillRect(renderer, &pixel);
    }

    SDL_RenderPresent(renderer);
    printf("Done Drawing");
    return 0;
  }

  int pollEvents(Inputs& events)
  {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT) {
        events.exit = true;
      }
    }
    return 0;
  }

  ~GUI()
  {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

  private:
  int pixSize;
  int width;
  int height;

  SDL_Renderer* renderer;
  SDL_Window* window;

  const SDL_Color BLACK = { 0, 0, 0, 255 };
  const SDL_Color GREY1 = { 85, 85, 85, 255 };
  const SDL_Color GREY2 = { 170, 170, 170, 255 };
  const SDL_Color WHITE = { 255, 255, 255, 255 };
};
