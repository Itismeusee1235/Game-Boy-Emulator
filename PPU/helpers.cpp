#include "ppu.hpp"

void PPU::triggerLYCCompare() {
    bool match = (LY == LYC);
    uint8_t stat_interrupt;
    if (match) stat_interrupt = (STAT.raw | 0x04);
    else stat_interrupt = STAT.coincidence();
    if (match && (STAT.raw & (1 << 6)) && requestInterrupt)
        requestInterrupt(1); // LYC=LY STAT interrupt
}

void PPU::enterMode(Mode m) {

    uint8_t stat_interrupt = (STAT.ppuMode() | static_cast<uint8_t>(m));

    bool trigger = false;

    switch(m) {
        case HBlank: {
            if (stat_interrupt & (1 << 3)) trigger = true;
            else printf("EnterMode Error: HBlank");
            break;
        }

        case VBlank: {
            if (stat_interrupt & (1 << 4)) {
                trigger = true;
            } else {
                printf("EnterMode Error: Vblank");
            }
            break;
        }

        case OAMScan: {
            if (stat_interrupt & (1 << 5)) {
                trigger = true;
            } else {
                printf("EnterMode Error: OAMScan");
            }
            break;
        }

        case PixelTransfer:
            break;
        
        default:
            printf("EnterMode: lmao mode nahi mila");
    }

    if (trigger && requestInterrupt) {
        requestInterrupt(1); // LCD STAT INTERRUPT
    } else {
        printf("EnterMode Error: trigger or requestInterrupt returned false");
    }


    PPU::triggerLYCCompare();
}


uint8_t PPU::readIO(uint16_t addr) {
    switch(addr) {
        case 0xFF40: return LCDC.raw; break;
        case 0xFF41: return STAT.raw; break;
        case 0xFF42: return SCY; break;
        case 0xFF43: return SCX; break;
        case 0xFF44: return LY; break;
        case 0xFF45: return LYC; break;
        case 0xFF47: return BGP; break;
        case 0xFF48: return OBP0; break;
        case 0xFF49: return OBP1; break;
        case 0xFF4A: return WY; break;
        case 0xFF4B: return WX; break;
        default: return 0xFF; break;
    }
}

void PPU::writeIO(uint16_t addr, uint16_t val) {
    switch(addr) {
        case 0xFF40: LCDC.raw = val; break;
        // case 0xFF41: STAT =  break; // dont exactly know how to implement this
        case 0xFF42: SCY = val; break;
        case 0xFF43: SCX = val; break;
        case 0xFF44: LY = 0; break; // writing resets LY
        case 0xFF45: LYC = val; break;
        case 0xFF46: performDMA(val); break;
        case 0xFF47: BGP = val; break;
        case 0xFF48: OBP0 = val; break;
        case 0xFF49: OBP1 = val; break;
        case 0xFF4A: WY = val; break;
        case 0xFF4B: WX = val; break;
        default: break;
    }
}

uint8_t PPU::vramRead(uint16_t addr) {
    if (addr < 0x8000 || addr > 0x9FFF) return 0xFF;
    uint8_t output = ram.readByte(addr);
    return output;
}

void PPU::vramWrite(uint16_t addr, uint8_t val) {
    if (addr < 0x8000 || addr > 0x9FFF) return;
    ram.writeByte(addr, val);
}

void PPU::reset() {
    mode = OAMScan;
    modeClock = 0;
    LY = 0;

    LCDC.raw = 0x91;
    STAT.raw = 0x85;
    SCY = SCX = 0;
    LYC = 0;
    BGP = 0xFC;
    OBP0 = OBP1 = 0xFF;
    WY = WX = 0;
    DMA = 0;
}

void PPU::doPixelTransfer() {
    uint8_t y = LY;
    uint8_t scy = SCY;
    uint8_t scx = SCX;
    uint8_t wx = WX - 7;
    uint8_t wy = WY;

    bool useWindow = (LCDC.winEnable()) && (y >= wy);
    uint16_t tileMapBase = (LCDC.bgTileMap()) ? 0x9C00 : 0x9800;
    uint16_t tileDataBase = (LCDC.tileData()) ? 0x8000 : 0x8800;

    for (int x = 0; x < 160; x++) {
        uint8_t bgX, bgY;
        if (useWindow && x >= wx) {
            bgX = x - wx;
            bgY = y - wy;
            tileMapBase = (LCDC.winTileMap()) ? 0x9C00 : 0x9800;
        } else {
            bgX = scx + x;
            bgY = scy + y;
        }

        uint16_t tileMapAddr = tileMapBase + ((bgY / 8) * 32) + (bgX / 8);
        int8_t tileIndex = ram.readByte(tileMapAddr);

        uint16_t tileAddr = (tileDataBase == 0x8000)
            ? tileDataBase + (tileIndex * 16)
            : tileDataBase + ((int8_t)tileIndex + 128) * 16;

        uint8_t line = bgY % 8;
        uint8_t data1 = ram.readByte(tileAddr + (line * 2));
        uint8_t data2 = ram.readByte(tileAddr + (line * 2) + 1);

        int colorBit = 7 - (bgX % 8);
        uint8_t colorId = ((data2 >> colorBit) & 1) << 1 | ((data1 >> colorBit) & 1);

        uint8_t palette = BGP;
        uint8_t shade = (palette >> (colorId * 2)) & 0x03;

        static const uint8_t shades[4] = {255, 192, 96, 0};
        framebuffer[y][x] = shades[shade];
    }
}


