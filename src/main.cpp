#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <cmath>
#include <FS.h>
#include <SparkFunMPU9250-DMP.h>
#include "Rainbow.h"
#include "WebIface.h"

const unsigned int n_pixels = 144;
WebIface webapp;
MPU9250_DMP imu;
float sine_table[361], cosine_table[361];

#ifdef ESP8266
const uint8_t pin_2812 = D4;
#else
const uint8_t pin_2812 = ;
#endif

Adafruit_NeoPixel strip(n_pixels, pin_2812);

uint32_t rainbowHSV(int angle, float brightness);
uint32_t rainbowSine(int angle, float brightness);
bool setupIMU();

void setup() 
{
  Serial.begin(115200);
  Wire.begin(D6, D7);

  for(unsigned int x = 0;x<360;++x)
  {
    sine_table[x] = sin(x*PI/360.);
    cosine_table[x] = cos(x*PI/360.);
  }
  Serial.println("Setting up WS2812B");
  strip.begin();
  strip.clear();
  strip.setBrightness(64);
  strip.show();

  Serial.println("Setting up SPIFFS");
  SPIFFS.begin();

  Serial.println("Setting up webserver");
  webapp.setup();
  Serial.println("Setting up IMU");
  setupIMU();
  Serial.println("Ready");
}

void loop() 
{
  webapp.handleClients();

  const int dtheta = webapp.getdTheta();
  const double harmonicfactor = webapp.getHarmonicFactor() * 360/72;
  static int theta = 0;
  if(theta>=360)
    theta = 0;
  if(theta<0)
    theta = 359;

  if(webapp.getAnimationMode() == WebIface::TWO_COLOURS)
  {
    WebIface::rgb8 rgb1 = webapp.getColour1();
    WebIface::rgb8 rgb2 = webapp.getColour2();
    for(uint16_t n = 0;n < strip.numPixels()/2;++n)
    {
      float f = 2.*PI*float(n + theta)/float(strip.numPixels()/2);
      f = (sin(f) + 1.) / 2.;
      f = max(f, 0.1f);
      uint32_t c = strip.Color(rgb1.r*f + rgb2.r*(1.-f), rgb1.g*f + rgb2.g*(1.-f), rgb1.b*f + rgb2.b*(1.-f));
      strip.setPixelColor(n, c);
      strip.setPixelColor(strip.numPixels() - n - 1, c);
    }
  } else if(webapp.getAnimationMode() == WebIface::RAINBOW) {
    for(uint16_t n = 0;n < strip.numPixels()/2;++n)
    {
      uint32_t c = rainbowHSV(theta + n * harmonicfactor, 1);
      strip.setPixelColor(n, c);
      strip.setPixelColor(strip.numPixels() - n - 1, c);
    }
  } else {
    if ( imu.fifoAvailable() )
    {
      // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
      if ( imu.dmpUpdateFifo() == INV_SUCCESS)
      {
        imu.computeEulerAngles(false);
        uint8_t r = 255.*fabs(imu.pitch*2./PI),
                g = 255.*fabs(imu.roll/PI),
                b = 255.*fabs(imu.yaw/PI);
        uint32_t c = strip.Color(r, g, b);
        Serial.println(String(r) + "/" + String(g) + "/" + String(b));
        for(uint16_t n = 0;n < strip.numPixels();++n)
        {
          strip.setPixelColor(n, c);
        }
      }
    }
  }
  strip.show();
  theta += dtheta;

  delay(webapp.getDelay_msec());
}



uint32_t rainbowHSV(int angle, float brightness)
{
  uint8_t red, green, blue;

  angle = angle % 360;
  if (angle<60) {red = 255; green = HSVlights[angle]; blue = 0;} else
  if (angle<120) {red = HSVlights[120-angle]; green = 255; blue = 0;} else 
  if (angle<180) {red = 0, green = 255; blue = HSVlights[angle-120];} else 
  if (angle<240) {red = 0, green = HSVlights[240-angle]; blue = 255;} else 
  if (angle<300) {red = HSVlights[angle-240], green = 0; blue = 255;} else 
                 {red = 255, green = 0; blue = HSVlights[360-angle];} 

  return Adafruit_NeoPixel::Color(float(red) * brightness,
                                  float(green) * brightness,
                                  float(blue) * brightness);
}

uint32_t rainbowSine(int angle, float brightness)
{
  return Adafruit_NeoPixel::Color(float(lights[(angle+120)%360]) * brightness,
                                  float(lights[angle])*brightness,
                                  float(lights[(angle+240)%360]) * brightness);
}

bool setupIMU()
{
  while(imu.begin() != INV_SUCCESS)
  {
    Serial.println("Unable to communicate with MPU-9250");
    delay(500);
  }
  imu.dmpBegin(DMP_FEATURE_LP_QUAT | // Enable low power 6-axis quat
               DMP_FEATURE_GYRO_CAL, // Use gyro calibration
              10); // Set DMP FIFO rate to 10 Hz
  return true;
}

float sineDeg(int x)
{
  x = x % 360;
  return sine_table[x];
}

extern "C" 
{
  // Needed for the DMP code
  int min(int a, int b)
  {
    return std::min(a, b);
  }
}

