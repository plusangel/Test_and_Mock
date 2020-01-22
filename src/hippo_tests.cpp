//
// Created by angelos on 21/01/2020.
//

#define CATCH_CONFIG_MAIN
#include "../Catch2/catch.hpp"
#include "autoBrakeCatch.h"
#include "hippomocks.h"

TEST_CASE("AutoBrake") {
  MockRepository mocks;
  mocks.autoExpect = false;

  SpeedUpdateCallback speed_update_callback;
  CarDetectedCallback car_detected_callback;

  auto *bus = mocks.Mock<IServiceBus2>();
  mocks.ExpectCall(bus, IServiceBus2::subscribe_to_speed)
      .Do([&](const auto &x) { speed_update_callback = x; });

  mocks.ExpectCall(bus, IServiceBus2::subscribe_to_car_detected)
      .Do([&](const auto &x) { car_detected_callback = x; });

  AutoBrake2 auto_brake{*bus};

  SECTION("initial car speed is zero") {
    REQUIRE(auto_brake.get_speed_ms() == Approx(0));
  }

  SECTION("initialises sensitivity to five") {
    REQUIRE(auto_brake.get_collision_threshold_s() == Approx(5));
  }

  SECTION("throws when sensitivity less than one") {
    REQUIRE_THROWS(auto_brake.set_collision_threshold_s(0.5L));
  }

  SECTION("saves speed after update") {
    speed_update_callback(SpeedUpdate{100L});
    REQUIRE(100L == auto_brake.get_speed_ms());
    speed_update_callback(SpeedUpdate{50L});
    REQUIRE(50L == auto_brake.get_speed_ms());
    speed_update_callback(SpeedUpdate{0L});
    REQUIRE(0L == auto_brake.get_speed_ms());
  }

  SECTION("alert when imminent") {
    mocks.ExpectCall(bus, IServiceBus2::publish).Match([](const auto &cmd) {
      return cmd.time_to_collision_s == Approx(1);
    });

    auto_brake.set_collision_threshold_s(10L);
    speed_update_callback(SpeedUpdate{100L});
    car_detected_callback(CarDetected{100L, 0L});
  }

  SECTION("no alert when not imminent") {
    auto_brake.set_collision_threshold_s(2L);
    speed_update_callback(SpeedUpdate{100L});
    car_detected_callback(CarDetected{1000L, 50L});
  }
}