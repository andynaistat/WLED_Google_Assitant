#ifndef _LEDS_H_
#define _LEDS_H_

#include <SinricProDevice.h>
#include <Capabilities/PowerStateController.h>
#include <Capabilities/ModeController.h>
#include <Capabilities/ColorController.h>

class Leds 
: public SinricProDevice
, public PowerStateController<Leds>
, public ModeController<Leds>
, public ColorController<Leds> {
  friend class PowerStateController<Leds>;
  friend class ModeController<Leds>;
  friend class ColorController<Leds>;
public:
  Leds(const String &deviceId) : SinricProDevice(deviceId, "Leds") {};
};

#endif
