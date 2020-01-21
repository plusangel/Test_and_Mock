//
// Created by angelos on 20/01/2020.
//

#include "autoBrake.h"
#include "gmock//gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::A;
using ::testing::DoubleEq;
using ::testing::Field;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::StrictMock;

struct MockServiceBus : IServiceBus {
  MOCK_METHOD1(publish, void(const BrakeCommand &cmd));
  MOCK_METHOD1(subscribe, void(SpeedUpdateCallback callback));
  MOCK_METHOD1(subscribe, void(CarDetectedCallback callback));
};

struct NiceAutoBrakeTest : ::testing::Test {
  NiceMock<MockServiceBus> bus;
  AutoBrake auto_brake{bus};
};

struct StrictAutoBrakeTest : ::testing::Test {

  StrictAutoBrakeTest() {
    EXPECT_CALL(bus, subscribe(A<CarDetectedCallback>()))
        .Times(1)
        .WillOnce(Invoke([this](const auto x) { car_detected_callback = x; }));
    EXPECT_CALL(bus, subscribe(A<SpeedUpdateCallback>()))
        .Times(1)
        .WillOnce(Invoke([this](const auto &x) { speed_update_callback = x; }));
  }

  CarDetectedCallback car_detected_callback;
  SpeedUpdateCallback speed_update_callback;
  StrictMock<MockServiceBus> bus;
};

TEST_F(NiceAutoBrakeTest, InitialCarSpeedIsZero) {
  ASSERT_DOUBLE_EQ(0, auto_brake.get_speed_ms());
}

TEST_F(NiceAutoBrakeTest, InitialSensitivityIsFive) {
  ASSERT_DOUBLE_EQ(5, auto_brake.get_collision_threshold_s());
}

TEST_F(NiceAutoBrakeTest, SensitivityGreaterThanOne) {
  ASSERT_ANY_THROW(auto_brake.set_collision_threshold_s(0.5L));
}

TEST_F(StrictAutoBrakeTest, NotAlertWhenNoImminent) {
  AutoBrake auto_brake{bus};

  auto_brake.set_collision_threshold_s(2L);
  speed_update_callback(SpeedUpdate{100L});
  car_detected_callback(CarDetected{1000L, 50L});
}

TEST_F(StrictAutoBrakeTest, AlertWhenImminent) {

  EXPECT_CALL(bus,
              publish(Field(&BrakeCommand::time_to_collision_s, DoubleEq(1L))))
      .Times(1);

  AutoBrake auto_brake{bus};

  auto_brake.set_collision_threshold_s(10L);
  speed_update_callback(SpeedUpdate{100L});
  car_detected_callback(CarDetected{100L, 0L});
}