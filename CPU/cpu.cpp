#include "./cpu.hpp"
#include "utils.hpp"
#include <bits/stdc++.h>
#include <iomanip>

using namespace std;

CPU::CPU() {
  // reg.af = 0;
  // reg.bc = 0;
  // reg.de = 0;
  // reg.hl = 0;
  // reg.sp = 0;
  // reg.pc = 0;

  reg.a = 0x01;
  reg.b = 0x00;
  reg.c = 0x13;
  reg.d = 0x00;
  reg.e = 0xD8;
  reg.h = 0x01;
  reg.l = 0x4D;
  reg.sp = 0xFFFE;
  reg.pc = 0x0100;

  reg8_pointers[0] = &reg.b;
  reg8_pointers[1] = &reg.c;
  reg8_pointers[2] = &reg.d;
  reg8_pointers[3] = &reg.e;
  reg8_pointers[4] = &reg.h;
  reg8_pointers[5] = &reg.l;
  reg8_pointers[6] = &reg.a;

  reg16_pointers[0] = &reg.bc;
  reg16_pointers[1] = &reg.de;
  reg16_pointers[2] = &reg.hl;
  reg16_pointers[3] = &reg.sp;
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
  *reg8_pointers[index] = value;
}

uint8_t CPU::get_reg8(int index) {
  if (index < 0 || index >= 7) {
    cout << "Invalid index for setting register(8): " << index << endl;
    return -1;
  }
  return *reg8_pointers[index];
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

void CPU::load8(int des_index, int src_index) {

  if (!inRange(src_index, 0, 6) && !inRange(des_index, 0, 6)) {
    cout << "Invalid src/des index for loading: " << src_index << " "
         << des_index << endl;
    return;
  }

  *reg8_pointers[des_index] = *reg8_pointers[src_index];
}

void CPU::load8(int index, uint16_t address) {

  if (!inRange(index, 0, 6)) {
    cout << "Invalid index for setting register(8) from address: " << index
         << endl;
    return;
  }
  *reg8_pointers[index] = ram.readByte(address);
}

void CPU::load8(uint16_t address, int index) {
  if (!inRange(index, 0, 6)) {
    cout << "Invalid index for setting address from register(8): " << index
         << endl;
    return;
  }
  ram.writeByte(address, *reg8_pointers[index]);
}
void CPU::load16(int des_index, int src_index) {

  *reg16_pointers[des_index] = *reg16_pointers[src_index];
}

void CPU::load16(int index, uint16_t address) {

  *reg16_pointers[index] = ram.readWord(address);
}

void CPU::load16(uint16_t address, int index) {
  ram.writeWord(address, *reg16_pointers[index]);
}

void CPU::add8(int index, bool carry) {
  if (!inRange(index, 0, 6)) {
    cout << "Invalid index for add8 : " << index << endl;
    return;
  }
  int result = 0;
  uint8_t value = *reg8_pointers[index];
  uint8_t A = get_reg8(6);

  int car = carry ? get_flag(4) : 0;

  result = A + value + car;

  bool HC = (A & 15) + (value & 15) + car > 15;
  bool C = result > 255;

  *reg8_pointers[6] = static_cast<uint8_t>(result);
  set_flag(4, C);
  set_flag(5, HC);
  set_flag(6, 0);
  set_flag(7, *reg8_pointers[6] == 0);
}

void CPU::add8(uint8_t value, bool carry) {
  int result = 0;
  uint8_t A = get_reg8(6);

  int car = carry ? get_flag(4) : 0;

  result = A + value + car;

  bool HC = (A & 15) + (value & 15) + car > 15;
  bool C = result > 255;

  *reg8_pointers[6] = static_cast<uint8_t>(result);
  set_flag(4, C);
  set_flag(5, HC);
  set_flag(6, 0);
  set_flag(7, *reg8_pointers[6] == 0);
}

void CPU::compare8(int index) {
  if (!inRange(index, 0, 6)) {
    cout << "Invalid index for compare8 : " << index << endl;
    return;
  }
  int result = 0;
  uint8_t value = *reg8_pointers[index];
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

void CPU::increment8(int index) {
  if (!inRange(index, 0, 6)) {
    cout << "Invalid index for increment8 : " << index << endl;
    return;
  }
  uint8_t value = (*reg8_pointers[index])++;
  bool HC = (value & 0x0f) == 15;
  bool Z = *reg8_pointers[index] == 0;

  set_flag(5, HC);
  set_flag(6, 0);
  set_flag(7, Z);
}

void CPU::increment8(uint16_t address) {

  uint8_t value = ram.readByte(address);
  bool HC = (value & 0x0f) == 15;
  bool Z = (++value) == 0;

  set_flag(5, HC);
  set_flag(6, 0);
  set_flag(7, Z);
  ram.writeByte(address, value);
}

void CPU::decrement8(int index) {
  if (!inRange(index, 0, 6)) {
    cout << "Invalid index for increment8 : " << index << endl;
    return;
  }
  uint8_t value = *reg8_pointers[index]--;
  bool HC = (value & 0xF) == 0;
  bool Z = *reg8_pointers[index] == 0;

  set_flag(5, HC);
  set_flag(6, 1);
  set_flag(7, Z);
}

void CPU::decrement8(uint16_t address) {
  uint8_t value = ram.readByte(address);
  bool HC = (value & 0xF) == 0;
  bool Z = (--value) == 0;

  set_flag(5, HC);
  set_flag(6, 1);
  set_flag(7, Z);
  ram.writeByte(address, value);
}

void CPU::sub8(int index, bool carry) {
  if (!inRange(index, 0, 6)) {
    cout << "Invalid index for decrement8 : " << index << endl;
    return;
  }
  uint8_t A = *reg8_pointers[6];
  uint8_t value = *reg8_pointers[index];
  value += carry ? get_flag(4) : 0;

  bool HC = (A & 0x0F) < (value & 0x0F);
  bool C = A < value;

  uint8_t result = A - value;
  *reg8_pointers[6] = result;

  set_flag(4, C);
  set_flag(5, HC);
  set_flag(6, 1);
  set_flag(7, result == 0);
}

void CPU::sub8(uint8_t value, bool carry) {
  uint8_t A = *reg8_pointers[6];
  value += carry ? get_flag(4) : 0;

  bool HC = (A & 0x0F) < (value & 0x0F);
  bool C = A < value;

  uint8_t result = A - value;
  *reg8_pointers[6] = result;

  set_flag(4, C);
  set_flag(5, HC);
  set_flag(6, 1);
  set_flag(7, result == 0);
}

void CPU::add16(int index) {
  if (!inRange(index, 0, 3)) {
    cout << "Invalid index for add register16 : " << index << endl;
    return;
  }
  uint16_t value = *reg16_pointers[index];
  uint16_t HL = *reg16_pointers[2];

  bool HC = (HL & 0x0FFF) + (value & 0x0FFF) > 0x0FFF;
  bool C = (HL + value) > 0xFFFF;

  *reg16_pointers[2] = HL + value;

  set_flag(4, C);
  set_flag(5, HC);
  set_flag(6, 0);
}

void CPU::increment16(int index) {

  if (!inRange(index, 0, 2)) {
    cout << "Invalid index for increment register16 : " << index << endl;
    return;
  }
  *reg16_pointers[index] += 1;
}

void CPU::decrement16(int index) {

  if (!inRange(index, 0, 2)) {
    cout << "Invalid index for decrement register16 : " << index << endl;
    return;
  }
  *reg16_pointers[index] -= 1;
}

void CPU::and8(int index) {
  if (!inRange(index, 0, 6)) {
    cout << "Invalid index for and8 : " << index << endl;
    return;
  }
  uint8_t result = *reg8_pointers[index] & *reg8_pointers[6];
  *reg8_pointers[6] = result;

  set_flag(4, 0);
  set_flag(5, 1);
  set_flag(6, 0);
  set_flag(7, result == 0);
}
void CPU::and8(uint8_t value) {
  uint8_t result = value & *reg8_pointers[6];
  *reg8_pointers[6] = result;

  set_flag(4, 0);
  set_flag(5, 1);
  set_flag(6, 0);
  set_flag(7, result == 0);
}

void CPU::cpl() {
  *reg8_pointers[6] = ~(*reg8_pointers[6]);
  set_flag(5, 1);
  set_flag(6, 1);
}

void CPU::or8(int index) {
  if (!inRange(index, 0, 6)) {
    cout << "Invalid index for and8 : " << index << endl;
    return;
  }
  uint8_t result = *reg8_pointers[index] | *reg8_pointers[6];
  *reg8_pointers[6] = result;

  set_flag(4, 0);
  set_flag(5, 0);
  set_flag(6, 0);
  set_flag(7, result == 0);
}
void CPU::or8(uint8_t value) {
  uint8_t result = value | *reg8_pointers[6];
  *reg8_pointers[6] = result;

  set_flag(4, 0);
  set_flag(5, 0);
  set_flag(6, 0);
  set_flag(7, result == 0);
}

void CPU::xor8(int index) {
  if (!inRange(index, 0, 6)) {
    cout << "Invalid index for and8 : " << index << endl;
    return;
  }
  uint8_t result = *reg8_pointers[index] ^ *reg8_pointers[6];
  *reg8_pointers[6] = result;

  set_flag(4, 0);
  set_flag(5, 0);
  set_flag(6, 0);
  set_flag(7, result == 0);
}
void CPU::xor8(uint8_t value) {
  uint8_t result = value ^ *reg8_pointers[6];
  *reg8_pointers[6] = result;

  set_flag(4, 0);
  set_flag(5, 0);
  set_flag(6, 0);
  set_flag(7, result == 0);
}

void CPU::bit(int index, int pos) {
  if (index == -1) {
    bool val = (ram.readByte(reg.hl) & (1 << pos)) == 0;
    set_flag(5, 1);
    set_flag(6, 0);
    set_flag(7, val);

  } else {
    bool val = (*reg8_pointers[index] & (1 << pos)) == 0;
    set_flag(5, 1);
    set_flag(6, 0);
    set_flag(7, val);
  }
}

void CPU::set(int index, int pos) {
  if (index == -1) {
    uint8_t value = ram.readByte(reg.hl);
    value |= (1 << pos);
    ram.writeByte(reg.hl, value);
  } else {
    uint8_t value = *reg8_pointers[index];
    value |= (1 << pos);
    *reg8_pointers[index] = value;
  }
}

void CPU::res(int index, int pos) {
  if (index == -1) {
    uint8_t value = ram.readByte(reg.hl);
    value = (value & (~(1 << pos)));
    ram.writeByte(reg.hl, value);

  } else {
    uint8_t value = *reg8_pointers[index];
    value = (value & (~(1 << pos)));
    *reg8_pointers[index] = value;
  }
}

void CPU::rl8(int index) {
  if (index == -1) {

    uint8_t value = ram.readByte(reg.hl);

    bool C = value >> 7;
    value = (value << 1) | get_flag(4);

    set_flag(4, C);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);

    ram.writeByte(reg.hl, value);
  } else {

    uint8_t value = *reg8_pointers[index];

    bool c = value >> 7;
    value = (value << 1) | get_flag(4);

    set_flag(4, c);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);

    *reg8_pointers[index] = value;
  }
}

void CPU::rlc8(int index) {
  if (index == -1) {

    uint8_t value = ram.readByte(reg.hl);

    bool C = value >> 7;
    value = (value << 1) | C;

    set_flag(4, C);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);

    ram.writeByte(reg.hl, value);
  } else

  {
    uint8_t value = *reg8_pointers[index];

    bool C = value >> 7;
    value = (value << 1) | C;

    set_flag(4, C);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);

    *reg8_pointers[index] = value;
  }
}

void CPU::rr8(int index) {
  if (index == -1) {
    uint8_t value = ram.readByte(reg.hl);

    bool c = value & 0x1;
    value = (value >> 1) | (get_flag(4) << 7);

    set_flag(4, c);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);

    ram.writeByte(reg.hl, value);
  } else {
    uint8_t value = *reg8_pointers[index];

    bool c = value & 0x1;
    value = (value >> 1) | (get_flag(4) << 7);

    set_flag(4, c);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);

    *reg8_pointers[index] = value;
  }
}

void CPU::rrc8(int index) {
  if (index == -1) {
    uint8_t value = ram.readByte(reg.hl);

    bool c = value & 0x1;
    value = (value >> 1) | (c << 7);

    set_flag(4, c);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);

    ram.writeByte(reg.hl, value);

  } else {
    uint8_t value = *reg8_pointers[index];

    bool c = value & 0x1;
    value = (value >> 1) | (c << 7);

    set_flag(4, c);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);

    *reg8_pointers[index] = value;
  }
}

void CPU::sla8(int index) {
  if (index == -1) {
    uint8_t value = ram.readByte(reg.hl);

    bool C = value >> 7;

    value = value << 1;

    set_flag(4, C);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);

    ram.writeByte(reg.hl, value);

  } else {

    uint8_t value = *reg8_pointers[index];

    bool C = value >> 7;

    value = value << 1;

    set_flag(4, C);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);
    *reg8_pointers[index] = value;
  }
}

void CPU::sra8(int index) {
  if (index == -1) {
    uint8_t value = ram.readByte(reg.hl);

    bool C = value & 0x1;

    value = (value & 0x80) | (value >> 1);

    set_flag(4, C);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);

    ram.writeByte(reg.hl, value);

  } else {
    uint8_t value = *reg8_pointers[index];

    bool C = value & 0x1;

    value = (value & 0x80) | (value >> 1);

    set_flag(4, C);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);
    *reg8_pointers[index] = value;
  }
}

void CPU::srl8(int index) {
  if (index == -1) {
    uint8_t value = ram.readByte(reg.hl);

    bool C = value & 0x1;

    value = (value >> 1);

    set_flag(4, C);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);

    ram.writeByte(reg.hl, value);
  } else {
    uint8_t value = *reg8_pointers[index];

    bool C = value & 0x1;

    value = (value >> 1);

    set_flag(4, C);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);
    *reg8_pointers[index] = value;
  }
}

void CPU::swap8(int index) {
  if (index == -1) {

    uint8_t value = ram.readByte(reg.hl);

    value = (value << 4) | (value >> 4);

    set_flag(4, 0);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);

    ram.writeByte(reg.hl, value);
  } else {
    uint8_t value = *reg8_pointers[index];

    value = (value << 4) | (value >> 4);

    set_flag(4, 0);
    set_flag(5, 0);
    set_flag(6, 0);
    set_flag(7, value == 0);

    *reg8_pointers[index] = value;
  }
}

bool CPU::jump(uint16_t address, int cc) {

  bool run = false;
  if (cc == -1) {
    run = true;
  } else if (cc == 0 && !get_flag(7)) {
    run = true;
  } else if (cc == 1 && get_flag(7)) {
    run = true;
  } else if (cc == 2 && !get_flag(4)) {
    run = true;
  } else if (cc == 3 && get_flag(4)) {
    run = true;
  }
  if (run) {
    reg.pc = address;
  }
  return run;
}

bool CPU::rel_jump(int8_t offset, int cc) {

  bool run = false;
  if (cc == -1) {
    run = true;
  } else if (cc == 0 && !get_flag(7)) {
    run = true;
  } else if (cc == 1 && get_flag(7)) {
    run = true;
  } else if (cc == 2 && !get_flag(4)) {
    run = true;
  } else if (cc == 3 && get_flag(4)) {
    run = true;
  }
  if (run) {

    reg.pc += offset;
  }
  return run;
}

bool CPU::call(int cc) {
  bool run = false;
  if (cc == -1) {
    run = true;
  } else if (cc == 0 && !get_flag(7)) {
    run = true;
  } else if (cc == 1 && get_flag(7)) {
    run = true;
  } else if (cc == 2 && !get_flag(4)) {
    run = true;
  } else if (cc == 3 && get_flag(4)) {
    run = true;
  }
  if (run) {
    uint16_t push_address = reg.pc + 3;
    reg.sp -= 1;
    ram.writeByte(reg.sp, push_address >> 8);
    reg.sp -= 1;
    ram.writeByte(reg.sp, push_address & 0x00FF);
    jump(ram.readWord(reg.pc + 1), -1);
  }
  return run;
}

bool CPU::ret(int cc) {
  bool run = false;
  if (cc == -1) {
    run = true;
  } else if (cc == 0 && !get_flag(7)) {
    run = true;
  } else if (cc == 1 && get_flag(7)) {
    run = true;
  } else if (cc == 2 && !get_flag(4)) {
    run = true;
  } else if (cc == 3 && get_flag(4)) {
    run = true;
  }
  if (run) {
    uint8_t low, high;
    low = ram.readByte(reg.sp);
    reg.sp += 1;
    high = ram.readByte(reg.sp);
    reg.sp += 1;
    reg.pc = (high << 8) | low;
  }
  return run;
}

void CPU::rst(uint16_t address) {
  reg.sp -= 1;
  ram.writeByte(reg.sp, address >> 8);
  reg.sp -= 1;
  ram.writeByte(reg.sp, address & 0x00FF);
  jump(ram.readWord(reg.pc + 1), -1);
}

void CPU::addSP(int8_t off) {

  uint16_t value = reg.sp;
  uint16_t offset = static_cast<uint16_t>(static_cast<int16_t>(off));

  bool HC = (value & 0x0F) + (offset & 0x0F) > 0x0F;
  bool C = (value & 0xFF) + (offset & 0xFF) > 0xFF;

  set_flag(4, C);
  set_flag(5, HC);
  set_flag(6, 0);
  set_flag(7, 0);

  reg.sp = value + off;
}

void CPU::pop(int index) {
  uint8_t low = ram.readByte(reg.sp);
  reg.sp += 1;
  uint8_t high = ram.readByte(reg.sp);
  reg.sp += 1;
  *reg16_pointers[index] = (high << 8) | low;
}
void CPU::popAF() {
  uint8_t low = ram.readByte(reg.sp);
  reg.sp += 1;
  uint8_t high = ram.readByte(reg.sp);
  reg.sp += 1;
  low &= 0xF0;
  reg.af = (high << 8) | low;
}
void CPU::push(int index) {
  uint8_t low = *reg16_pointers[index] & 0x00FF;
  uint8_t high = *reg16_pointers[index] >> 8;
  reg.sp -= 1;
  ram.writeByte(reg.sp, high);
  reg.sp -= 1;
  ram.writeByte(reg.sp, low);
}
void CPU::pushAF() {
  uint8_t low = reg.f & 0xF0;
  uint8_t high = reg.a;
  reg.sp -= 1;
  ram.writeByte(reg.sp, high);
  reg.sp -= 1;
  ram.writeByte(reg.sp, low);
}

void CPU::execute() {
  uint8_t opcode;
  opcode = ram.readByte(reg.pc);
  uint8_t high = (opcode & 0xF0) >> 4;
  uint8_t low = opcode & 0x0F;

  uint16_t nextAddress = reg.pc + 1;
  uint8_t nextByte = ram.readByte(reg.pc + 1);
  uint16_t nextWord = ram.readWord(reg.pc + 1);

  printf("PC = %04X: %02X %02X %02X\n", reg.pc, ram.readByte(reg.pc),
         ram.readByte(reg.pc + 1), ram.readByte(reg.pc + 1));
  ram.testMemory(reg.pc);
  bool pc_shifted = false;

  if (ime_clock > 0) {
    ime_clock--;
  }

  switch (high) {
  case 0x0: {
    switch (low) {
    case 0x0: {
      break;
    }
    case 0x1: {
      load16(0, nextAddress);
      break;
    }
    case 0x2: {
      load8(reg.bc, 6);
      break;
    }
    case 0x3: {
      increment16(0);
      break;
    }
    case 0x4: {
      increment8(0);
      break;
    }
    case 0x5: {
      decrement8(0);
      break;
    }
    case 0x6: {
      load8(0, nextAddress);
      break;
    }
    case 0x7: {
      rlc8(6);
      set_flag(5, 0);
      set_flag(6, 0);
      set_flag(7, 0);
      break;
    }
    case 0x8: {
      load16(nextAddress, 3);
      break;
    }
    case 0x9: {
      add16(0);
      break;
    }
    case 0xA: {
      load8(6, reg.bc);
      break;
    }
    case 0xB: {
      decrement16(0);
      break;
    }
    case 0xC: {
      increment8(1);
      break;
    }
    case 0xD: {
      decrement8(1);
      break;
    }
    case 0xE: {
      load8(1, nextAddress);
      break;
    }
    case 0xF: {
      rrc8(6);
      set_flag(5, 0);
      set_flag(6, 0);
      set_flag(7, 0);
      break;
    }
    }
    break;
  }
  case 0x1: {
    switch (low) {
    case 0x0: {
      stopped = true;
      // Not used rn
      break;
    }
    case 0x1: {
      load16(1, nextAddress);
      break;
    }
    case 0x2: {
      load8(reg.de, 6);
      break;
    }
    case 0x3: {
      increment16(1);
      break;
    }
    case 0x4: {
      increment8(2);
      break;
    }
    case 0x5: {
      decrement8(2);
      break;
    }
    case 0x6: {
      load8(1, nextAddress);
      break;
    }
    case 0x7: {
      rl8(6);
      set_flag(5, 0);
      set_flag(6, 0);
      set_flag(7, 0);
      break;
    }
    case 0x8: {
      int8_t offset = static_cast<int8_t>(nextByte);
      if (rel_jump(offset, -1)) {
        pc_shifted = true;
      }
      break;
    }
    case 0x9: {
      add16(1);
      break;
    }
    case 0xA: {
      load8(6, reg.de);
      break;
    }
    case 0xB: {
      decrement16(1);
      break;
    }
    case 0xC: {
      increment8(3);
      break;
    }
    case 0xD: {
      decrement8(3);
      break;
    }
    case 0xE: {
      load8(3, nextAddress);
      break;
    }
    case 0xF: {
      rr8(6);
      set_flag(5, 0);
      set_flag(6, 0);
      set_flag(7, 0);
      break;
    }
    }
    break;
  }
  case 0x2: {
    switch (low) {
    case 0x0: {
      int8_t offset = static_cast<int8_t>(nextByte);
      if (rel_jump(offset, 0)) {
        pc_shifted = true;
      }
      break;
    }
    case 0x1: {
      load16(2, nextAddress);
      break;
    }
    case 0x2: {
      load8(reg.hl, 6);
      increment16(2);
      break;
    }
    case 0x3: {
      increment16(2);
      break;
    }
    case 0x4: {
      increment8(4);
      break;
    }
    case 0x5: {
      decrement8(4);
      break;
    }
    case 0x6: {
      load8(4, nextAddress);
      break;
    }
    case 0x7: {
      // NOTE: Implement DAA
      break;
    }
    case 0x8: {
      int8_t offset = static_cast<int8_t>(nextByte);
      if (rel_jump(offset, 1)) {
        pc_shifted = true;
      }
      break;
    }
    case 0x9: {
      add16(2);
      break;
    }
    case 0xA: {
      load8(6, reg.hl);
      increment16(2);
      break;
    }
    case 0xB: {
      decrement16(2);
      break;
    }
    case 0xC: {
      increment8(5);
      break;
    }
    case 0xD: {
      decrement8(5);
      break;
    }
    case 0xE: {
      load8(5, nextAddress);
      break;
    }
    case 0xF: {
      cpl();
      break;
    }
    }
    break;
  }
  case 0x3: {
    switch (low) {
    case 0x0: {
      int8_t offset = static_cast<int8_t>(nextByte);
      if (rel_jump(offset, 2)) {
        pc_shifted = true;
      }
      break;
    }
    case 0x1: {
      load16(3, nextAddress);
      break;
    }
    case 0x2: {
      load8(reg.hl, 6);
      decrement16(2);
      break;
    }
    case 0x3: {
      increment16(3);
      break;
    }
    case 0x4: {
      increment8(reg.hl);
      break;
    }
    case 0x5: {
      decrement8(reg.hl);
      break;
    }
    case 0x6: {
      // This is a address to addres load so i didnt make a separate
      // function,
      // one time thing
      ram.writeByte(reg.hl, nextByte);
      break;
    }
    case 0x7: {
      set_flag(4, 1);
      set_flag(6, 0);
      set_flag(7, 0);
      break;
    }
    case 0x8: {
      int8_t offset = static_cast<int8_t>(nextByte);
      if (rel_jump(offset, 3)) {
        pc_shifted = true;
      }
      break;
    }
    case 0x9: {
      add16(3);
      break;
    }
    case 0xA: {
      load8(6, reg.hl);
      decrement16(2);
      break;
    }
    case 0xB: {
      decrement16(3);
      break;
    }
    case 0xC: {
      increment8(6);
      break;
    }
    case 0xD: {
      decrement8(6);
      break;
    }
    case 0xE: {
      load8(6, nextAddress);
      break;
    }
    case 0xF: {
      set_flag(4, !get_flag(4));
      set_flag(6, 0);
      set_flag(7, 0);
      break;
    }
    }
    break;
  }
  case 0x4:
  case 0x5:
  case 0x6:
  case 0x7: {
    int dest = (opcode >> 3) & 0x07;
    int src = opcode & 0x7;

    if (dest == 6 && src == 6) {
      halted = true;
      break;
    }

    if (dest == 6) {
      if (src == 7) {
        load8(reg.hl, 6);
      } else {
        load8(reg.hl, src);
      }
    } else if (dest == 7) {
      if (src == 6) {
        load8(6, reg.hl);
      } else {
        load8(6, src);
      }
    } else {
      load8(dest, src);
    }

    break;
  }
  case 0x8: {
    int src = opcode & 0x07;
    bool carry = opcode & 0x08;

    if (src == 6) {
      add8(ram.readByte(reg.hl), carry);
    } else if (src == 7) {
      add8(6, carry);
    } else {
      add8(src, carry);
    }

    break;
  }
  case 0x9: {
    int src = opcode & 0x07;
    bool carry = opcode & 0x08;

    if (src == 6) {
      sub8(ram.readByte(reg.hl), carry);
    } else if (src == 7) {
      sub8(6, carry);
    } else {
      sub8(src, carry);
    }

    break;
  }
  case 0xA: {
    int src = opcode & 0x07;
    bool isXOR = opcode & 0x08;
    if (isXOR) {
      if (src == 6) {
        xor8(ram.readByte(reg.hl));
      } else if (src == 7) {
        xor8(6);
      } else {
        xor8(src);
      }
    } else {
      if (src == 6) {
        and8(ram.readByte(reg.hl));
      } else if (src == 7) {
        and8(6);
      } else {
        and8(src);
      }
    }

    break;
  }
  case 0xB: {

    int src = opcode & 0x07;
    bool isCompare = opcode & 0x08;
    if (isCompare) {
      if (src == 6) {
        compare8(ram.readByte(reg.hl));
      } else if (src == 7) {
        compare8(6);
      } else {
        compare8(src);
      }
    } else {
      if (src == 6) {
        or8(ram.readByte(reg.hl));
      } else if (src == 7) {
        or8(6);
      } else {
        or8(src);
      }
    }

    break;
  }
  case 0xC: {
    switch (low) {
    case 0x0: {
      if (ret(0)) {
        pc_shifted = true;
      }
      break;
    }
    case 0x1: {
      pop(0);
      break;
    }
    case 0x2: {
      if (jump(nextWord, 0)) {
        pc_shifted = true;
      }
      break;
    }
    case 0x3: {
      if (jump(nextWord, -1)) {
        pc_shifted = true;
      }
      break;
    }
    case 0x4: {
      if (call(0)) {
        pc_shifted = true;
      }
      break;
    }
    case 0x5: {
      push(0);
      break;
    }
    case 0x6: {
      add8(nextByte, 0);
      break;
    }
    case 0x7: {
      rst(0x00);
      pc_shifted = true;
      break;
    }
    case 0x8: {
      if (ret(1)) {
        pc_shifted = true;
      }
      break;
    }
    case 0x9: {
      if (ret(-1)) {
        pc_shifted = true;
      }
      break;
    }
    case 0xA: {
      if (jump(nextWord, 1)) {
        pc_shifted = true;
      }
      break;
    }
    case 0xB: {
      uint8_t sub_op = ram.readByte(reg.pc + 1);
      int sub_low = sub_op & 0x0F;
      int sub_high = (sub_op & 0xF0) >> 4;

      int index = sub_op & 0x07;
      bool type2 = sub_op & 0x08;

      if (index == 0x06) {
        index = -1;
      } else if (index == 0x07) {
        index = 6;
      }

      switch (sub_high) {
      case 0x0: {
        if (type2) {
          rrc8(index);
        } else {
          rlc8(index);
        }
        break;
      }
      case 0x1: {
        if (type2) {
          rr8(index);
        } else {
          rl8(index);
        }
        break;
      }
      case 0x2: {
        if (type2) {
          sra8(index);
        } else {
          sla8(index);
        }
        break;
      }
      case 0x3: {
        if (type2) {
          srl8(index);
        } else {
          swap8(index);
        }
        break;
      }
      case 0x4:
      case 0x5:
      case 0x6:
      case 0x7: {
        int pos = (sub_high - 0x4) * 2 + type2;
        bit(index, pos);
        break;
      }
      case 0x8:
      case 0x9:
      case 0xA:
      case 0xB: {
        int pos = (sub_high - 0x8) * 2 + type2;
        res(index, pos);
        break;
      }
      case 0xC:
      case 0xD:
      case 0xE:
      case 0xF: {
        int pos = (sub_high - 0x4) * 2 + type2;
        set(index, pos);
        break;
      }
      }
      break;
    }
    case 0xC: {
      if (call(1)) {
        pc_shifted = true;
      }
      break;
    }
    case 0xD: {
      if (call(-1)) {
        pc_shifted = true;
      }
      break;
    }
    case 0xE: {
      add8(nextByte, 1);
      break;
    }
    case 0xF: {
      rst(0x08);
      pc_shifted = true;
      break;
    }
    }
    break;
  }
  case 0xD: {
    switch (low) {
    case 0x0: {
      if (ret(2)) {
        pc_shifted = true;
      }
      break;
    }
    case 0x1: {
      pop(1);
      break;
    }
    case 0x2: {
      if (jump(nextWord, 2)) {
        pc_shifted = true;
      }
      break;
    }
    case 0x4: {
      if (call(2)) {
        pc_shifted = true;
      }
      break;
    }
    case 0x5: {
      push(1);
      break;
    }
    case 0x6: {
      sub8(nextByte, 0);
      break;
    }
    case 0x7: {
      rst(0x10);
      pc_shifted = true;
      break;
    }
    case 0x8: {
      if (ret(3)) {
        pc_shifted = true;
      }
      break;
    }
    case 0x9: {
      if (ret(-1)) {
        pc_shifted = true;
      }
      ime_clock = 1;
      break;
    }
    case 0xA: {
      if (jump(nextWord, 3)) {
        pc_shifted = true;
      }
      break;
    }
    case 0xC: {
      if (call(3)) {
        pc_shifted = true;
      }
      break;
    }
    case 0xE: {
      sub8(nextByte, 1);
      break;
    }
    case 0xF: {
      rst(0x18);
      pc_shifted = true;
      break;
    }
    }
    break;
  }
  case 0xE: {
    switch (low) {
    case 0x0: {
      load8(static_cast<uint16_t>(0xFF00 + nextByte), 6);
      break;
    }
    case 0x1: {
      pop(2);
      break;
    }
    case 0x2: {
      load8(0xFF00 + reg.c, 6);
      break;
    }
    case 0x5: {
      push(2);
      break;
    }
    case 0x6: {
      and8(nextByte);
      break;
    }
    case 0x7: {
      rst(0x20);
      pc_shifted = true;
      break;
    }
    case 0x8: {
      addSP(static_cast<int8_t>(nextByte));
      break;
    }
    case 0x9: {
      if (jump(reg.hl, -1)) {
        pc_shifted = true;
      }
      break;
    }
    case 0xA: {
      load8(nextAddress, 6);
      break;
    }
    case 0xE: {
      xor8(nextByte);
      break;
    }
    case 0xF: {
      rst(0x28);
      pc_shifted = true;
      break;
    }
    }
    break;
  }
  case 0xF: {
    switch (low) {
    case 0x0: {
      load8(6, 0xFF00 + nextByte);
      break;
    }
    case 0x1: {
      popAF();
      break;
    }
    case 0x2: {
      load8(6, 0xFF00 + reg.c);
      break;
    }
    case 0x3: {
      ime = false;
      break;
    }
    case 0x5: {
      pushAF();
      break;
    }
    case 0x6: {
      or8(nextByte);
      break;
    }
    case 0x7: {
      rst(0x30);
      pc_shifted = true;
      break;
    }
    case 0x8: {
      int8_t offset = static_cast<int8_t>(nextByte);
      uint16_t SP = reg.sp;
      bool HC = (SP & 0x07) + (offset & 0x07) > 0x7;
      bool C = (SP & 0xFF) + (offset & 0xFF) > 0xFF;
      reg.hl = reg.sp + offset;
      set_flag(4, C);
      set_flag(5, HC);
      set_flag(6, 0);
      set_flag(7, 0);
      break;
    }
    case 0x9: {
      load16(2, 3);
      break;
    }
    case 0xA: {
      load8(6, nextAddress);
      break;
    }
    case 0xB: {
      ime_clock = 2;
      break;
    }
    case 0xE: {
      compare8(nextByte);
      break;
    }
    case 0xF: {
      rst(0x38);
      pc_shifted = true;
      break;
    }
    }
    break;
  }
  }

  if (!pc_shifted) {
    reg.pc += pc_increments[high][low];
    pc_shifted = false;
  }

  if (ime_clock == 1) {
    ime = true;
    ime_clock = 0;
  }
}
