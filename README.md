# Gamepad Example for Adafruit TinyUSB

I did not see a TinyUSB gamepad/joystick example so I wrote one. Be sure to set
the USB Stack to TinyUSB.

The example works on Adafruit SAMD (tested with an ItsyBitsy M0), Seeeduino
SAMD (tested on XIAO), and Adafruit nRF52840 (not tested but it compiles).

This produces a USB composite device with HID gamepad and CDC ACM port. An
ItsyBitsy M0 running this example works on an XAC so removing the CDC ACM port
is optional.

If the CDC ACM port must be removed, the changes are described
[here](https://github.com/adafruit/Adafruit_TinyUSB_ArduinoCore/issues/19).
Patching is the only option for now.

Similar patches are needed if using the Seeeduino SAMD or the Adafruit nRF52
board packages.
