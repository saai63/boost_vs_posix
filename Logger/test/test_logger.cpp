#include <logger.h>
#include <stdio.h>
#include <gtest/gtest.h>
#include <errno.h>
#include <pthread.h>

TEST(LoggerTest,Test_console_default)
{
    printf("The below logs should be printed to console\n");
    char *logString = "This is a log\n";
    EXPECT_EQ(DEBUG_L1("%s",logString),strlen(logString));
    EXPECT_EQ(DEBUG_L2("%s",logString),strlen(logString));
    EXPECT_EQ(DEBUG_L3("%s",logString),0);
    EXPECT_EQ(DEBUG_L4("%s",logString),0);
}

TEST(LoggerTest,Test_console_additional)
{
    printf("The below logs should be printed to console\n");
    SET_LOG_LEVEL(15);
    char *logString = "This is a log\n";
    EXPECT_EQ(DEBUG_L1("%s",logString),strlen(logString));
    EXPECT_EQ(DEBUG_L2("%s",logString),strlen(logString));
    EXPECT_EQ(DEBUG_L3("%s",logString),strlen(logString));
    EXPECT_EQ(DEBUG_L4("%s",logString),strlen(logString));
}

TEST(LoggerTest,Test_file_default)
{
    unlink("test_logging.txt");
    printf("The below logs should be printed to a log file\n");
    SET_LOG_LEVEL(3);
    SET_LOG_FILE("test_logging.txt");
    char *logString = "This is a log\n";
    EXPECT_EQ(DEBUG_L1("%s",logString),strlen(logString));
    EXPECT_EQ(DEBUG_L2("%s",logString),strlen(logString));
    EXPECT_EQ(DEBUG_L3("%s",logString),0);
    EXPECT_EQ(DEBUG_L4("%s",logString),0);
    struct stat buf;
    EXPECT_EQ(stat("test_logging.txt", &buf),0);
}

TEST(LoggerTest,Test_file_additional)
{
    struct stat buf;
    stat("test_logging.txt", &buf);
    int old_size = buf.st_size;
    printf("The below logs should be printed to a log file\n");
    SET_LOG_LEVEL(15);
    char *logString = "This is a log\n";
    EXPECT_EQ(DEBUG_L1("%s",logString),strlen(logString));
    EXPECT_EQ(DEBUG_L2("%s",logString),strlen(logString));
    EXPECT_EQ(DEBUG_L3("%s",logString),strlen(logString));
    EXPECT_EQ(DEBUG_L4("%s",logString),strlen(logString));
    stat("test_logging.txt", &buf);
    EXPECT_GT(buf.st_size, old_size);
}

TEST(LoggerTest,Test_new_file)
{
    unlink("test_logging1.txt");
    char *logString = "This is another log\n";
    SET_LOG_FILE("test_logging1.txt");
    printf("This log should come in new log\n");
    EXPECT_EQ(DEBUG_L1("%s",logString),strlen(logString));
    struct stat buf;
    EXPECT_EQ(stat("test_logging1.txt", &buf),0);
    SET_LOG_FILE("");
    printf("This log should come on console\n");
    EXPECT_EQ(DEBUG_L1("%s",logString),strlen(logString));
}

TEST(LoggerTest,Test_error_case)
{
    SET_LOG_FILE("temp/test.txt");
    char *logString = "This is another log\n";
    printf("The below logs should come on console\n");
    EXPECT_EQ(DEBUG_L1("%s",logString),strlen(logString));
}

void* threadFunc(void* arg)
{
    DEBUG_L1("This is thread log\n");
    return NULL;
}

TEST(LoggerTest,Test_thread_case)
{
    unlink("logger_threads.txt");
    SET_LOG_FILE("logger_threads.txt");
    pthread_attr_t attr;
    pthread_t loggerThread;
    pthread_attr_init(&attr);

    EXPECT_EQ(pthread_create(&loggerThread, &attr, threadFunc, NULL), 0);
    pthread_join(loggerThread, NULL);
    struct stat buf;
    EXPECT_EQ(stat("logger_threads.txt", &buf),0);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    return 0;
}
