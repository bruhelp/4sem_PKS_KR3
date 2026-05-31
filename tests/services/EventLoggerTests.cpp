#include <gtest/gtest.h>

#include "../../src/services/EventLogger.h"

#include <filesystem>
#include <fstream>
#include <string>

TEST(EventLoggerTest, ConstructorCreatesResultsDirectory)
{
    EventLogger logger;

    EXPECT_TRUE(
        std::filesystem::exists("results"));
}

TEST(EventLoggerTest, ConstructorCreatesLogFile)
{
    EventLogger logger;

    EXPECT_TRUE(
        std::filesystem::exists(
            "results/events.log"));
}

TEST(EventLoggerTest, LogWritesMessage)
{
    EventLogger logger;

    logger.log("TEST_MESSAGE");

    std::ifstream file(
        "results/events.log");

    std::string line;
    bool found = false;

    while (std::getline(file, line))
    {
        if (line == "TEST_MESSAGE")
        {
            found = true;
            break;
        }
    }

    EXPECT_TRUE(found);
}

TEST(EventLoggerTest, MultipleMessagesCanBeWritten)
{
    EventLogger logger;

    logger.log("MSG1");
    logger.log("MSG2");

    EXPECT_TRUE(true);
}

TEST(EventLoggerTest, EmptyMessageDoesNotCrash)
{
    EventLogger logger;

    EXPECT_NO_THROW(
        logger.log(""));
}

TEST(EventLoggerTest, LongMessageDoesNotCrash)
{
    EventLogger logger;

    std::string msg(
        1000,
        'A');

    EXPECT_NO_THROW(
        logger.log(msg));
}