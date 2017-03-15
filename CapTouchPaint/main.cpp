/***************************************************
  This is our touchscreen painting example for the Adafruit ILI9341
  captouch shield
  ----> http://www.adafruit.com/products/1947

  Check out the links above for our tutorials and wiring diagrams

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <mbed.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>


// Pin defintions for connections to LCD breakout.
#define TFT_MOSI    p11
#define TFT_MISO    p12
#define TFT_SCLK    p13
#define TFT_CS      p14
#define TFT_DC      p15
#define TFT_RST     p16
#define TOUCH_SCL   p27
#define TOUCH_SDA   p28

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3

#define map(IN,IN_MIN,IN_MAX,OUT_MIN,OUT_MAX) \
            (((((IN)-(IN_MIN))*((OUT_MAX)-(OUT_MIN)))/((IN_MAX)-(IN_MIN)))+(OUT_MIN))
            

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST, TFT_MISO);
Adafruit_FT6206  ctp(TOUCH_SDA, TOUCH_SCL);

int oldcolor, currentcolor;


void setup(void)
{
    printf("Cap Touch Paint!\n");
    
    tft.begin();

    // pass in 'sensitivity' coefficient
    if (!ctp.begin(40))
    {  
        printf("Couldn't start FT6206 touchscreen controller\n");
        exit(-1);
    }
    
    printf("Capacitive touchscreen started\n");
    
    tft.fillScreen(ILI9341_BLACK);
    
    // make the color selection boxes
    tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
    tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
    tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN);
    tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN);
    tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE);
    tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);
    
    // select the current color 'red'
    tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
    currentcolor = ILI9341_RED;
}

void loop() 
{
    // Wait for a touch
    if (!ctp.touched())
    {
        return;
    }

    // Retrieve a point  
    TS_Point p = ctp.getPoint();
  
    // flip it around to match the screen.
    p.x = map(p.x, 0, 240, 240, 0);
    p.y = map(p.y, 0, 320, 320, 0);

    // Print out the remapped (rotated) coordinates
    printf("(%d, %d)\n", p.x, p.y);
  

    if (p.y < BOXSIZE) {
        oldcolor = currentcolor;
    
        if (p.x < BOXSIZE) { 
            currentcolor = ILI9341_RED; 
            tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
        } else if (p.x < BOXSIZE*2) {
            currentcolor = ILI9341_YELLOW;
            tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
        } else if (p.x < BOXSIZE*3) {
            currentcolor = ILI9341_GREEN;
            tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
        } else if (p.x < BOXSIZE*4) {
            currentcolor = ILI9341_CYAN;
            tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
        } else if (p.x < BOXSIZE*5) {
            currentcolor = ILI9341_BLUE;
            tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
        } else if (p.x <= BOXSIZE*6) {
            currentcolor = ILI9341_MAGENTA;
            tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
        }
    
        if (oldcolor != currentcolor) {
            if (oldcolor == ILI9341_RED) 
                tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
            if (oldcolor == ILI9341_YELLOW) 
                tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
            if (oldcolor == ILI9341_GREEN) 
                tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN);
            if (oldcolor == ILI9341_CYAN) 
                tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN);
            if (oldcolor == ILI9341_BLUE) 
                tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE);
            if (oldcolor == ILI9341_MAGENTA) 
                tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);
        }
    }
    if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height())) {
        tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
    }
}

int main(void)
{
    setup();
    while (1)
        loop();
}
