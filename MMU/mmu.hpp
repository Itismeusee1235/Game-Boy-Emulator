#ifndef MMU_HPP
#define MMU_HPP

#include <bits/stdc++.h>

#define MEM_SIZE 0x10000

class MMU {
  uint8_t memory[MEM_SIZE];

public:
  MMU();

  bool loadROM(char *filename);

  uint8_t readByte(uint16_t address);
  uint16_t readWord(uint16_t address);

  void writeByte(uint16_t address, uint8_t byte);
  void writeWord(uint16_t address, uint16_t word);

public:
  void testMemory(int index) {
    std::cout << index << " " << std::bitset<8>(memory[index]) << " "
              << std::bitset<8>(readByte(index)) << " "
              << std::bitset<16>(readWord(index)) << std::endl;
  }
  void printMem();
};

#endif
