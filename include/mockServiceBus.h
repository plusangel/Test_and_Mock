//
// Created by angelos on 17/01/2020.
//
#include "iServiceBus.h"

struct MockServiceBus : IServiceBus {
  void publish(const BrakeCommand &cmd) override {
    command_published++;
    last_command = cmd;
  }

  void subscribe(SpeedUpdateCallback callback) override {
    speed_update_callback = callback;
  }

  void subscribe(CarDetectedCallback callback) override {
    car_detected_callback = callback;
  }

  void subscribe(SpeedLimitCallBack callback) override {
    speed_limit_callback = callback;
  }

  BrakeCommand last_command{};
  int command_published{};

  SpeedUpdateCallback speed_update_callback{};
  CarDetectedCallback car_detected_callback{};
  SpeedLimitCallBack speed_limit_callback{};
};