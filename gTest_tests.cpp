//
// Created by angelos on 18/01/2020.
//

#include "autoBrake.h"
#include "gtest/gtest.h"
#include <mockServiceBus.h>

struct AutoBrakeTest : ::testing::Test {
  MockServiceBus bus{};
  AutoBrake auto_brake{bus};
};

TEST_F(AutoBrakeTest, InitialCarSpeedIsZero) {
  ASSERT_DOUBLE_EQ(0, auto_brake.get_speed_ms());
}

TEST_F(AutoBrakeTest, InitialSensitivityIsFive) {
  ASSERT_DOUBLE_EQ(5, auto_brake.get_collision_threshold_s());
}

TEST_F(AutoBrakeTest, SensitivityGreaterThanOne) {
  ASSERT_ANY_THROW(auto_brake.set_collision_threshold_s(0.5L));
}

TEST_F(AutoBrakeTest, SpeedIsSaved) {
  bus.speed_update_callback(SpeedUpdate{100L});
  ASSERT_EQ(100L, auto_brake.get_speed_ms());
  bus.speed_update_callback(SpeedUpdate{50L});
  ASSERT_EQ(50L, auto_brake.get_speed_ms());
  bus.speed_update_callback(SpeedUpdate{0L});
  ASSERT_DOUBLE_EQ(0L, auto_brake.get_speed_ms());
}

TEST_F(AutoBrakeTest, AlertWhenImminent) {
  auto_brake.set_collision_threshold_s(10L);
  bus.speed_update_callback(SpeedUpdate{100L});
  bus.car_detected_callback(CarDetected{100L, 0L});
  ASSERT_EQ(bus.command_published, 1);
  ASSERT_DOUBLE_EQ(bus.last_command.time_to_collision_s, 1);
}

TEST_F(AutoBrakeTest, NotAlertWhenNotImminent) {
  auto_brake.set_collision_threshold_s(2L);
  bus.speed_update_callback(SpeedUpdate{100L});
  bus.car_detected_callback(CarDetected{1000L, 50L});
  ASSERT_EQ(bus.command_published, 0);
}