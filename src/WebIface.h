#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
typedef ESP8266WebServer WebServer;
#else
#include <WiFi.h>
#include <WebServer.h>
#endif
#include <IPAddress.h>
#include "Adafruit_NeoPixel.h"

class WebIface
{
    public:
    typedef enum 
    {
        RAINBOW,
        TWO_COLOURS,
        IMU
    } animation_mode_t;
    class rgb8
    {
        public:
        rgb8(uint8_t r_=0, uint8_t g_=0, uint8_t b_=0) : r(r_), g(g_), b(b_) {}
        uint8_t r, g, b;
    };

    protected:
    unsigned int delay_msec = 10;
    int dTheta = 1;
    double harmonicfactor = 1;
    rgb8 rgb1, rgb2;
    animation_mode_t animation_mode = WebIface::RAINBOW;
    //IPAddress ip(10, 1, 1, 1);
    WebServer server;//(ip, 80);

    public:
    bool setup();
    bool handleClients();

    unsigned int getDelay_msec() const { return delay_msec; }
    int getdTheta() const { return dTheta; }
    double getHarmonicFactor() const { return harmonicfactor; }
    animation_mode_t getAnimationMode() const { return animation_mode; }
    rgb8 getColour1() const { return rgb1; }
    rgb8 getColour2() const { return rgb2; }

    protected:
    void handleRoot();
    void setColour1();
    void setColour2();
    void setDelay_msec();
    void setdTheta();
    void setHarmonicFactor();
    void setAnimationMode();

    void send200();
};

#endif
