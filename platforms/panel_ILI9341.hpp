#ifndef LGFX_PANEL_ILI9341_HPP_
#define LGFX_PANEL_ILI9341_HPP_

#include "panel_lcd_common.hpp"

namespace lgfx
{
  struct Panel_ILI9341_COMMON : public PanelLcdCommon
  {
    static constexpr uint8_t SPI_MODE = 0;
    static constexpr bool HAS_OFFSET = false;

    static constexpr uint8_t LEN_CMD = 8;
    static constexpr uint8_t LEN_PIXEL_READ = 24;
    static constexpr uint8_t LEN_DUMMY_READ_PIXEL = 8;
    static constexpr uint8_t LEN_DUMMY_READ_RDDID = 0;
  //  static constexpr uint8_t LEN_PIXEL_WRITE = (_bpp > 16) ? 24 : 16;
  //  static constexpr uint8_t LEN_R_BITS = (_bpp > 16) ? 6 : 5;
  //  static constexpr uint8_t LEN_G_BITS = 6;
  //  static constexpr uint8_t LEN_B_BITS = (_bpp > 16) ? 6 : 5;

    struct CMD : public CommandCommon
    {
      static constexpr uint8_t FRMCTR1 = 0xB1;
      static constexpr uint8_t FRMCTR2 = 0xB2;
      static constexpr uint8_t FRMCTR3 = 0xB3;
      static constexpr uint8_t INVCTR  = 0xB4;
      static constexpr uint8_t DFUNCTR = 0xB6;
      static constexpr uint8_t PWCTR1  = 0xC0;
      static constexpr uint8_t PWCTR2  = 0xC1;
      static constexpr uint8_t PWCTR3  = 0xC2;
      static constexpr uint8_t PWCTR4  = 0xC3;
      static constexpr uint8_t PWCTR5  = 0xC4;
      static constexpr uint8_t VMCTR1  = 0xC5;
      static constexpr uint8_t VMCTR2  = 0xC7;
      static constexpr uint8_t GMCTRP1 = 0xE0;
      static constexpr uint8_t GMCTRN1 = 0xE1;

      static constexpr uint8_t RDINDEX = 0xD9; // ili9341
      static constexpr uint8_t IDXRD   = 0xDD; // ILI9341 only, indexed control register read
    };
    enum MAD
    { MY  = 0x80
    , MX  = 0x40
    , MV  = 0x20
    , ML  = 0x10
    , BGR = 0x08
    , MH  = 0x04
    , RGB = 0x00
    };
    enum PIX
    { RGB565_2BYTE = 0x55
    , RGB666_3BYTE = 0x66
    };

    static const uint8_t* getInitCommands(uint8_t listno = 0) {
      static constexpr uint8_t list0[] = {
          0xEF       , 3, 0x03,0x80,0x02,
          0xCF       , 3, 0x00,0xC1,0x30,
          0xED       , 4, 0x64,0x03,0x12,0x81,
          0xE8       , 3, 0x85,0x00,0x78,
          0xCB       , 5, 0x39,0x2C,0x00,0x34,0x02,
          0xF7       , 1, 0x20,
          0xEA       , 2, 0x00,0x00,
          CMD::PWCTR1, 1, 0x23,
          CMD::PWCTR2, 1, 0x10,
          CMD::VMCTR1, 2, 0x3e,0x28,
          CMD::VMCTR2, 1, 0x86,
  //      CMD::MADCTL, 1, 0xA8,
  //      CMD::PIXSET, 1, (_bpp > 16) ? PIX::RGB666_3BYTE : PIX::RGB565_2BYTE,
          CMD::FRMCTR1,2, 0x00,0x18,
          CMD::DFUNCTR,3, 0x08,0x82,0x27,
          0xF2       , 1, 0x00,
          CMD::GAMMASET,1, 0x01,  // Gamma set, curve 1
          CMD::GMCTRP1,15, 0x0F,0x31,0x2B,0x0C,0x0E,0x08,0x4E,0xF1,0x37,0x07,0x10,0x03,0x0E,0x09,0x00,
          CMD::GMCTRN1,15, 0x00,0x0E,0x14,0x03,0x11,0x07,0x31,0xC1,0x48,0x08,0x0F,0x0C,0x31,0x36,0x0F,
          CMD::SLPOUT, 0,
          0xFF,0xFF, // end
      };
      static constexpr uint8_t list1[] = {
          CMD::DISPON, 0,
          0xFF,0xFF, // end
      };
      switch (listno) {
      case 0: return list0;
      case 1: return list1;
      default: return nullptr;
      }
    }

    inline static uint8_t getAdjustBpp(uint8_t bpp) { return (bpp > 17 ) ? 24 : 16; }
    inline static PIX getPixset(uint8_t bpp) { return (bpp > 16) ? RGB666_3BYTE : RGB565_2BYTE; }
  };

  struct Panel_M5Stack : public Panel_ILI9341_COMMON
  {
    static constexpr int16_t PANEL_WIDTH = 240;
    static constexpr int16_t PANEL_HEIGHT = 320;
    static const rotation_data_t* getRotationData(uint8_t r) {
      static rotation_data_t res = {0,0,0};
      r = r & 7;
      static constexpr uint8_t madctl_table[] = {
        MAD::MV|        MAD::MY|MAD::BGR,
                                MAD::BGR,
        MAD::MV|MAD::MX|        MAD::BGR,
                MAD::MX|MAD::MY|MAD::BGR,
        MAD::MV|MAD::MX|MAD::MY|MAD::BGR,
                        MAD::MY|MAD::BGR,
        MAD::MV|                MAD::BGR,
                MAD::MX|        MAD::BGR,
      };
      res.madctl = madctl_table[r];
      return &res;
    }
  };

  struct Panel_ILI9341_240x320 : public Panel_ILI9341_COMMON
  { // ESP-WROVER-KIT ILI9341
    static constexpr int16_t PANEL_WIDTH = 240;
    static constexpr int16_t PANEL_HEIGHT = 320;
    static const rotation_data_t* getRotationData(uint8_t r) {
      static rotation_data_t res = {0,0,0};
      r = r & 7;
      static constexpr uint8_t madctl_table[] = {
                MAD::MX|        MAD::BGR,
        MAD::MV|                MAD::BGR,
                        MAD::MY|MAD::BGR,
        MAD::MV|MAD::MX|MAD::MY|MAD::BGR,
                MAD::MX|MAD::MY|MAD::BGR,
        MAD::MV|MAD::MX|        MAD::BGR,
                                MAD::BGR,
        MAD::MV|        MAD::MY|MAD::BGR,
      };
      res.madctl = madctl_table[r];
      return &res;
    }

  };
}

#endif
