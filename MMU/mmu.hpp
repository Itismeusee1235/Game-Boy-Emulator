#include <bits/stdc++.h>

class MMU {
  uint8_t memory[0xFFFF];

  uint8_t readByte(uint16_t address);
  uint16_t readWord(uint16_t address);

  void writeByte(uint16_t address);
  void writeWord(uint16_t address);
};
