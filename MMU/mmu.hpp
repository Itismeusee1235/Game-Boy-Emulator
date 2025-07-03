#include <bits/stdc++.h>

#define MEM_SIZE 16 * 1024

class MMU {
  uint8_t memory[MEM_SIZE];

public:
  MMU();

  uint8_t readByte(uint16_t address);
  uint16_t readWord(uint16_t address);

  void writeByte(uint16_t address, uint8_t byte);
  void writeWord(uint16_t address, uint16_t word);
};
