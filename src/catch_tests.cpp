//
// Created by angelos on 17/01/2020.
//
#define CATCH_CONFIG_MAIN
#include "../Catch2/catch.hpp"
#include "autoBrake.h"
#include <mockServiceBus.h>

TEST_CASE("AutoBrake") {
  MockServiceBus bus{};
  AutoBrake auto_brake{bus};

  SECTION("initial car speed is zero") {
    REQUIRE(auto_brake.get_speed_ms() == Approx(0));
  }

  SECTION("initialises sensitivity to five") {
    REQUIRE(auto_brake.get_collision_threshold_s() == Approx(5));
  }

  SECTION("throws when sensitivity less than one") {
    REQUIRE_THROWS(auto_brake.set_collision_threshold_s(0.5L));
  }

  SECTION("initialises last known speed limit to 39") {
    REQUIRE(auto_brake.get_last_known_speed_limit() == 39);
  }

  SECTION("saves speed after update") {
    bus.speed_update_callback(SpeedUpdate{100L});
    REQUIRE(100L == auto_brake.get_speed_ms());
    bus.speed_update_callback(SpeedUpdate{50L});
    REQUIRE(50L == auto_brake.get_speed_ms());
    bus.speed_update_callback(SpeedUpdate{0L});
    REQUIRE(0L == auto_brake.get_speed_ms());
  }

  SECTION("saves speed limits after update") {
    bus.speed_limit_callback(SpeedLimitDetected{100L});
    REQUIRE(100L == auto_brake.get_last_known_speed_limit());
    bus.speed_limit_callback(SpeedLimitDetected{50L});
    REQUIRE(50L == auto_brake.get_last_known_speed_limit());
    bus.speed_limit_callback(SpeedLimitDetected{0L});
    REQUIRE(0L == auto_brake.get_last_known_speed_limit());
  }

  SECTION("alert when imminent") {
    auto_brake.set_collision_threshold_s(10L);
    bus.speed_update_callback(SpeedUpdate{100L});
    bus.car_detected_callback(CarDetected{100L, 0L});
    REQUIRE(bus.command_published == 1);
    REQUIRE(bus.last_command.time_to_collision_s == Approx(1));
  }

  SECTION("no alert when not imminent") {
    auto_brake.set_collision_threshold_s(2L);
    bus.speed_update_callback(SpeedUpdate{100L});
    bus.car_detected_callback(CarDetected{1000L, 50L});
    REQUIRE(bus.command_published == 0);
  }
}
