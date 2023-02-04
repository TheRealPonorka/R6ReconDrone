#include "esp_camera.h"
#include <WiFi.h>


#include <FastLED.h>

#define NUM_LEDS 6 // how many LEDs are used
#define DATA_PIN 12 //define the proper LED PIN
#define COLOR_ORDER GRB //WS2812 color order is Green Red Blue
#define CHIPSET WS2812B // chipset of the LED strip is WS2812B
#define BRIGHTNESS 64 // limiting brightness to 25%
#define VOLTS 5 // LED strip voltage is 5V
#define MAX_AMPS 300 //limiting LED strip max current to 300mA
extern byte GREEN;
extern byte RED;
extern byte BLUE;
CRGB leds[NUM_LEDS];

//
// WARNING!!! PSRAM IC required for UXGA resolution and high JPEG quality
//            Ensure ESP32 Wrover Module or other board with PSRAM is selected
//            Partial images will be transmitted if image exceeds buffer size
//

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM

#include "camera_pins.h"

// SSID and password that are going to be used for the Access Point you will create -> DONT use the SSID/Password of your router:
const char* ssid = "R6_Drone_01";
const char* password = "Rainbow6";

// Configure IP addresses of the local access point
IPAddress local_IP(192,168,6,6);
IPAddress gateway(192,168,6,1);
IPAddress subnet(255,255,255,0);

// GPIO setting
extern int gpLb =  2; // Left Wheel Back
extern int gpLf = 14; // Left Wheel Forward
extern int gpRb = 15; // Right Wheel Back
extern int gpRf = 13; // Right Wheel Forward
extern int gpLed =  4; // Light
extern String WiFiAddr ="";

void startCameraServer();

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  pinMode(gpLb, OUTPUT); //Left Backward
  pinMode(gpLf, OUTPUT); //Left Forward
  pinMode(gpRb, OUTPUT); //Right Forward
  pinMode(gpRf, OUTPUT); //Right Backward
  pinMode(gpLed, OUTPUT); //Light

  //initialize
  digitalWrite(gpLb, LOW);
  digitalWrite(gpLf, LOW);
  digitalWrite(gpRb, LOW);
  digitalWrite(gpRf, LOW);
  digitalWrite(gpLed, LOW);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

  Serial.print("Setting up Access Point ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Starting Access Point ... ");
  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");

  Serial.print("IP address = ");
  Serial.println(WiFi.softAPIP());
  startCameraServer();

  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS,MAX_AMPS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  GREEN = 255;
  RED = 0;
  BLUE = 0;
  FastLED.show();
}



void loop() {
 leds[0] = CRGB::Black;
 leds[1] = CRGB::Black;
 leds[2] = CRGB::Black;
 leds[3] = CRGB::Black;
 leds[4] = CRGB::Black;
 leds[5] = CRGB::Black;
 FastLED.show();
 delay(0);

 leds[2] = CRGB(RED, GREEN, BLUE);
 leds[3] = CRGB(RED, GREEN, BLUE);
 FastLED.show();
 delay(75);

 leds[1] = CRGB(RED, GREEN, BLUE);
 leds[4] = CRGB(RED, GREEN, BLUE);
 FastLED.show();
 delay(75);

 leds[0] = CRGB(RED, GREEN, BLUE);
 leds[5] = CRGB(RED, GREEN, BLUE);
 FastLED.show();
 delay(500);

 leds[0] = CRGB::Black;
 leds[5] = CRGB::Black;
 FastLED.show();
 delay(75);

 leds[1] = CRGB::Black;
 leds[4] = CRGB::Black;
 FastLED.show();
 delay(75);

 leds[2] = CRGB::Black;
 leds[3] = CRGB::Black;
 FastLED.show();
 delay(75);
}
