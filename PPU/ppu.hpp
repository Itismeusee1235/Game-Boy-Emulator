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
  void writeIO(uint16_t addr, uint16_t value);

  void step(int tcycles);

  void reset();

  PPU();

  private:
  // Ram object
  MMU ram;
  uint8_t framebuffer[144][160]; // color value of each pixel

  // different modes that the ppo works on
  enum Mode { HBlank = 0,
    VBlank = 1,
    OAMScan = 2,
    PixelTransfer = 3 };
  Mode mode = OAMScan;
  int tcyclesCount = 0; // Stores the number of tcycles to run
  int modeClock = 0; // counts dots

  array<SpriteEntry, 10> sprites {}; //  Number of sprites to display

  // Registers
  struct {
    uint8_t raw = 0x91;
    int bgwinEnable() { return (raw & 0x01); };
    int spriteEnable() { return (raw & 0x02); };
    int spriteSize() { return (raw & 0x04); };
    int bgTileMap() { return (raw & 0x08); };
    int tileData() { return (raw & 0x10); };
    int winEnable() { return (raw & 0x20); };
    int winTileMap() { return (raw & 0x40); };
    int displayEnable() { return (raw & 0x80); };
  } LCDC;

  struct {
    uint8_t raw = 0x85;
    int ppuMode() { return (raw & 0x03); }
    int coincidence() { return (raw & 0x04); }
    int mode0Interrupt() { return (raw & 0x08); }
    int mode1Interrupt() { return (raw & 0x10); }
    int mode2Interrupt() { return (raw & 0x20); }
    int lcylyInterrupt() { return (raw & 0x40); }
  } STAT;

  int LY = 0;
  uint8_t SCY = 0, SCX = 0; // LY and LX
  uint8_t LYC = 0;
  uint8_t BGP = 0xFC;
  uint8_t OBP0 = 0xFF, OBP1 = 0xFF;
  uint8_t WY = 0, WX = 0;
  uint8_t DMA = 0;

  int OAMindex = 0; // index of the sprite it is checking
  int spriteCount = 0; // number of sprites on current scanline

  // internal helpers
  void enterMode(Mode m);
  void updateSTATMode(); // set STAT mode bits, trigger STAT interrupt if configured
  void doVBlank(); // handle vblank timing & interrupt
  void triggerLYCCompare(); // compares the value of the lyc and ly registers

  // memory helpers
  uint8_t vramRead(uint16_t addr);
  void vramWrite(uint16_t addr, uint8_t val);

  // utilities
  void performDMA(uint8_t highByte); // OAM DMA (copy from XX00..XX9F into OAM)
  void doPixelTransfer();
};

#endif
