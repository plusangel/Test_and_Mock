//
// Created by angelos on 12/01/2020.
//

#ifndef AUTOBRAKE_AUTOBRAKE_H
#define AUTOBRAKE_AUTOBRAKE_H

#include "iServiceBus.h"
#include <stdexcept>

struct AutoBrake {
  explicit AutoBrake(IServiceBus &bus) : speed_mps{}, collision_threshold_s{5} {

    bus.subscribe(
        [this](const SpeedUpdate &update) { speed_mps = update.velocity_mps; });

    bus.subscribe([this, &bus](const CarDetected &cd) {
      const auto relative_velocity_mps = speed_mps - cd.velocity_mps;
      const auto time_to_collision_s = cd.distance_m / relative_velocity_mps;
      if (time_to_collision_s > 0 && time_to_collision_s <= collision_threshold_s) {
        bus.publish(BrakeCommand{time_to_collision_s});
      }
    });
  }

  [[nodiscard]] double get_collision_threshold_s() const {
    return collision_threshold_s;
  }
  [[nodiscard]] double get_speed_ms() const { return speed_mps; }

  void set_collision_threshold_s(double x) {
    if (x < 1)
      throw std::runtime_error{"Collision less than 1."};
    collision_threshold_s = x;
  }

private:
  double collision_threshold_s;
  double speed_mps;
};

#endif // AUTOBRAKE_AUTOBRAKE_H
