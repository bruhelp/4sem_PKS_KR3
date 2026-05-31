#include <gtest/gtest.h>

#include "../../src/scheduler/CallDispatcher.h"
#include "../../src/scheduler/FIFOStrategy.h"

#include "../../src/core/ElevatorSystem.h"
#include "../../src/core/Elevator.h"

#include "../../src/model/Call.h"

TEST(CallDispatcherTest, ReturnsUnknownWithoutStrategy)
{
    CallDispatcher dispatcher;

    EXPECT_STREQ(
        "Unknown",
        dispatcher.getStrategyName());
}

TEST(CallDispatcherTest, StrategyNameChangesAfterSetStrategy)
{
    CallDispatcher dispatcher;

    dispatcher.setStrategy(
        std::make_unique<FIFOStrategy>());

    EXPECT_STREQ(
        "FIFO",
        dispatcher.getStrategyName());
}

TEST(CallDispatcherTest, DispatchReturnsNullptrWithoutStrategy)
{
    CallDispatcher dispatcher;

    ElevatorSystem system(
        nullptr,
        nullptr);

    Call call(
        1,
        1,
        5,
        0);

    EXPECT_EQ(
        nullptr,
        dispatcher.dispatchCall(
            system,
            call));
}

TEST(CallDispatcherTest, DispatchReturnsElevatorWithStrategy)
{
    CallDispatcher dispatcher;

    dispatcher.setStrategy(
        std::make_unique<FIFOStrategy>());

    ElevatorSystem system(
        nullptr,
        nullptr);

    system.addElevator(
        std::make_unique<Elevator>(
            1,
            5,
            100,
            nullptr));

    Call call(
        1,
        1,
        5,
        0);

    Elevator* result =
        dispatcher.dispatchCall(
            system,
            call);

    EXPECT_NE(
        nullptr,
        result);
}

TEST(CallDispatcherTest, StrategyCanBeReplaced)
{
    CallDispatcher dispatcher;

    dispatcher.setStrategy(
        std::make_unique<FIFOStrategy>());

    EXPECT_STREQ(
        "FIFO",
        dispatcher.getStrategyName());

    dispatcher.setStrategy(
        std::make_unique<FIFOStrategy>());

    EXPECT_STREQ(
        "FIFO",
        dispatcher.getStrategyName());
}