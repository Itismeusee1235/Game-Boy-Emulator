#include "./CPU/cpu.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <bits/stdc++.h>
using namespace std;

int SCREEN_WIDTH = 200;
int SCREEN_HEIGHT = 200;

int instruction_counter = 0;
uint8_t LY = 0;

void update_ly_fake(MMU &m) {
  instruction_counter++;
  if (instruction_counter >= 10) {
    LY = (LY + 1) % 154;
    m.writeByte(0xFF44, LY);
    instruction_counter = 0;
  }
}

bool Break(CPU &gb) {
  // uint16_t PC = 0xC7B6;
  // uint16_t AF = 0x8160;
  // uint16_t BC = 0x00FF;
  // uint16_t DE = 0xC79B;
  // uint16_t HL = 0xC7A4;
  // uint16_t sp = 0xDFEF;
  uint16_t PC = 0xDEF8;
  uint16_t AF = 0x0000;
  uint16_t BC = 0xDEF4;
  uint16_t DE = 0xDEF5;
  uint16_t HL = 0xDEF6;
  uint16_t sp = 0xDFF1;

  if (gb.get_reg16(-1) == PC && gb.get_reg16(4) == AF &&
      gb.get_reg16(0) == BC && gb.get_reg16(1) == DE && gb.get_reg16(2) == HL &&
      gb.get_reg16(3) == sp) {
    return true;
  }
  return false;
}

int main() {

  CPU gb;
  gb.ram.loadROM("./TestROMS/11-op a,(hl).gb");

  SDL_Window *win;
  SDL_Renderer *rend;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("Failed to inti : %s ", SDL_GetError());
    return -1;
  }

  if (TTF_Init() < 0) {
    printf("Failed to init ttf: %s ", TTF_GetError());
    return -1;
  }

  win = SDL_CreateWindow("GameBoy Emulator", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                         SDL_WINDOW_SHOWN);
  if (win == NULL) {
    printf("Failed to make window: %s ", SDL_GetError());
    return -1;
  }

  rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

  if (rend == NULL) {
    printf("Failed to make SDL_Renderer: %s ", SDL_GetError());
    return -1;
  }

  TTF_Font *font =
      TTF_OpenFont("/home/fenrir/.local/share/fonts/FiraMono-Regular.ttf", 24);
  SDL_Color White = {255, 255, 255};
  SDL_Surface *msg_surf =
      TTF_RenderText_Blended_Wrapped(font, "Hello\n World", White, 200);
  SDL_Texture *msg = SDL_CreateTextureFromSurface(rend, msg_surf);
  SDL_Rect msg_rect{0, 0, msg_surf->w, msg_surf->h};

  bool quit = false;
  SDL_Event ev;
  bool execute = true;
  bool step = true;
  bool b = true;

  while (!quit) {
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT) {
        quit = true;
        break;
      } else if (ev.type == SDL_KEYDOWN) {
        if (ev.key.keysym.sym == SDLK_n) {
          execute = true;
        } else if (ev.key.keysym.sym == SDLK_TAB) {
          step = !step;
        } else if (ev.key.keysym.sym == SDLK_q) {
          quit = true;
          break;
        } else if (ev.key.keysym.sym == SDLK_b) {
          b = true;
        }
      }
    }

    if (!step) {
      gb.execute();
      update_ly_fake(gb.ram);
      if (!b) {

        SDL_Delay(1);
      }
    } else {
      if (execute) {
        gb.execute();
        update_ly_fake(gb.ram);
        execute = false;
      }
    }
    // 0xC37A
    // if ((gb.get_reg16(-1) == 0xC084 || gb.get_reg16(-1) == 0xC445) && b) {
    //   step = true;
    // }
    if (Break(gb) && b) {
      step = true;
    }

    SDL_SetRenderDrawColor(rend, 0x0, 0x0, 0x0, 0xF);
    SDL_RenderClear(rend);

    stringstream ss;
    ss << std::uppercase << std::hex << std::setfill('0') << "PC: 0x"
       << std::setw(4) << gb.get_reg16(-1) << "\n"
       << "AF: 0x" << std::setw(4) << gb.get_reg16(4) << "\n"
       << "BC: 0x" << std::setw(4) << gb.get_reg16(0) << "\n"
       << "DE: 0x" << std::setw(4) << gb.get_reg16(1) << "\n"
       << "HL: 0x" << std::setw(4) << gb.get_reg16(2) << "\n"
       << "SP: 0x" << std::setw(4) << gb.get_reg16(3) << "\n";

    msg_surf =
        TTF_RenderText_Blended_Wrapped(font, ss.str().c_str(), White, 200);
    msg = SDL_CreateTextureFromSurface(rend, msg_surf);
    msg_rect.w = msg_surf->w;
    msg_rect.h = msg_surf->h;

    SDL_RenderCopy(rend, msg, NULL, &msg_rect);

    SDL_DestroyTexture(msg);
    SDL_FreeSurface(msg_surf);

    SDL_RenderPresent(rend);
  }

  SDL_Quit();

  return 0;
}
