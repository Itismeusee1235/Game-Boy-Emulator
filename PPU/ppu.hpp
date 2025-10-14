#ifndef PPU_HPP
#define PPU_HPP

#include <bits/stdc++.h>
using namespace std;

using Pixel = uint8_t;
using FrameBuffer = array<Pixel, 160*144>;

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
        // different modes that the ppo works on
        enum Mode { HBlank = 0, VBlank = 1, OAMScan = 2, PixelTransfer = 3 };
        Mode mode = OAMScan;
        int modeClock = 0; // counts dots

        //memory
        array<uint8_t, 0x2000> vram{};
        array<uint8_t, 160> oam{};

        array<SpriteEntry, 40> sprites{};

        // Registers
        uint8_t LCDC = 0x91;
        uint8_t STAT = 0x85;
        uint8_t SCY = 0, SCX = 0;
        uint8_t LYC = 0;
        uint8_t BGP = 0xFC;
        uint8_t OBP0 = 0xFF, OBP1 = 0xFF;
        uint8_t WY = 0, WX = 0;
        uint8_t DMA = 0;

         // internal helpers
        void enterMode(Mode m);
        void updateSTATMode(); // set STAT mode bits, trigger STAT interrupt if configured
        void doVBlank();       // handle vblank timing & interrupt

        // memory helpers
        uint8_t vramRead(uint16_t addr) const;
        void vramWrite(uint16_t addr, uint8_t val);
        uint8_t oamRead(uint16_t addr) const;
        void oamWrite(uint16_t addr, uint8_t val);

        // utilities
        void performDMA(uint8_t highByte); // OAM DMA (copy from XX00..XX9F into OAM)
};

#endif
