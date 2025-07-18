#include "./mmu.hpp"
#include <fstream>
#include <iostream>
using namespace std;

MMU::MMU() { memset(memory, 0, 0xFFFF * sizeof(uint8_t)); }

bool MMU::loadROM(char *filename) {
  std::ifstream file;
  size_t size = 0;
  file.open(filename, ios::in | ios::binary);
  if (file.is_open()) {
    file.seekg(0, ios::end);
    size = file.tellg();
    if (size > 0x10000) {
      cout << "ROM too big to load" << endl;
      return false;
    }
    file.seekg(0, ios::beg);
    char c;
    for (int i = 0x0; file.get(c); i++) {
      memory[i] = (uint8_t)c;
    }

  } else {
    return false;
  }

  cout << "ROM Loaded. First few bytes:\n";
  for (int i = 0; i < 8; i++) {
    printf("0x%04X: 0x%02X\n", i, memory[i]);
  }
  return true;
}

uint8_t MMU::readByte(uint16_t address) { return memory[address]; }

uint16_t MMU::readWord(uint16_t address) {

  uint16_t rdata;
  rdata = memory[address] | (memory[address + 1] << 8);
  return rdata;
}

void MMU::writeByte(uint16_t address, uint8_t byte) {

  memory[address] = byte;
  // printf("writing to %4X, %2X\n", address, byte);
  if (address == 0xFF02 && (byte & 0x81) == 0x81) {
    // cout << "Serial Port" << endl;
    char ch = static_cast<char>(memory[0xFF01]);
    cout << ch << flush;
    // cout << "-" << ch << "-" << flush;
    std::ofstream file("out.txt", std::ios::app); // open in append mode
    if (file.is_open()) {
      file << ch; // append the character
      file.close();
    } else {
      std::cerr << "Failed to open file." << std::endl;
    }
    memory[0xFF02] = 0x00;
  }
}

void MMU::writeWord(uint16_t address, uint16_t word) {
  memory[address] = (word & 0xff);
  memory[address + 1] = (word >> 8);
}

void MMU::printMem() {
  for (int i = 0; i < 0x8000; i++) {
    printf("%4X> ", i);
    for (int j = 0; j < 0xF; j++) {
      printf("%2X ", memory[i++]);
    }
    cout << endl;
  }
}
