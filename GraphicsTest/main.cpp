/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
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
#include <mbed.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>


#define TFT_MOSI    p11
#define TFT_MISO    p12
#define TFT_SCLK    p13
#define TFT_CS      p14
#define TFT_DC      p15
#define TFT_RST     p16

#define max(a,b) ((a) > (b) ? a : b)
#define min(a,b) ((a) < (b) ? a : b)


Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST, TFT_MISO);
Timer            g_timer;


unsigned long testFillScreen();
unsigned long testText();
unsigned long testLines(uint16_t color);
unsigned long testFastLines(uint16_t color1, uint16_t color2);
unsigned long testRects(uint16_t color);
unsigned long testFilledRects(uint16_t color1, uint16_t color2);
unsigned long testFilledCircles(uint8_t radius, uint16_t color);
unsigned long testCircles(uint8_t radius, uint16_t color);
unsigned long testTriangles();
unsigned long testFilledTriangles();
unsigned long testRoundRects();
unsigned long testFilledRoundRects();


void setup()
{
    printf("ILI9341 Test!\n");

    tft.begin();
    g_timer.start();

    // read diagnostics (optional but can help debug problems)
    uint8_t x = tft.readcommand8(ILI9341_RDMODE);
    printf("Display Power Mode: 0x%02X\n", x);
    x = tft.readcommand8(ILI9341_RDMADCTL);
    printf("MADCTL Mode: 0x%02X\n", x);
    x = tft.readcommand8(ILI9341_RDPIXFMT);
    printf("Pixel Format: 0x%02X\n", x);
    x = tft.readcommand8(ILI9341_RDIMGFMT);
    printf("Image Format: 0x%02X\n", x);
    x = tft.readcommand8(ILI9341_RDSELFDIAG);
    printf("Self Diagnostic: 0x%02X\n", x);

    printf("Benchmark                Time (microseconds)\n");
    wait_ms(10);
    printf("Screen fill              %ld\n", testFillScreen());
    wait_ms(500);

    printf("Text                     %ld\n", testText());
    wait_ms(3000);

    printf("Lines                    %ld\n", testLines(ILI9341_CYAN));
    wait_ms(500);

    printf("Horiz/Vert Lines         %ld\n", testFastLines(ILI9341_RED, ILI9341_BLUE));
    wait_ms(500);

    printf("Rectangles (outline)     %ld\n", testRects(ILI9341_GREEN));
    wait_ms(500);

    printf("Rectangles (filled)      %ld\n", testFilledRects(ILI9341_YELLOW, ILI9341_MAGENTA));
    wait_ms(500);

    printf("Circles (filled)         %ld\n", testFilledCircles(10, ILI9341_MAGENTA));

    printf("Circles (outline)        %ld\n", testCircles(10, ILI9341_WHITE));
    wait_ms(500);

    printf("Triangles (outline)      %ld\n", testTriangles());
    wait_ms(500);

    printf("Triangles (filled)       %ld\n", testFilledTriangles());
    wait_ms(500);

    printf("Rounded rects (outline)  %ld\n", testRoundRects());
    wait_ms(500);

    printf("Rounded rects (filled)   %ld\n", testFilledRoundRects());
    wait_ms(500);

    printf("Done!\n");
}


void loop(void) 
{
    for(uint8_t rotation=0; rotation<4; rotation++) 
    {
        tft.setRotation(rotation);
        testText();
        wait_ms(1000);
    }
}

int main(void)
{
    setup();
    while (1)
        loop();
}

static void yield()
{
}

unsigned long testFillScreen() 
{
    unsigned long start = g_timer.read_us();
    tft.fillScreen(ILI9341_BLACK);
    yield();
    tft.fillScreen(ILI9341_RED);
    yield();
    tft.fillScreen(ILI9341_GREEN);
    yield();
    tft.fillScreen(ILI9341_BLUE);
    yield();
    tft.fillScreen(ILI9341_BLACK);
    yield();
    return g_timer.read_us() - start;
}

unsigned long testText() 
{
    tft.fillScreen(ILI9341_BLACK);
    unsigned long start = g_timer.read_us();
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
    tft.print("Hello World!\n");
    tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
    tft.print("1234.56\n");
    tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
    tft.print("0xDEADBEEF\n");
    tft.print("\n");
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(5);
    tft.print("Groop\n");
    tft.setTextSize(2);
    tft.print("I implore thee,\n");
    tft.setTextSize(1);
    tft.print("my foonting turlingdromes.\n");
    tft.print("And hooptiously drangle me\n");
    tft.print("with crinkly bindlewurdles,\n");
    tft.print("Or I will rend thee\n");
    tft.print("in the gobberwarts\n");
    tft.print("with my blurglecruncheon,\n");
    tft.print("see if I don't!\n");
    return g_timer.read_us() - start;
}

unsigned long testLines(uint16_t color) 
{
    unsigned long start, t;
    int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();
    
    tft.fillScreen(ILI9341_BLACK);
    yield();
    
    x1    = y1 = 0;
    y2    = h - 1;
    start = g_timer.read_us();
    for(x2=0; x2<w; x2+=6) 
        tft.drawLine(x1, y1, x2, y2, color);
    x2    = w - 1;
    for(y2=0; y2<h; y2+=6) 
        tft.drawLine(x1, y1, x2, y2, color);
    t     = g_timer.read_us() - start; // fillScreen doesn't count against timing
    
    yield();
    tft.fillScreen(ILI9341_BLACK);
    yield();
    
    x1    = w - 1;
    y1    = 0;
    y2    = h - 1;
    start = g_timer.read_us();
    for(x2=0; x2<w; x2+=6) 
        tft.drawLine(x1, y1, x2, y2, color);
    x2    = 0;
    for(y2=0; y2<h; y2+=6) 
        tft.drawLine(x1, y1, x2, y2, color);
    t    += g_timer.read_us() - start;
    
    yield();
    tft.fillScreen(ILI9341_BLACK);
    yield();
    
    x1    = 0;
    y1    = h - 1;
    y2    = 0;
    start = g_timer.read_us();
    for(x2=0; x2<w; x2+=6) 
        tft.drawLine(x1, y1, x2, y2, color);
    x2    = w - 1;
    for(y2=0; y2<h; y2+=6) 
        tft.drawLine(x1, y1, x2, y2, color);
    t    += g_timer.read_us() - start;
    
    yield();
    tft.fillScreen(ILI9341_BLACK);
    yield();
    
    x1    = w - 1;
    y1    = h - 1;
    y2    = 0;
    start = g_timer.read_us();
    for(x2=0; x2<w; x2+=6) 
        tft.drawLine(x1, y1, x2, y2, color);
    x2    = 0;
    for(y2=0; y2<h; y2+=6) 
        tft.drawLine(x1, y1, x2, y2, color);
    
    yield();
    return g_timer.read_us() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) 
{
    unsigned long start;
    int           x, y, w = tft.width(), h = tft.height();
    
    tft.fillScreen(ILI9341_BLACK);
    start = g_timer.read_us();
    for(y=0; y<h; y+=5) 
        tft.drawFastHLine(0, y, w, color1);
    for(x=0; x<w; x+=5) 
        tft.drawFastVLine(x, 0, h, color2);
    
    return g_timer.read_us() - start;
}

unsigned long testRects(uint16_t color) 
{
    unsigned long start;
    int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;
    
    tft.fillScreen(ILI9341_BLACK);
    n     = min(tft.width(), tft.height());
    start = g_timer.read_us();
    for(i=2; i<n; i+=6) 
    {
        i2 = i / 2;
        tft.drawRect(cx-i2, cy-i2, i, i, color);
    }
    
    return g_timer.read_us() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) 
{
    unsigned long start, t = 0;
    int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;
    
    tft.fillScreen(ILI9341_BLACK);
    n = min(tft.width(), tft.height());
    for(i=n; i>0; i-=6) 
    {
        i2    = i / 2;
        start = g_timer.read_us();
        tft.fillRect(cx-i2, cy-i2, i, i, color1);
        t    += g_timer.read_us() - start;
        // Outlines are not included in timing results
        tft.drawRect(cx-i2, cy-i2, i, i, color2);
        yield();
    }
    
    return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) 
{
    unsigned long start;
    int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;
    
    tft.fillScreen(ILI9341_BLACK);
    start = g_timer.read_us();
    for(x=radius; x<w; x+=r2) 
    {
        for(y=radius; y<h; y+=r2) 
        {
            tft.fillCircle(x, y, radius, color);
        }
    }
    
    return g_timer.read_us() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) 
{
    unsigned long start;
    int           x, y, r2 = radius * 2,
            w = tft.width()  + radius,
            h = tft.height() + radius;
    
    // Screen is not cleared for this one -- this is
    // intentional and does not affect the reported time.
    start = g_timer.read_us();
    for(x=0; x<w; x+=r2) 
    {
        for(y=0; y<h; y+=r2) 
        {
            tft.drawCircle(x, y, radius, color);
        }
    }
    
    return g_timer.read_us() - start;
}

unsigned long testTriangles() 
{
    unsigned long start;
    int           n, i, cx = tft.width()  / 2 - 1,
                  cy = tft.height() / 2 - 1;
    
    tft.fillScreen(ILI9341_BLACK);
    n     = min(cx, cy);
    start = g_timer.read_us();
    for(i=0; i<n; i+=5) 
    {
        tft.drawTriangle(
                cx    , cy - i, // peak
                cx - i, cy + i, // bottom left
                cx + i, cy + i, // bottom right
                tft.color565(i, i, i));
    }
    
    return g_timer.read_us() - start;
}

unsigned long testFilledTriangles() 
{
    unsigned long start, t = 0;
    int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;
    
    tft.fillScreen(ILI9341_BLACK);
    start = g_timer.read_us();
    for(i=min(cx,cy); i>10; i-=5) 
    {
        start = g_timer.read_us();
        tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, tft.color565(0, i*10, i*10));
        t += g_timer.read_us() - start;
        tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, tft.color565(i*10, i*10, 0));
        yield();
    }
    
    return t;
}

unsigned long testRoundRects() 
{
    unsigned long start;
    int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;
    
    tft.fillScreen(ILI9341_BLACK);
    w     = min(tft.width(), tft.height());
    start = g_timer.read_us();
    for(i=0; i<w; i+=6) 
    {
        i2 = i / 2;
        tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
    }
    
    return g_timer.read_us() - start;
}

unsigned long testFilledRoundRects() 
{
    unsigned long start;
    int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;
    
    tft.fillScreen(ILI9341_BLACK);
    start = g_timer.read_us();
    for(i=min(tft.width(), tft.height()); i>20; i-=6) 
    {
        i2 = i / 2;
        tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
        yield();
    }
    
    return g_timer.read_us() - start;
}