/*************************************************** 
  This is a library for the Adafruit Capacitive Touch Screens

  ----> http://www.adafruit.com/products/1947
 
  Check out the links above for our tutorials and wiring diagrams
  This chipset uses I2C to communicate

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include "Adafruit_FT6206.h"


/**************************************************************************/
/*! 
    @brief  Instantiates a new FT6206 class
*/
/**************************************************************************/
// I2C, no address adjustments or pins
Adafruit_FT6206::Adafruit_FT6206(PinName sda, PinName scl) :
    m_i2c(sda, scl)
{
}


/**************************************************************************/
/*! 
    @brief  Setups the HW
*/
/**************************************************************************/
bool Adafruit_FT6206::begin(uint8_t threshhold) 
{
    m_i2c.frequency(400000);

    // change threshhold to be higher/lower
    writeRegister8(FT6206_REG_THRESHHOLD, threshhold);
    
    if ((readRegister8(FT6206_REG_VENDID) != 17) || (readRegister8(FT6206_REG_CHIPID) != 6)) 
        return false;
    return true;
}

// DONT DO THIS - REALLY - IT DOESNT WORK
void Adafruit_FT6206::autoCalibrate(void) 
{
    writeRegister8(FT6206_REG_MODE, FT6206_REG_FACTORYMODE);
    wait_ms(100);
    //Serial.println("Calibrating...");
    writeRegister8(FT6206_REG_CALIBRATE, 4);
    wait_ms(300);
    for (uint8_t i = 0; i < 100; i++) 
    {
        uint8_t temp;
        temp = readRegister8(FT6206_REG_MODE);
        printf("%02X\n", temp);
        //return to normal mode, calibration finish 
        if (0x0 == ((temp & 0x70) >> 4))
            break;
    }
    wait_ms(200);
    //Serial.println("Calibrated");
    wait_ms(300);
    writeRegister8(FT6206_REG_MODE, FT6206_REG_FACTORYMODE);
    wait_ms(100);
    writeRegister8(FT6206_REG_CALIBRATE, 5);
    wait_ms(300);
    writeRegister8(FT6206_REG_MODE, FT6206_REG_WORKMODE);
    wait_ms(300);
}


bool Adafruit_FT6206::touched(void) 
{
    uint8_t n = readRegister8(FT6206_REG_NUMTOUCHES);
    if ((n == 1) || (n == 2)) 
        return true;
    return false;
}

/*****************************/

void Adafruit_FT6206::readData(uint16_t *x, uint16_t *y) 
{
    uint8_t i2cdat[16];

    // UNDONE: Some Adafruit magic here sending a 0.
    uint8_t reg = 0;
    bool    repeatedStart = true;
    m_i2c.write(FT6206_ADDR << 1, (const char*)&reg, sizeof(reg), repeatedStart);
    m_i2c.read(FT6206_ADDR << 1, (char*)i2cdat, sizeof(i2cdat));

    uint8_t touches = i2cdat[0x02];
    uint16_t touchX[2];
    uint16_t touchY[2];

    if (touches > 2) 
    {
        touches = 0;
    }
    if (touches == 0) 
    {
        *x = *y = 0;
        return;
    }

    for (uint8_t i=0; i<2; i++) 
    {
        touchX[i] = i2cdat[0x03 + i*6] & 0x0F;
        touchX[i] <<= 8;
        touchX[i] |= i2cdat[0x04 + i*6]; 
        touchY[i] = i2cdat[0x05 + i*6] & 0x0F;
        touchY[i] <<= 8;
        touchY[i] |= i2cdat[0x06 + i*6];
    }
    *x = touchX[0]; 
    *y = touchY[0];
}

TS_Point Adafruit_FT6206::getPoint(void) 
{
  uint16_t x, y;
  readData(&x, &y);
  return TS_Point(x, y, 1);
}


uint8_t Adafruit_FT6206::readRegister8(uint8_t reg) 
{
    uint8_t x ;

    bool repeatedStart = true;
    m_i2c.write(FT6206_ADDR << 1, (const char*)&reg, sizeof(reg), repeatedStart);
    m_i2c.read(FT6206_ADDR << 1, (char*)&x, sizeof(x));
    return x;
}

void Adafruit_FT6206::writeRegister8(uint8_t reg, uint8_t val) 
{
    char buffer[] = { reg, val };

    m_i2c.write(FT6206_ADDR << 1, buffer, sizeof(buffer));
}

/****************/

TS_Point::TS_Point(void) {
  x = y = 0;
}

TS_Point::TS_Point(int16_t x0, int16_t y0, int16_t z0) {
  x = x0;
  y = y0;
  z = z0;
}

bool TS_Point::operator==(TS_Point p1) {
  return  ((p1.x == x) && (p1.y == y) && (p1.z == z));
}

bool TS_Point::operator!=(TS_Point p1) {
  return  ((p1.x != x) || (p1.y != y) || (p1.z != z));
}
