//
// Created by angelos on 17/01/2020.
//

#ifndef AUTOBRAKE_ISERVICEBUS_H
#define AUTOBRAKE_ISERVICEBUS_H
#include <functional>
#include "services.h"

using SpeedUpdateCallback = std::function<void(const SpeedUpdate &)>;
using CarDetectedCallback = std::function<void(const CarDetected &)>;
using SpeedLimitCallBack = std::function<void(const SpeedLimitDetected &)>;

struct IServiceBus {
  virtual ~IServiceBus() = default;
  virtual void publish(const BrakeCommand &) = 0;
  virtual void subscribe(SpeedUpdateCallback) = 0;
  virtual void subscribe(CarDetectedCallback) = 0;
  virtual void subscribe(SpeedLimitCallBack) = 0;
};

#endif // AUTOBRAKE_ISERVICEBUS_H
