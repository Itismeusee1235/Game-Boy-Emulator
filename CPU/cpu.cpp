#include "./cpu.hpp"
using namespace std;

CPU::CPU() {
  reg.af = 0;
  reg.bc = 0;
  reg.de = 0;
  reg.hl = 0;
  reg.sp = 0;
  reg.pc = 0;
}

void CPU::print_reg() {
  cout << "f" << "\t\t" << "a" << "\t\t" << "af" << endl;
  cout << bitset<8>(reg.f) << "\t" << bitset<8>(reg.a) << "\t"
       << bitset<16>(reg.af) << endl;
  cout << "c" << "\t\t" << "b" << "\t\t" << "bc" << endl;
  cout << bitset<8>(reg.c) << "\t" << bitset<8>(reg.b) << "\t"
       << bitset<16>(reg.bc) << endl;
  cout << "e" << "\t\t" << "d" << "\t\t" << "de" << endl;
  cout << bitset<8>(reg.e) << "\t" << bitset<8>(reg.d) << "\t"
       << bitset<16>(reg.de) << endl;
  cout << "l" << "\t\t" << "h" << "\t\t" << "hl" << endl;
  cout << bitset<8>(reg.h) << "\t" << bitset<8>(reg.h) << "\t"
       << bitset<16>(reg.hl) << endl;
  cout << "sp" << endl;
  cout << bitset<16>(reg.sp) << endl;
  cout << "pc" << endl;
  cout << bitset<16>(reg.pc) << endl;
}

void CPU::set_flag(uint8_t flag, uint8_t val) {
  // carry       : 4
  // half carry  : 5
  // subtraction : 6
  // zero        : 7

  if (flag < 4 || flag > 7) {
    cout << "invalid flag for setting : " << static_cast<int>(flag) << endl;
    return;
  }
  if (val != 0 && val != 1) {
    cout << "Invalid value for setting : " << static_cast<int>(val) << endl;
    return;
  }

  reg.f = (reg.f & (~(1 << flag))) | (val << flag);
}

bool CPU::get_flag(uint8_t flag) {

  if (flag < 4 || flag > 7) {
    cout << "invalid flag for getting : " << static_cast<int>(flag) << endl;
    return 0;
  }

  return (reg.f >> flag) & 1;
}

void CPU::set_reg8(int index, uint8_t value) {
  // A _ : 1
  // B c : 2 3
  // D E : 4 5
  // H L : 5 7

  switch (index) {
  case 1: {
    reg.a = value;
    break;
  }
  case 2: {
    reg.b = value;
    break;
  }
  case 3: {
    reg.c = value;
    break;
  }
  case 4: {
    reg.d = value;
    break;
  }
  case 5: {
    reg.e = value;
    break;
  }
  case 6: {
    reg.h = value;
    break;
  }
  case 7: {
    reg.l = value;
    break;
  }
  default: {
    cout << "Invalid register(8) index for setting : " << index << endl;
  }
  }
}

uint8_t CPU::get_reg8(int index) {
  // A _ : 1
  // B c : 2 3
  // D E : 4 5
  // H L : 5 7

  switch (index) {
  case 1: {
    return reg.a;
  }
  case 2: {
    return reg.b;
  }
  case 3: {
    return reg.c;
  }
  case 4: {
    return reg.d;
  }
  case 5: {
    return reg.e;
  }
  case 6: {
    return reg.h;
  }
  case 7: {
    return reg.l;
  }
  default: {
    cout << "Invalid register(8) index for getting : " << index << endl;
  }
  }
  return 0;
}

void CPU::set_reg16(int index, uint16_t value) {
  //
  // AF : 1
  // BC : 2
  // DE : 3
  // HL : 4
  // SP : 5
  // PC : 6

  switch (index) {
  case 1: {
    reg.af = value;
    break;
  }
  case 2: {
    reg.bc = value;
    break;
  }
  case 3: {
    reg.de = value;
    break;
  }
  case 4: {
    reg.hl = value;
    break;
  }
  case 5: {
    reg.sp = value;
    break;
  }
  case 6: {
    reg.pc = value;
    break;
  }
  default: {
    cout << "Invalid register(16) index for setting : " << index << endl;
  }
  }
}

uint16_t CPU::get_reg16(int index) {
  //
  // AF : 1
  // BC : 2
  // DE : 3
  // HL : 4
  // SP : 5
  // PC : 6

  switch (index) {
  case 1: {
    return reg.af;
  }
  case 2: {
    return reg.bc;
  }
  case 3: {
    return reg.de;
  }
  case 4: {
    return reg.hl;
  }
  case 5: {
    return reg.sp;
  }
  case 6: {
    return reg.pc;
  }
  default: {
    cout << "Invalid register(16) index for getting : " << index << endl;
  }
    return 0;
  }
}
