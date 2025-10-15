#ifndef PPU_HPP
#define PPU_HPP

#include <bits/stdc++.h>
#include "../MMU/mmu.hpp"
using namespace std;

using Pixel = uint8_t;
using FrameBuffer = array<Pixel, 160 * 144>;

struct SpriteEntry {
  uint8_t y;
  uint8_t x;
  uint8_t tile;
  uint8_t flags;
};

class PPU {
  public:
  function<void(int)> requestInterrupt;

  uint8_t readIO(uint16_t addr);
  void writeIO(uint16_t addr, uint8_t value);

  void step(int tcycles);

  void reset();

  PPU();

  private:
  // Ram object
  MMU ram;

  // different modes that the ppo works on
  enum Mode { HBlank = 0,
    VBlank = 1,
    OAMScan = 2,
    PixelTransfer = 3 };
  Mode mode = OAMScan;
  int modeClock = 0; // counts dots

  array<SpriteEntry, 40> sprites {};

  // Registers
  struct LCDC {
    uint8_t raw = 0x91;
    int bgwinEnable() { return (raw & 0x01); };
    int spriteEnable() { return (raw & 0x02); };
    int spriteSize() { return (raw & 0x04); };
    int bgTileMap() { return (raw & 0x08); };
    int tileData() { return (raw & 0x10); };
    int winEnable() { return (raw & 0x20); };
    int winTileMap() { return (raw & 0x40); };
    int displayEnable() { return (raw & 0x80); };
  };

  struct STAT {
    uint8_t raw = 0x85;
    int ppuMode() { return (raw & 0x03); }
    int coincidence() { return (raw & 0x04); }
    int mode0Interrupt() { return (raw & 0x08); }
    int mode1Interrupt() { return (raw & 0x10); }
    int mode2Interrupt() { return (raw & 0x20); }
    int lcylyInterrupt() { return (raw & 0x40); }
  };

  uint8_t SCY = 0, SCX = 0;
  uint8_t LYC = 0;
  uint8_t BGP = 0xFC;
  uint8_t OBP0 = 0xFF, OBP1 = 0xFF;
  uint8_t WY = 0, WX = 0;
  uint8_t DMA = 0;

  // internal helpers
  void enterMode(Mode m);
  void updateSTATMode(); // set STAT mode bits, trigger STAT interrupt if configured
  void doVBlank(); // handle vblank timing & interrupt

  // memory helpers
  uint8_t vramRead(uint16_t addr) const;
  void vramWrite(uint16_t addr, uint8_t val);
  uint8_t oamRead(uint16_t addr) const;
  void oamWrite(uint16_t addr, uint8_t val);
  void readRegisters();
  void writeToRegister(int reg, uint8_t value);

  // utilities
  void performDMA(uint8_t highByte); // OAM DMA (copy from XX00..XX9F into OAM)
};

#endif
