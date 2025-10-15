#include <iostream>
#include <cstdlib>
#include <ctime>
#include "./GUI.hpp"
const int WIDTH = 144;
const int HEIGHT = 60;

void generate(uint8_t* pixels)
{
  std::srand(std::time(nullptr));

  // Generate random pixel art
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      pixels[y * WIDTH + x] = std::rand() % 4; // 0 to 3
    }
  }
}

int main()
{
  GUI test;
  uint8_t pixels[HEIGHT][WIDTH];

  if (test.Init("Test", WIDTH, HEIGHT, 4) < 0) {
    return -1;
  }

  generate(&pixels[0][0]);
  int c = 0;

  Inputs inp;
  while (!inp.exit) {
    if (c >= pow(10, 2)) {
      c = 0;
      generate(&pixels[0][0]);
    }
    test.pollEvents(inp);
    test.Draw(&pixels[0][0], 100 * 100);
    c++;
  }

  return 0;
}
