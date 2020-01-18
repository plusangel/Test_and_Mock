//
// Created by angelos on 17/01/2020.
//

#ifndef AUTOBRAKE_ISERVICEBUS_H
#define AUTOBRAKE_ISERVICEBUS_H
#include <functional>

struct SpeedUpdate {
  double velocity_mps;
};

struct CarDetected {
  double distance_m;
  double velocity_mps;
};

struct BrakeCommand {
  double time_to_collision_s;
};

using SpeedUpdateCallback = std::function<void(const SpeedUpdate &)>;
using CarDetectedCallback = std::function<void(const CarDetected &)>;

struct IServiceBus {
  virtual ~IServiceBus() = default;
  virtual void publish(const BrakeCommand &) = 0;
  virtual void subscribe(SpeedUpdateCallback) = 0;
  virtual void subscribe(CarDetectedCallback) = 0;
};

#endif // AUTOBRAKE_ISERVICEBUS_H
