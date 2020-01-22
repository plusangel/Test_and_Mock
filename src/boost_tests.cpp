//
// Created by angelos on 18/01/2020.
//

#define BOOST_TEST_MODULE AutoBrakeTest
#include "autoBrake.h"
#include <boost/test/included/unit_test.hpp>
#include <mockServiceBus.h>

struct AutoBrakeTest {
  MockServiceBus bus{};
  AutoBrake auto_brake{bus};
};

BOOST_FIXTURE_TEST_CASE(InitialCarSpeedIsZero, AutoBrakeTest) {
  BOOST_TEST(auto_brake.get_speed_ms() == 0);
}

BOOST_FIXTURE_TEST_CASE(InitialSensitivityIsFive, AutoBrakeTest) {
  BOOST_TEST(auto_brake.get_collision_threshold_s() == 5);
}

BOOST_FIXTURE_TEST_CASE(SensitivityGreaterThanOne, AutoBrakeTest) {
  BOOST_REQUIRE_THROW(auto_brake.set_collision_threshold_s(0.5L),
                      std::exception);
}

BOOST_FIXTURE_TEST_CASE(SpeedIsSaved, AutoBrakeTest) {
  bus.speed_update_callback(SpeedUpdate{100L});
  BOOST_TEST(100L == auto_brake.get_speed_ms());
  bus.speed_update_callback(SpeedUpdate{50L});
  BOOST_TEST(50L == auto_brake.get_speed_ms());
  bus.speed_update_callback(SpeedUpdate{0L});
  BOOST_TEST(0L == auto_brake.get_speed_ms());
}

BOOST_FIXTURE_TEST_CASE(AlertWhenImminent, AutoBrakeTest) {
  auto_brake.set_collision_threshold_s(10L);
  bus.speed_update_callback(SpeedUpdate{100L});
  bus.car_detected_callback(CarDetected{100L, 0L});
  BOOST_TEST(bus.command_published == 1);
  BOOST_TEST(bus.last_command.time_to_collision_s == 1);
}

BOOST_FIXTURE_TEST_CASE(NotAlertWhenNotImminent, AutoBrakeTest) {
  auto_brake.set_collision_threshold_s(2L);
  bus.speed_update_callback(SpeedUpdate{100L});
  bus.car_detected_callback(CarDetected{1000L, 50L});
  BOOST_TEST(bus.command_published == 0);
}