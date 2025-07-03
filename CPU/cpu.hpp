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

  uint8_t *reg_pointers[7];

  MMU ram;

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
};

#endif
