#include "./CPU/cpu.hpp"
#include <bits/stdc++.h>

int main() {
  CPU gb;
  char *rom = "./TestROMS/02-interrupts.gb";
  gb.ram.loadROM(rom);
  uint8_t byte = gb.ram.readByte(0xFF0F);
  byte = (byte & (~1)) | 1;
  gb.ram.writeByte(0xFF0F, byte);
  byte = gb.ram.readByte(0xFFFF);
  byte = (byte & (~1)) | 1;
  gb.ram.writeByte(0xFFFF, byte);
  while (true) {
    gb.execute();
    gb.print_reg();
    // if (gb.ram.readByte(gb.get_reg16(-1)) == 0x98) {
    //   gb.execute();
    //   gb.print_reg();
    //   break;
    // }
  }
  return -1;
}
