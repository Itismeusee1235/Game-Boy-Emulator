#ifndef CPU_HPP
#define CPU_HPP

#include "../MMU/mmu.hpp"
#include <bits/stdc++.h>

class CPU {
  struct {
    union {
      struct {
        uint8_t f;
        uint8_t a;
      };
      uint16_t af;
    };
    union {
      struct {
        uint8_t c;
        uint8_t b;
      };
      uint16_t bc;
    };
    union {
      struct {
        uint8_t e;
        uint8_t d;
      };
      uint16_t de;
    };
    union {
      struct {
        uint8_t l;
        uint8_t h;
      };
      uint16_t hl;
    };
    uint16_t sp;
    uint16_t pc;
  } reg;

  bool ime = false;
  int ime_clock = 0;
  bool stopped = false;
  bool halted = false;

  uint8_t *reg8_pointers[7];
  uint16_t *reg16_pointers[4];

  MMU ram;

  const uint8_t pc_increments[16][16] = {
      {1, 3, 1, 1, 1, 1, 2, 1, 3, 1, 1, 1, 1, 1, 2, 1}, // 0x00–0x0F
      {2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1}, // 0x10–0x1F
      {2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1}, // 0x20–0x2F
      {2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1}, // 0x30–0x3F
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 0x40–0x4F
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 0x50–0x5F
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 0x60–0x6F
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 0x70–0x7F
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 0x80–0x8F
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 0x90–0x9F
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 0xA0–0xAF
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 0xB0–0xBF
      {1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 2, 3, 1, 2, 1}, // 0xC0–0xCF
      {1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 1, 3, 1, 2, 1}, // 0xD0–0xDF
      {2, 1, 2, 1, 0, 1, 2, 1, 3, 1, 3, 1, 3, 1, 3, 1}, // 0xE0–0xEF
      {2, 1, 2, 1, 0, 1, 2, 1, 3, 1, 3, 1, 3, 1, 3, 1}, // 0xF0–0xFF
  };

public:
  CPU();

  void print_reg();

  // Functions for setting and getting flags

  void set_flag(uint8_t flag, uint8_t val);
  bool get_flag(uint8_t flag);

  // Functions for setting and getting registers
  void set_reg8(int index, uint8_t value);
  uint8_t get_reg8(int index);

  void set_reg16(int index, uint16_t value);
  uint16_t get_reg16(int index);

  // Functions for loading
  void load(int des_index, int src_index);
  void load(int index, uint16_t address);
  void load(uint16_t address, int src_index);

  // Functions for 8 bit arithematic
  void add8(int index, bool carry);
  void add8(uint8_t value, bool carry);

  void compare8(int index);
  void compare8(uint8_t value);

  void decrement8(int index);
  void decrement8(uint16_t address);

  void increment8(int index);
  void increment8(uint16_t address);

  void sub8(int index, bool carry);
  void sub8(uint8_t value, bool carry);

  // Functions for 16 bit arithematic
  void add16(int index);
  void increment16(int index);
  void decrement16(int index);

  // Functions for bitwise logic
  void and8(int index);
  void and8(uint8_t value);

  void cpl();

  void or8(int index);
  void or8(uint8_t value);

  void xor8(int index);
  void xor8(uint8_t value);

  // Functions for bit flags
  void bit(int index, int pos);

  void set(int index, int pos);

  void res(int index, int pos);

  // Functions for bit shift
  void rl8(int index);
  void rlc8(int index);

  void rr8(int index);
  void rrc8(int index);

  void sla8(int index);

  void sra8(int index);
  void srl8(int index);

  void swap8(int index);

  // Functions for jumps and subroutines
  void jump(uint16_t address, int condition);
  void rel_jump(int8_t offset, int condition);

  // functions for stack manipulation
  void addSP(int8_t offset);

  void execute();
};

#endif
