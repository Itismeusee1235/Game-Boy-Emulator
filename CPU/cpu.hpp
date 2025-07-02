#ifndef CPU_HPP
#define CPU_HPP

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

public:
  CPU();

  void print_reg();

  void set_flag(uint8_t flag, uint8_t val);
  bool get_flag(uint8_t flag);

  void set_reg8(int index, uint8_t value);
  uint8_t get_reg8(int index);

  void set_reg16(int index, uint16_t value);
  uint16_t get_reg16(int index);
};

#endif
