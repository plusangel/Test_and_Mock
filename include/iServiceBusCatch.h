//
// Created by angelos on 17/01/2020.
//

#ifndef AUTOBRAKE_ISERVICEBUS_H
#define AUTOBRAKE_ISERVICEBUS_H
#include <functional>
#include "services.h"

using SpeedUpdateCallback = std::function<void(const SpeedUpdate &)>;
using CarDetectedCallback = std::function<void(const CarDetected &)>;

struct IServiceBus2 {
  virtual ~IServiceBus2() = default;
  virtual void publish(const BrakeCommand &) = 0;
  virtual void subscribe_to_speed(SpeedUpdateCallback) = 0;
  virtual void subscribe_to_car_detected(CarDetectedCallback) = 0;
};

#endif // AUTOBRAKE_ISERVICEBUS_H
