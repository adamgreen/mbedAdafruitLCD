/***************************************************
  This is our library for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include "Adafruit_ILI9341.h"


// Constructor
Adafruit_ILI9341::Adafruit_ILI9341(PinName cs, PinName dc, PinName mosi, PinName sclk, PinName rst, PinName miso)
    : Adafruit_GFX(ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT),
      m_spi(mosi, miso, sclk),
      m_csPin(cs),
      m_dcPin(dc),
      m_rstPin(rst)
{
}


void Adafruit_ILI9341::spiwrite(uint8_t c)
{
    m_spi.write(c);
}


void Adafruit_ILI9341::writecommand(uint8_t c)
{
    m_dcPin = 0;
    m_csPin = 0;

    spiwrite(c);

    m_csPin = 1;
}


void Adafruit_ILI9341::writedata(uint8_t c)
{
    m_dcPin = 1;
    m_csPin = 0;

    spiwrite(c);

    m_csPin = 1;
}


void Adafruit_ILI9341::begin(void)
{
    // Configure SPI peripheral.
    int bitsPerSPIFrame = 8;
    int spiMode = 0;
    m_spi.format(bitsPerSPIFrame, spiMode);

    // UNDONE: mbed LPC1768 can run SPI at 48MHz maximum so could try that out to see what happens.
    int spiFrequency = 24000000;
    m_spi.frequency(spiFrequency);

    // Toggle RST low to reset display.
    m_rstPin = 1;
    wait_ms(5);
    m_rstPin = 0;
    wait_ms(20);
    m_rstPin = 1;
    wait_ms(150);

    // UNDONE: Lots of magic numbers here from Adafruit.
    writecommand(0xEF);
    writedata(0x03);
    writedata(0x80);
    writedata(0x02);

    writecommand(0xCF);
    writedata(0x00);
    writedata(0XC1);
    writedata(0X30);

    writecommand(0xED);
    writedata(0x64);
    writedata(0x03);
    writedata(0X12);
    writedata(0X81);

    writecommand(0xE8);
    writedata(0x85);
    writedata(0x00);
    writedata(0x78);

    writecommand(0xCB);
    writedata(0x39);
    writedata(0x2C);
    writedata(0x00);
    writedata(0x34);
    writedata(0x02);

    writecommand(0xF7);
    writedata(0x20);

    writecommand(0xEA);
    writedata(0x00);
    writedata(0x00);

    writecommand(ILI9341_PWCTR1);    //Power control
    writedata(0x23);   //VRH[5:0]

    writecommand(ILI9341_PWCTR2);    //Power control
    writedata(0x10);   //SAP[2:0];BT[3:0]

    writecommand(ILI9341_VMCTR1);    //VCM control
    writedata(0x3e); //¶Ô±È¶Èµ÷½Ú
    writedata(0x28);

    writecommand(ILI9341_VMCTR2);    //VCM control2
    writedata(0x86);  //--

    writecommand(ILI9341_MADCTL);    // Memory Access Control
    writedata(0x48);

    writecommand(ILI9341_PIXFMT);
    writedata(0x55);

    writecommand(ILI9341_FRMCTR1);
    writedata(0x00);
    writedata(0x18);

    writecommand(ILI9341_DFUNCTR);    // Display Function Control
    writedata(0x08);
    writedata(0x82);
    writedata(0x27);

    writecommand(0xF2);    // 3Gamma Function Disable
    writedata(0x00);

    writecommand(ILI9341_GAMMASET);    //Gamma curve selected
    writedata(0x01);

    writecommand(ILI9341_GMCTRP1);    //Set Gamma
    writedata(0x0F);
    writedata(0x31);
    writedata(0x2B);
    writedata(0x0C);
    writedata(0x0E);
    writedata(0x08);
    writedata(0x4E);
    writedata(0xF1);
    writedata(0x37);
    writedata(0x07);
    writedata(0x10);
    writedata(0x03);
    writedata(0x0E);
    writedata(0x09);
    writedata(0x00);

    writecommand(ILI9341_GMCTRN1);    //Set Gamma
    writedata(0x00);
    writedata(0x0E);
    writedata(0x14);
    writedata(0x03);
    writedata(0x11);
    writedata(0x07);
    writedata(0x31);
    writedata(0xC1);
    writedata(0x48);
    writedata(0x08);
    writedata(0x0F);
    writedata(0x0C);
    writedata(0x31);
    writedata(0x36);
    writedata(0x0F);

    writecommand(ILI9341_SLPOUT);    //Exit Sleep

    wait_ms(120);
    writecommand(ILI9341_DISPON);    //Display on

    // Finish initializing the LCD by setting the rotation.
    setRotation(rotation);
}


void Adafruit_ILI9341::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    writecommand(ILI9341_CASET); // Column addr set
    writedata(x0 >> 8);
    writedata(x0 & 0xFF);     // XSTART
    writedata(x1 >> 8);
    writedata(x1 & 0xFF);     // XEND

    writecommand(ILI9341_PASET); // Row addr set
    writedata(y0>>8);
    writedata(y0);     // YSTART
    writedata(y1>>8);
    writedata(y1);     // YEND

    writecommand(ILI9341_RAMWR); // write to RAM
}


void Adafruit_ILI9341::pushColor(uint16_t color)
{
    m_dcPin = 1;
    m_csPin = 0;

    spiwrite(color >> 8);
    spiwrite(color);

    m_csPin = 1;
}

void Adafruit_ILI9341::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height))
        return;

    setAddrWindow(x,y,x+1,y+1);

    m_dcPin = 1;
    m_csPin = 0;

    spiwrite(color >> 8);
    spiwrite(color);

    m_csPin = 1;
}


void Adafruit_ILI9341::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    // Rudimentary clipping
    if((x >= _width) || (y >= _height))
        return;

    if((y+h-1) >= _height)
        h = _height-y;

    setAddrWindow(x, y, x, y+h-1);
    
    uint8_t hi = color >> 8;
    uint8_t lo = color;

    m_dcPin = 1;
    m_csPin = 0;

    while (h--)
    {
        spiwrite(hi);
        spiwrite(lo);
    }

    m_csPin = 0;
}


void Adafruit_ILI9341::drawFastHLine(int16_t x, int16_t y, int16_t w,  uint16_t color)
{
    // Rudimentary clipping
    if((x >= _width) || (y >= _height))
        return;
    if((x+w-1) >= _width)
        w = _width-x;

    setAddrWindow(x, y, x+w-1, y);

    uint8_t hi = color >> 8;
    uint8_t lo = color;

    m_dcPin = 1;
    m_csPin = 0;

    while (w--)
    {
        spiwrite(hi);
        spiwrite(lo);
    }

    m_csPin = 1;
}

void Adafruit_ILI9341::fillScreen(uint16_t color)
{
    fillRect(0, 0,  _width, _height, color);
}

// fill a rectangle
void Adafruit_ILI9341::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    // rudimentary clipping (drawChar w/big text requires this)
    if((x >= _width) || (y >= _height))
        return;
    if((x + w - 1) >= _width)
        w = _width  - x;
    if((y + h - 1) >= _height)
        h = _height - y;

    setAddrWindow(x, y, x+w-1, y+h-1);

    uint8_t hi = color >> 8;
    uint8_t lo = color;

    m_dcPin = 1;
    m_csPin = 0;

    for(y=h; y>0; y--)
    {
        for(x=w; x>0; x--)
        {
            spiwrite(hi);
            spiwrite(lo);
        }
    }

    m_csPin = 1;
}


// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Adafruit_ILI9341::color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}


#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

void Adafruit_ILI9341::setRotation(uint8_t m)
{
    writecommand(ILI9341_MADCTL);
    rotation = m % 4; // can't be higher than 3
    switch (rotation)
    {
    case 0:
        writedata(MADCTL_MX | MADCTL_BGR);
        _width  = ILI9341_TFTWIDTH;
        _height = ILI9341_TFTHEIGHT;
        break;
    case 1:
        writedata(MADCTL_MV | MADCTL_BGR);
        _width  = ILI9341_TFTHEIGHT;
        _height = ILI9341_TFTWIDTH;
        break;
    case 2:
        writedata(MADCTL_MY | MADCTL_BGR);
        _width  = ILI9341_TFTWIDTH;
        _height = ILI9341_TFTHEIGHT;
        break;
    case 3:
        writedata(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
        _width  = ILI9341_TFTHEIGHT;
        _height = ILI9341_TFTWIDTH;
        break;
    }
}


void Adafruit_ILI9341::invertDisplay(bool i)
{
    writecommand(i ? ILI9341_INVON : ILI9341_INVOFF);
}


////////// stuff not actively being used, but kept for posterity


uint8_t Adafruit_ILI9341::spiread(void)
{
    return m_spi.write(0x00);
}

uint8_t Adafruit_ILI9341::readdata(void)
{
    m_dcPin = 1;
    m_csPin = 0;

    uint8_t data = spiread();

    m_csPin = 1;

    return data;
}


uint8_t Adafruit_ILI9341::readcommand8(uint8_t c, uint8_t index)
{
    // woo sekret command?
    m_dcPin = 0;
    m_csPin = 0;
    spiwrite(0xD9);
    
    // data
    m_dcPin = 1;
    spiwrite(0x10 + index);
    m_csPin = 1;
    
    m_dcPin = 0;
    m_csPin = 0;
    spiwrite(c);
    
    m_dcPin = 1;
    uint8_t data = spiread();
    m_csPin = 1;
    
    return data;
}
