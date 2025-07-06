#include "./CPU/cpu.hpp"
#include "./MMU/mmu.hpp"
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

int main() {
  CPU gb;

  char *filename = "./test.gb";
  gb.ram.loadROM(filename);
  int i = 0;
  while (true) {
    gb.execute();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    i++;
  }
  return 0;
}
