/************************************************************************
MIT License

Copyright (c) 2021 gdsports625@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*************************************************************************/
#include "Adafruit_TinyUSB.h"

#define ATTRIBUTE_PACKED  __attribute__((packed, aligned(1)))

typedef struct ATTRIBUTE_PACKED {
    uint16_t buttons;
    int8_t	leftXAxis;
    int8_t	leftYAxis;
    int8_t	rightXAxis;
    int8_t	rightYAxis;
} HID_GamepadReport_Data_t;

// HID report descriptor using TinyUSB's template
// Single Report (no ID) descriptor
uint8_t const desc_hid_report[] =
{
  TUD_HID_REPORT_DESC_GAMEPAD()
};


class TUGamepad {
  public:
    inline TUGamepad(void);

    inline void begin(void);
    inline void end(void);
    inline void loop(void);
    inline void write(void);
    inline void write(void *report);
    inline void press(uint8_t b);
    inline void release(uint8_t b);
    inline void releaseAll(void);

    inline void buttons(uint16_t b);
    inline void leftXAxis(int8_t a);
    inline void leftYAxis(int8_t a);
    inline void rightXAxis(int8_t a);
    inline void rightYAxis(int8_t a);
    inline bool ready(void) { return this->usb_hid.ready(); };

    // Sending is public for advanced users.
    inline bool SendReport(void* data, size_t length) {
        return this->usb_hid.sendReport(0, data, (uint8_t)length);
    };

  protected:
    HID_GamepadReport_Data_t _report;
    uint32_t startMillis;
    Adafruit_USBD_HID usb_hid;
};

TUGamepad::TUGamepad(void)
{
  this->usb_hid.setPollInterval(1);
  this->usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  // setStringDescriptor is undefined on nRF52840
  // this->usb_hid.setStringDescriptor("TinyUSB Gamepad");
}

void TUGamepad::begin(void)
{
  this->usb_hid.begin();

  // release all buttons, center all sticks, etc.
  end();
  startMillis = millis();
}

void TUGamepad::loop(void)
{
  if (startMillis != millis()) {
    write();
    startMillis = millis();
  }
}

void TUGamepad::end(void)
{
  memset(&_report, 0x00, sizeof(_report));
  SendReport(&_report, sizeof(_report));
}

void TUGamepad::write(void)
{
  SendReport(&_report, sizeof(_report));
}

void TUGamepad::write(void *report)
{
  memcpy(&_report, report, sizeof(_report));
  SendReport(&_report, sizeof(_report));
}

void TUGamepad::press(uint8_t b)
{
  b &= 0xF; // Limit value between 0..15
  _report.buttons |= (uint16_t)1 << b;
}


void TUGamepad::release(uint8_t b)
{
  b &= 0xF; // Limit value between 0..15
  _report.buttons &= ~((uint16_t)1 << b);
}


void TUGamepad::releaseAll(void)
{
  _report.buttons = 0;
}

void TUGamepad::buttons(uint16_t b)
{
  _report.buttons = b;
}


void TUGamepad::leftXAxis(int8_t a)
{
  _report.leftXAxis = a;
}


void TUGamepad::leftYAxis(int8_t a)
{
  _report.leftYAxis = a;
}


void TUGamepad::rightXAxis(int8_t a)
{
  _report.rightXAxis = a;
}


void TUGamepad::rightYAxis(int8_t a)
{
  _report.rightYAxis = a;
}


TUGamepad Gamepad;

void setup()
{
  Gamepad.begin();

  // wait until device mounted
  while( !USBDevice.mounted() ) delay(1);
}


void loop()
{
  if ( !Gamepad.ready() ) return;

  static uint8_t count = 0;
  // 16 buttons
  if (count > 15) {
    Gamepad.releaseAll();
    count = 0;
  }
  Gamepad.press(count);
  count++;

  // Move x/y Axis to a random position
  Gamepad.leftXAxis(random(256));
  Gamepad.leftYAxis(random(256));
  Gamepad.rightXAxis(random(256));
  Gamepad.rightYAxis(random(256));

  // Functions above only set the values.
  // This writes the report to the host.
  Gamepad.loop();
  delay(100);
}
