#ifndef MMU_HPP
#define MMU_HPP

#include "./mmu.hpp"
using namespace std;

MMU::MMU() { memset(memory, 0, 0xFFFF * sizeof(uint8_t)); }

uint8_t MMU::readByte(uint16_t address) { return memory[address]; }

uint16_t MMU::readWord(uint16_t address) {

  if (address & 1) {
    cout << "Invalid address for reading word : " << address << endl;
    return 0;
    ;
  }
  uint16_t rdata;
  rdata = memory[address] | (memory[address + 1] << 8);
  return rdata;
}

void MMU::writeByte(uint16_t address, uint8_t byte) { memory[address] = byte; }

void MMU::writeWord(uint16_t address, uint16_t word) {
  if (address & 1) {
    cout << "Invalid address for writing word : " << address << endl;
    return;
  }
  memory[address] = (word & 0xff);
  memory[address + 1] = (word >> 8);
}

#endif
