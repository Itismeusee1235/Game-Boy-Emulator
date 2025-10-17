#include "./ppu.hpp"

void PPU::step(int tcycles)
{
  if (!(LCDC.displayEnable())) return; // LCD Disabled

  this->tcyclesCount += tcycles;
  modeClock += tcycles;

  while (tcyclesCount > 0) {
    switch (mode) {
    case HBlank: {
      if (LY >= 143) {
        enterMode(VBlank);
      } else {
        enterMode(OAMScan);
      }
      break;
    }
    case VBlank: {
      if (LY < 153) {
        enterMode(VBlank);
      } else {
        enterMode(OAMScan);
      }
      break;
    }
    case OAMScan: {

      uint16_t address = 0xFE00 + 4 * OAMindex;
      uint8_t x, y, tileNumber, flags;

      y = ram.readByte(address);
      x = ram.readByte(address + 1);
      tileNumber = ram.readByte(address + 2);
      flags = ram.readByte(address + 3);

      int spriteHeight = LCDC.spriteSize() ? 16 : 8;

      if (x > 0 && (LY + 16) >= y && (LY + 16) < y + spriteHeight && spriteCount < 10) {
        SpriteEntry sprite;
        sprite.x = x;
        sprite.y = y;
        sprite.tile = tileNumber;
        sprite.flags = flags;
        spriteCount++;
      }

      modeClock += 2;
      OAMindex++;
      if (modeClock >= 80) {
        enterMode(PixelTransfer);
        break;
      }
      break;
    }
    case PixelTransfer: {
      if (modeClock >= 172) {
        doPixelTransfer();
        enterMode(HBlank);
        modeClock -= 172;
      }
      break;
    }
    }
  }
}
