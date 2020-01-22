//
// Created by angelos on 22/01/2020.
//

#ifndef AUTOBRAKE_SERVICES_H
#define AUTOBRAKE_SERVICES_H

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

struct SpeedLimitDetected {
  unsigned short speed_mps;
};

#endif // AUTOBRAKE_SERVICES_H
