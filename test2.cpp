#include "./CPU/cpu.hpp"
#include <bits/stdc++.h>

int main() {
  CPU gb;
  char *tests[] = {"./TestROMS/01-special.gb",
                   "./TestROMS/02-interrupts.gb",
                   "./TestROMS/03-op sp,hl.gb",
                   "./TestROMS/04-op r,imm.gb",
                   "./TestROMS/05-op rp.gb",
                   "./TestROMS/06-ld r,r.gb",
                   "./TestROMS/07-jr,jp,call,ret,rst.gb",
                   "./TestROMS/08-misc instrs.gb",
                   "./TestROMS/09-op r,r.gb",
                   "./TestROMS/10-bit ops.gb",
                   "./TestROMS/11-op a,(hl).gb"};

  gb.ram.loadROM(tests[5]);
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
