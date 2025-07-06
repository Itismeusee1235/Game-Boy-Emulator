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

uint8_t CPU::get_operand8(int index) {
  return (index >= 0) ? *reg8_pointers[index] : ram.readByte(reg.hl);
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
  int result = 0;

  uint8_t value = get_operand8(index);

  uint8_t A = *reg8_pointers[6];

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
void CPU::sub8(int index, bool carry) {
  uint8_t A = *reg8_pointers[6];

  uint8_t value = get_operand8(index);
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
void CPU::and8(int index) {
  uint8_t value = get_operand8(index);
  uint8_t result = value & *reg8_pointers[6];
  *reg8_pointers[6] = result;

  set_flag(4, 0);
  set_flag(5, 1);
  set_flag(6, 0);
  set_flag(7, result == 0);
}
void CPU::or8(int index) {
  uint8_t value = get_operand8(index);
  uint8_t result = value | *reg8_pointers[6];
  *reg8_pointers[6] = result;

  set_flag(4, 0);
  set_flag(5, 0);
  set_flag(6, 0);
  set_flag(7, result == 0);
}
void CPU::xor8(int index) {
  uint8_t value = get_operand8(index);
  if (index >= 0) {
    value = *reg8_pointers[index];
  }
  uint8_t result = value ^ *reg8_pointers[6];
  *reg8_pointers[6] = result;

  set_flag(4, 0);
  set_flag(5, 0);
  set_flag(6, 0);
  set_flag(7, result == 0);
}
void CPU::compare8(int index) {
  uint8_t value = get_operand8(index);
  if (index >= 0) {
    value = *reg8_pointers[index];
  }
  int result = 0;
  uint8_t A = *reg8_pointers[6];

  result = A - value;

  bool HC = (A & 15) < (value & 15);
  bool C = A < value;

  set_flag(4, C);
  set_flag(5, HC);
  set_flag(6, 1);
  set_flag(7, result == 0);
}

void CPU::increment8(int index) {
  uint8_t value = get_operand8(index);
  uint8_t result = value + 1;
  bool HC = (value & 0x0F) + 1 > 0x0F;
  bool Z = result == 0;

  set_flag(5, HC);
  set_flag(6, 0);
  set_flag(7, Z);
  if (index == -1) {
    ram.writeByte(reg.hl, result);
  } else {
    *reg8_pointers[index] = result;
  }
}

void CPU::decrement8(int index) {

  uint8_t value = get_operand8(index);
  uint8_t result = value - 1;
  bool HC = (value & 0xF) == 0;
  bool Z = result == 0;

  set_flag(5, HC);
  set_flag(6, 1);
  set_flag(7, Z);
  if (index == -1) {
    ram.writeByte(reg.hl, result);
  } else {
    *reg8_pointers[index] = result;
  }
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

void CPU::cpl() {
  *reg8_pointers[6] = ~(*reg8_pointers[6]);
  set_flag(5, 1);
  set_flag(6, 1);
}

void CPU::handle_CB(uint8_t sub_op) {
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
  // printf("Address: %4X, cc: %d, run: %d\n", address, cc, run);
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

  bool pc_shifted = false;

  uint8_t opcode;
  opcode = ram.readByte(reg.pc);

  uint8_t high = (opcode & 0xF0) >> 4;
  uint8_t low = opcode & 0x0F;

  uint16_t nextAddress = reg.pc + 1;
  uint8_t nextByte = ram.readByte(reg.pc + 1);
  uint16_t nextWord = ram.readWord(reg.pc + 1);

  printf("\nPC = %04X: %02X %02X %04X\n", reg.pc, ram.readByte(reg.pc),
         ram.readByte(reg.pc + 1), ram.readWord(reg.pc + 1));
  // ram.testMemory(reg.pc);

  if (ime_clock > 0) {
    ime_clock--;
  }

  switch (opcode) {

  // LD r16 n16
  case 0x01:
  case 0x11:
  case 0x21:
  case 0x31: {
    int des = (opcode >> 4) & 0x03;
    cout << "LD " << des << " n16" << endl;
    load16(des, nextAddress);
    break;
  }

  // LD r16 A
  case 0x02:
  case 0x12:
  case 0x22:
  case 0x32: {
    int des = (opcode >> 4) & 0x03;
    if (des == 2 || des == 3) {
      cout << "LD [HLI/D] A] " << endl;
      ram.writeByte(reg.hl, reg.a);
      if (des == 2) {
        reg.hl++;
      } else {
        reg.hl--;
      }
    } else {
      cout << "LD [" << des << "] A " << endl;
      ram.writeByte(*reg16_pointers[des], reg.a);
    }
    break;
  }

  // LD A r16
  case 0x0A:
  case 0x1A:
  case 0x2A:
  case 0x3A: {
    int src = (opcode >> 4) & 0x03;
    if (src == 2 || src == 3) {
      cout << "LD A [HLI/D]" << endl;
      reg.a = ram.readByte(reg.hl);
      if (src == 2) {
        reg.hl++;
      } else {
        reg.hl--;
      }
    } else {
      cout << "LD A [" << src << "]" << endl;
      reg.a = ram.readByte(*reg16_pointers[src]);
    }
    break;
  }

  // LD r n8
  case 0x06:
  case 0x16:
  case 0x26:
  case 0x36:
  case 0x0E:
  case 0x1E:
  case 0x2E:
  case 0x3E: {
    int des = (opcode >> 3) & 0x07;
    int des_index = (des == 7) ? 6 : des;
    if (des == 6) {
      cout << "LD [HL] n8" << endl;
      ram.writeByte(reg.hl, ram.readByte(nextAddress));
    } else {
      cout << "LD " << des_index << " n8" << endl;
      load8(des_index, nextAddress);
    }
    break;
  }

  // LD r1 r2
  case 0x40:
  case 0x41:
  case 0x42:
  case 0x43:
  case 0x44:
  case 0x45:
  case 0x46:
  case 0x47:
  case 0x48:
  case 0x49:
  case 0x4A:
  case 0x4B:
  case 0x4C:
  case 0x4D:
  case 0x4E:
  case 0x4F:
  case 0x50:
  case 0x51:
  case 0x52:
  case 0x53:
  case 0x54:
  case 0x55:
  case 0x56:
  case 0x57:
  case 0x58:
  case 0x59:
  case 0x5A:
  case 0x5B:
  case 0x5C:
  case 0x5D:
  case 0x5E:
  case 0x5F:
  case 0x60:
  case 0x61:
  case 0x62:
  case 0x63:
  case 0x64:
  case 0x65:
  case 0x66:
  case 0x67:
  case 0x68:
  case 0x69:
  case 0x6A:
  case 0x6B:
  case 0x6C:
  case 0x6D:
  case 0x6E:
  case 0x6F:
  case 0x70:
  case 0x71:
  case 0x72:
  case 0x73:
  case 0x74:
  case 0x75:
  case 0x77:
  case 0x78:
  case 0x79:
  case 0x7A:
  case 0x7B:
  case 0x7C:
  case 0x7D:
  case 0x7E:
  case 0x7F: {

    int src = opcode & 0x07;
    int des = (opcode >> 3) & 0x07;

    int src_index = (src == 7) ? 6 : src;
    int des_index = (des == 7) ? 6 : des;

    if (src == 6) {
      cout << "Loading into " << des_index << " from HL " << endl;
      load8(des_index, reg.hl);
    } else if (des == 6) {
      cout << "Loading into HL from " << des_index << endl;
      load8(reg.hl, src_index);
    } else {
      cout << "Loading " << des_index << " from " << src_index << endl;
      load8(src_index, des_index);
    }
    break;
  }

  // LD r a16, LD r16 A
  case 0xEA: {
    cout << "LD [a16] A" << endl;
    ram.writeByte(nextWord, reg.a);
    break;
  }
  case 0xFA: {
    cout << "LD A [n16]" << endl;
    reg.a = ram.readByte(nextWord);
    break;
  }

  // LDH [n8] A
  case 0xE0: {
    cout << "LD [0xFF00 + n8] A" << endl;
    ram.writeByte(0xFF00 + nextByte, reg.a);
    break;
  }
  case 0xE2: {
    cout << "LD [C] A" << endl;
    ram.writeByte(0xFF00 + reg.c, reg.a);
    break;
  }

  // LDH A n8
  case 0xF0: {
    cout << "LD A [n8]" << endl;
    reg.a = ram.readByte(0xFF00 + nextByte);
    break;
  }
  case 0xF2: {
    cout << "LD A [C]" << endl;
    reg.a = ram.readByte(0xFF00 + reg.c);
    break;
  }

  // LD a16 SP
  case 0x08: {
    cout << "LD a16 SP" << endl;
    ram.writeWord(nextWord, reg.sp);
    break;
  }

  // LD HL SP+e8
  case 0xF8: {
    cout << "LD HL SP+e8" << endl;
    uint16_t sp = reg.sp;
    int8_t offset = static_cast<int8_t>(nextByte);
    bool hc = ((sp & 0x0f) + (offset & 0x0f)) > 0x0f;
    bool c = ((sp & 0xff) + (offset & 0xff)) > 0xff;
    reg.hl = sp + offset;
    set_flag(4, c);
    set_flag(5, hc);
    set_flag(6, 0);
    set_flag(7, 0);
    break;
  }

  // LD SP HL
  case 0xF9: {
    cout << "LD SP HL" << endl;
    reg.sp = reg.hl;
    break;
  }

  // POP r16
  case 0xC1:
  case 0xD1:
  case 0xE1:
  case 0xF1: {
    int des = (opcode >> 4) & 0x03;
    if (des == 3) {
      cout << "POP AF" << endl;
      popAF();
    } else {
      cout << "POP " << des << endl;
      pop(des);
    }
    break;
  }

  // PUSH r16
  case 0xC5:
  case 0xD5:
  case 0xE5:
  case 0xF5: {
    int des = (opcode >> 4) & 0x03;
    if (des == 3) {
      cout << "PUSH AF" << endl;
      pushAF();
    } else {
      cout << "PUSH " << des << endl;
      push(des);
    }
    break;
  }

  // ADD ADC r1, r2
  case 0x80:
  case 0x81:
  case 0x82:
  case 0x83:
  case 0x84:
  case 0x85:
  case 0x86:
  case 0x87:
  case 0x88:
  case 0x89:
  case 0x8A:
  case 0x8B:
  case 0x8C:
  case 0x8D:
  case 0x8E:
  case 0x8F: {
    int src = opcode & 0x07;
    bool carry = opcode & 0x08;

    if (src == 6) {
      cout << "ADD A [HL]";
      add8(-1, carry);
    } else if (src == 7) {
      cout << "ADD A A";
      add8(6, carry);
    } else {
      add8(src, carry);
      cout << "ADD A " << src;
    }
    cout << carry << endl;
    break;
  }

  // ADD ADC A n8
  case 0xC6: {
    cout << "ADD A n8" << endl;
    uint8_t temp = reg.b;
    reg.b = nextByte;
    add8(0, 0);
    reg.b = temp;
    break;
  }
  case 0xCE: {
    cout << "ADC A n8" << endl;
    uint8_t temp = reg.b;
    reg.b = nextByte;
    add8(0, 1);
    reg.b = temp;
    break;
  }

  // SUB SUBC r1, r2
  case 0x90:
  case 0x91:
  case 0x92:
  case 0x93:
  case 0x94:
  case 0x95:
  case 0x96:
  case 0x97:
  case 0x98:
  case 0x99:
  case 0x9A:
  case 0x9B:
  case 0x9C:
  case 0x9D:
  case 0x9E:
  case 0x9F: {
    int src = opcode & 0x07;
    bool carry = opcode & 0x08;

    if (src == 6) {
      cout << "SUB A [HL]";
      sub8(-1, carry);
    } else if (src == 7) {
      cout << "SUB A A";
      sub8(6, carry);
    } else {
      cout << "SUB A " << src;
      sub8(src, carry);
    }
    cout << carry << endl;
    break;
  }

  // SUB SBC A n8
  case 0xD6: {
    cout << "SUB A n8" << endl;
    uint8_t temp = reg.b;
    reg.b = nextByte;
    sub8(0, 0);
    reg.b = temp;
    break;
  }
  case 0xDE: {
    cout << "SBC A n8" << endl;
    uint8_t temp = reg.b;
    reg.b = nextByte;
    sub8(0, 1);
    reg.b = temp;
    break;
  }

  // AND A r1
  case 0xA0:
  case 0xA1:
  case 0xA2:
  case 0xA3:
  case 0xA4:
  case 0xA5:
  case 0xA6:
  case 0xA7: {
    int src = opcode & 0x07;

    if (src == 6) {
      cout << "AND A [HL]" << endl;
      and8(-1);
    } else if (src == 7) {
      cout << "AND A A" << endl;
      and8(6);
    } else {
      cout << "AND A " << src << endl;
      and8(src);
    }
    break;
  }

    // AND A n8
  case 0xE6: {
    cout << "AND A n8" << endl;
    uint8_t temp = reg.b;
    reg.b = nextByte;
    and8(0);
    reg.b = temp;
    break;
  }

  // XOR A r1
  case 0xA8:
  case 0xA9:
  case 0xAA:
  case 0xAB:
  case 0xAC:
  case 0xAD:
  case 0xAE:
  case 0xAF: {
    int src = opcode & 0x07;

    if (src == 6) {
      cout << "XOR A [HL]" << endl;
      xor8(-1);
    } else if (src == 7) {
      cout << "XOR A A" << endl;
      xor8(6);
    } else {
      cout << "XOR A " << src << endl;
      xor8(src);
    }
    break;
  }

    // XOR A n8
  case 0xEE: {
    cout << "XOR A n8" << endl;
    uint8_t temp = reg.b;
    reg.b = nextByte;
    xor8(0);
    reg.b = temp;
    break;
  }

  // OR A r1
  case 0xB0:
  case 0xB1:
  case 0xB2:
  case 0xB3:
  case 0xB4:
  case 0xB5:
  case 0xB6:
  case 0xB7: {
    int src = opcode & 0x07;

    if (src == 6) {
      cout << "OR A [HL]" << endl;
      or8(-1);
    } else if (src == 7) {
      cout << "OR A A" << endl;
      or8(6);
    } else {
      cout << "OR A " << src << endl;
      or8(src);
    }
    break;
  }

    // OR A n8
  case 0xF6: {
    cout << "OR A n8" << endl;
    uint8_t temp = reg.b;
    reg.b = nextByte;
    or8(0);
    reg.b = temp;
    break;
  }

  // CP A r1
  case 0xB8:
  case 0xB9:
  case 0xBA:
  case 0xBB:
  case 0xBC:
  case 0xBD:
  case 0xBE:
  case 0xBF: {
    int src = opcode & 0x07;

    if (src == 6) {
      cout << "CP A [HL]" << endl;
      compare8(-1);
    } else if (src == 7) {
      cout << "CP A A" << endl;
      compare8(6);
    } else {
      cout << "CP A " << src << endl;
      compare8(src);
    }
    break;
  }

  case 0xFE: {
    cout << "CP A n8" << endl;
    uint8_t temp = reg.b;
    reg.b = nextByte;
    compare8(0);
    reg.b = temp;
    break;
  }
  // INC r8
  case 0x04:
  case 0x14:
  case 0x24:
  case 0x34:
  case 0x0C:
  case 0x1C:
  case 0x2C:
  case 0x3C: {
    int des = (opcode >> 3) & 0x07;
    cout << "INC " << des << endl;
    increment8(des);
    break;
  }

  // DEC r8
  case 0x05:
  case 0x15:
  case 0x25:
  case 0x35:
  case 0x0D:
  case 0x1D:
  case 0x2D:
  case 0x3D: {
    int des = (opcode >> 3) & 0x07;
    cout << "DEC " << des << endl;
    decrement8(des);
    break;
  }

  // SCF
  case 0x37: {
    cout << "SCF" << endl;
    set_flag(4, 1);
    set_flag(5, 0);
    set_flag(6, 0);
    break;
  }

  // DAA
  case 0x27: {
    cout << "DAA" << endl;
    // NOTE: Implement DAA
    break;
  }

  // CPL
  case 0x2F: {
    cout << "CPL" << endl;
    cpl();
    break;
  }

  // CCF
  case 0x3F: {
    cout << "CCF" << endl;
    set_flag(4, 0);
    set_flag(5, 0);
    set_flag(6, 0);
    break;
  }

  // INC r16
  case 0x03:
  case 0x13:
  case 0x23:
  case 0x33: {
    int des = (opcode >> 3) & 0x07;
    cout << "INC " << des << endl;
    increment16(des);
    break;
  }

  // DEC r16
  case 0x0B:
  case 0x1B:
  case 0x2B:
  case 0x3B: {
    int des = (opcode >> 3) & 0x07;
    cout << "DEC " << des << endl;
    increment16(des);
    break;
  }

    // ADD HL r16
  case 0x09:
  case 0x19:
  case 0x29:
  case 0x39: {
    int des = (opcode >> 3) & 0x07;
    cout << "ADD HL " << des << endl;
    add16(des);
    break;
  }

  // ADD SP e8
  case 0xE8: {
    cout << "ADD SP e8" << endl;
    addSP(static_cast<int8_t>(nextByte));
    break;
  }

  // JR cc e8
  case 0x18:
  case 0x20:
  case 0x28:
  case 0x30:
  case 0x38: {
    int cc = (opcode == 0x18) ? -1 : (opcode >> 3) & 0x03;
    cout << "JR " << cc << " e8" << endl;
    if (rel_jump(static_cast<int8_t>(nextByte), cc)) {
      pc_shifted = true;
    }
    break;
  }

  // JP cc a16
  case 0xC2:
  case 0xC3:
  case 0xCA:
  case 0xD2:
  case 0xDA: {
    int cc = (opcode == 0xC3) ? -1 : (opcode >> 3) & 0x03;
    cout << "JP " << cc << " a16" << endl;
    if (jump(nextAddress, cc)) {
      pc_shifted = true;
    }
    cout << pc_shifted << endl;
    break;
  }

  // RET cc
  case 0xC0:
  case 0xC8:
  case 0xC9:
  case 0xD0:
  case 0xD8: {
    int cc = (opcode == 0xC9) ? -1 : (opcode >> 3) & 0x03;
    cout << "RET " << cc << endl;
    if (ret(cc)) {
      pc_shifted = true;
    }
    break;
  }

  // RETI
  case 0xD9: {
    cout << "RETI" << endl;
    ret(-1);
    pc_shifted = true;
    ime_clock = 1;
    break;
  }

  // CALL cc a16
  case 0xC4:
  case 0xCC:
  case 0xCD:
  case 0xD4:
  case 0xDC: {
    int cc = (opcode == 0xCD) ? -1 : (opcode >> 3) & 0x03;
    cout << "CALL " << cc << " a16" << endl;
    if (call(cc)) {
      pc_shifted = true;
    }
    break;
  }

  // RST vec
  case 0xC7:
  case 0xD7:
  case 0xE7:
  case 0xF7:
  case 0xCF:
  case 0xDF:
  case 0xEF:
  case 0xFF: {
    cout << "RST" << endl;
    uint16_t address = (opcode >> 3) & 0x7;
    address *= 8;
    rst(address);
    pc_shifted = true;
    break;
  }

  // NOP
  case 0x0: {
    cout << "NOP" << endl;
    break;
  }

    // HALT
  case 0x76: {
    cout << "HALT" << endl;
    halted = true;
    break;
  }

  // STOP
  case 0x10: {
    cout << "STOP" << endl;
    break;
  }

  // DI
  case 0xF3: {
    cout << "DI" << endl;
    ime = false;
    break;
  }

  // EI
  case 0xFB: {
    cout << "EI" << endl;
    ime_clock = 2;
    break;
  }

    // CB prefix:
  case 0xCB: {
    cout << "CB" << endl;
    handle_CB(nextByte);
    break;
  }

  default: {
    printf("Unknow opcode %X\n", opcode);
  }
  }

  if (!pc_shifted) {
    reg.pc += pc_increments[high][low];
    cout << "Shifted PC by " << 0 + pc_increments[high][low] << endl;
    pc_shifted = false;
  }

  if (ime_clock == 1) {
    ime = true;
    ime_clock = 0;
  }
}
