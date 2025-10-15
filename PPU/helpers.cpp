#include "ppu.hpp"

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
        case 0xFF41: STAT =  break; // dont exactly know how to implement this
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