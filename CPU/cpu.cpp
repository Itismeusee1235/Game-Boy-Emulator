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

void CPU::load(int des_index, int src_index) {

  if (!inRange(src_index, 0, 6) && !inRange(des_index, 0, 6)) {
    cout << "Invalid src/des index for loading: " << src_index << " "
         << des_index << endl;
    return;
  }

  *reg8_pointers[des_index] = *reg8_pointers[src_index];
}

void CPU::load(int index, uint16_t address) {

  if (!inRange(index, 0, 6)) {
    cout << "Invalid index for setting register(8) from address: " << index
         << endl;
    return;
  }
  *reg8_pointers[index] = ram.readByte(address);
}

void CPU::load(uint16_t address, int index) {
  if (!inRange(index, 0, 6)) {
    cout << "Invalid index for setting address from register(8): " << index
         << endl;
    return;
  }
  ram.writeByte(address, *reg8_pointers[index]);
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
  if (!inRange(index, 0, 2)) {
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

void CPU::jump(uint16_t address, int condition) {

  if (condition == 0 && !get_flag(7)) {
    return;
  } else if (condition == 1 && get_flag(7)) {
    return;
  } else if (condition == 2 && !get_flag(4)) {
    return;
  } else if (condition == 3 && get_flag(4)) {
    return;
  }

  reg.pc = address;
}

void CPU::rel_jump(int8_t offset, int condition) {

  if (condition == 0 && !get_flag(7)) {
    return;
  } else if (condition == 1 && get_flag(7)) {
    return;
  } else if (condition == 2 && !get_flag(4)) {
    return;
  } else if (condition == 3 && get_flag(4)) {
    return;
  }

  reg.pc += offset;
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

void CPU::execute() {
  uint8_t opcode;
  opcode = ram.readByte(reg.pc);
  uint8_t high = (opcode & 0xF0) >> 4;
  uint8_t low = opcode & 0x0F;

  bool pc_shifted = false;

  switch (high) {
  case 0x0: {
    switch (low) {
    case 0x2: {
      load(reg.bc, 6);
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
      load(0, ram.readByte(reg.pc + 1));
      break;
    }
    case 0x7: {
      rlc8(6);
      set_flag(5, 0);
      set_flag(6, 0);
      set_flag(7, 0);
      break;
    }
    case 0xA: {
      load(6, reg.bc);
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
      load(1, ram.readByte(reg.pc + 1));
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
    case 0x2: {
      load(reg.de, 6);
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
      load(2, ram.readByte(reg.pc + 1));
      break;
    }
    case 0x7: {
      rl8(6);
      set_flag(5, 0);
      set_flag(6, 0);
      set_flag(7, 0);
      break;
    }
    case 0xA: {
      load(6, reg.de);
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
      load(3, ram.readByte(reg.pc + 1));
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
    case 0x2: {
      load(reg.hl, 6);
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
      load(4, ram.readByte(reg.pc + 1));
      break;
    }
    case 0x7: {
      // NOTE: Implement DAA
    }
    case 0xA: {
      load(6, reg.hl);
      increment16(2);
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
      load(5, ram.readByte(reg.pc + 1));
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
    case 0x2: {
      load(reg.hl, 6);
      decrement16(2);
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
      load(reg.hl, ram.readByte(reg.pc + 1));
      break;
    }
    case 0x7: {
      set_flag(4, 1);
      set_flag(6, 0);
      set_flag(7, 0);
      break;
    }
    case 0xA: {
      load(6, reg.hl);
      decrement16(2);
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
      load(6, ram.readByte(reg.pc + 1));
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
      break;
    }

    if (dest == 6) {
      if (src == 7) {
        load(reg.hl, 6);
      } else {
        load(reg.hl, src);
      }
    } else if (dest == 7) {
      if (src == 6) {
        load(6, reg.hl);
      } else {
        load(6, src);
      }
    } else {
      load(dest, src);
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
    case 0x6: {
      add8(ram.readByte(reg.pc + 1), 0);
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
    case 0xE: {
      add8(ram.readByte(reg.pc + 1), 1);
      break;
    }
    }
    break;
  }
  case 0xD: {
    switch (low) {
    case 0x6: {
      sub8(ram.readByte(reg.pc + 1), 0);
      break;
    }
    case 0xE: {
      sub8(ram.readByte(reg.pc + 1), 1);
      break;
    }
    }
    break;
  }
  case 0xE: {
    switch (low) {
    case 0x0: {
      uint8_t n = ram.readByte(reg.pc + 1);
      load(0xFF00 + n, 6);
      break;
    }
    case 0x2: {
      load(0xFF00 + reg.c, 6);
      break;
    }
    case 0x6: {
      and8(ram.readByte(reg.pc + 1));
      break;
    }
    case 0xA: {
      load(ram.readWord(reg.pc + 1), 6);
      break;
    }
    case 0xE: {
      xor8(ram.readByte(reg.pc + 1));
    }
    }
    break;
  }
  case 0xF: {
    switch (low) {
    case 0x0: {
      uint8_t n = ram.readByte(reg.pc + 1);
      load(6, 0xFF00 + n);
      break;
    }
    case 0x2: {
      load(6, 0xFF00 + reg.c);
      break;
    }
    case 0x6: {
      or8(ram.readByte(reg.pc + 1));
      break;
    }
    case 0xA: {
      load(6, ram.readWord(reg.pc + 1));
      break;
    }
    case 0xE: {
      compare8(ram.readByte(reg.pc + 1));
      break;
    }
    }
    break;
  }
  }

  if (!pc_shifted) {
    reg.pc += pc_increments[high][low];
  }
}
