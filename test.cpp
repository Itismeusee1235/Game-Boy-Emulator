#include "./CPU/cpu.hpp"
#include "./MMU/mmu.hpp"
#include "SDL2/SDL.h"
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

// int main() {
//   CPU gb;
//
//   char *filename = "./test.gb";
//   gb.ram.loadROM(filename);
//   int i = 0;
//
//   SDL_Init(SDL_INIT_EVENTS);
//   SDL_Window *win;
//   win = SDL_CreateWindow("This", SDL_WINDOWPOS_UNDEFINED,
//                          SDL_WINDOWPOS_UNDEFINED, 400, 400,
//                          SDL_WINDOW_SHOWN);
//
//   bool quit = false;
//   bool exec = true;
//   while (!quit) {
//     SDL_Event ev;
//     while (SDL_PollEvent(&ev)) {
//       if (ev.type == SDL_QUIT) {
//         quit = true;
//       } else if (ev.type == SDL_KEYDOWN) {
//         exec = true;
//       }
//     }
//
//     if (exec) {
//       gb.execute();
//       exec = false;
//     }
//     // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//   }
//   return 0;
// }
int main() {
  CPU gb;

  char *filename = "./01-special.gb";
  if (!gb.ram.loadROM(filename)) {
    cout << "Failed";
    return -1;
  }
  char rand;
  while (true) {
    gb.execute();
    cin >> rand;
  }

  return 0;
}
