#include "./cpu.hpp"
#include "utils.hpp"
using namespace std;

CPU::CPU() {
  reg.af = 0;
  reg.bc = 0;
  reg.de = 0;
  reg.hl = 0;
  reg.sp = 0;
  reg.pc = 0;

  reg_pointers[0] = &reg.b;
  reg_pointers[1] = &reg.c;
  reg_pointers[2] = &reg.d;
  reg_pointers[3] = &reg.e;
  reg_pointers[4] = &reg.h;
  reg_pointers[5] = &reg.l;
  reg_pointers[6] = &reg.a;
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
  cout << bitset<8>(reg.l) << "\t" << bitset<8>(reg.h) << "\t"
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
  if (index < 0 || index >= 7) {
    cout << "Invalid index for setting register(8): " << index << endl;
    return;
  }
  *reg_pointers[index] = value;
}

uint8_t CPU::get_reg8(int index) {
  if (index < 0 || index >= 7) {
    cout << "Invalid index for setting register(8): " << index << endl;
    return -1;
  }
  return *reg_pointers[index];
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

void CPU::load(int des_index, int src_index) {

  if (!inRange(src_index, 0, 6) && !inRange(des_index, 0, 6)) {
    cout << "Invalid src/des index for loading: " << src_index << " "
         << des_index << endl;
    return;
  }

  *reg_pointers[des_index] = *reg_pointers[src_index];
}

void CPU::load(int index, uint16_t address) {

  if (!inRange(index, 0, 6)) {
    cout << "Invalid index for setting register(8) from address: " << index
         << endl;
    return;
  }
  *reg_pointers[index] = ram.readByte(address);
}

void CPU::load(uint16_t address, int index) {
  if (!inRange(index, 0, 6)) {
    cout << "Invalid index for setting address from register(8): " << index
         << endl;
    return;
  }
  ram.writeByte(address, *reg_pointers[index]);
}

void CPU::add8(int index, bool carry) {
  int result = 0;
  uint8_t value = *reg_pointers[index];
  uint8_t A = get_reg8(6);

  int car = carry ? get_flag(4) : 0;

  result = A + value + car;

  bool HC = (A & 15) + (value & 15) + car > 15;
  bool C = result > 255;

  *reg_pointers[6] = static_cast<uint8_t>(result);
  set_flag(4, C);
  set_flag(5, HC);
  set_flag(6, 0);
  set_flag(7, *reg_pointers[6] == 0);
}

void CPU::add8(uint8_t value, bool carry) {
  int result = 0;
  uint8_t A = get_reg8(6);

  int car = carry ? get_flag(4) : 0;

  result = A + value + car;

  bool HC = (A & 15) + (value & 15) + car > 15;
  bool C = result > 255;

  *reg_pointers[6] = static_cast<uint8_t>(result);
  set_flag(4, C);
  set_flag(5, HC);
  set_flag(6, 0);
  set_flag(7, *reg_pointers[6] == 0);
}

void CPU::compare8(int index) {
  int result = 0;
  uint8_t value = *reg_pointers[index];
  uint8_t A = get_reg8(6);

  result = A - value;

  bool HC = (A & 15) < (value & 15);
  bool C = A < value;

  set_flag(4, C);
  set_flag(5, HC);
  set_flag(6, 1);
  set_flag(7, result == 0);
}

void CPU::compare8(uint8_t value) {
  int result = 0;
  uint8_t A = get_reg8(6);

  result = A - value;

  bool HC = (A & 15) < (value & 15);
  bool C = A < value;

  set_flag(4, C);
  set_flag(5, HC);
  set_flag(6, 1);
  set_flag(7, result == 0);
}
