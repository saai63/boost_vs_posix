#include <logger.h>
#include <posix_queues.h>

#include <stdio.h>
#include <gtest/gtest.h>
#include <errno.h>
#include <pthread.h>

Logger *pLogger = NULL;

TEST(posixMessageQueueTest, Test_MsgQ_init)
{
    MessageQueueIF *pMsg = new posix_queues("/TestMsgQueue", pLogger);
    EXPECT_TRUE(pMsg != NULL);
    delete pMsg;
}

TEST(posixMessageQueueTest, Test_MsgQ_create_ok)
{
    MessageQueueIF *pMsg = new posix_queues("/TestMsgQueue", pLogger);
    EXPECT_TRUE(pMsg != NULL);
    EXPECT_TRUE(pMsg->bOpenQueue(MQ_CREATE, true, 10, 10));
    EXPECT_TRUE(pMsg->bCloseQueue());
    EXPECT_TRUE(pMsg->bDeleteQueue());
    delete pMsg;
}

TEST(posixMessageQueueTest, Test_MsgQ_open_ng)
{
    MessageQueueIF *pMsg = new posix_queues("/TestMsgQueue", pLogger);
    EXPECT_TRUE(pMsg != NULL);
    EXPECT_FALSE(pMsg->bOpenQueue(MQ_OPEN, true, 10, 10));
    delete pMsg;
}

TEST(posixMessageQueueTest, Test_MsgQ_open_ok)
{
    MessageQueueIF *pMsg = new posix_queues("/TestMsgQueue", pLogger);
    EXPECT_TRUE(pMsg != NULL);
    EXPECT_TRUE(pMsg->bOpenQueue(MQ_CREATE, true, 10, 10));
    EXPECT_TRUE(pMsg->bCloseQueue());
    EXPECT_TRUE(pMsg->bOpenQueue(MQ_OPEN, true, 10, 10));
    EXPECT_TRUE(pMsg->bCloseQueue());
    EXPECT_TRUE(pMsg->bDeleteQueue());
    delete pMsg;
}

TEST(posixMessageQueueTest, Test_MsgQ_open_or_create_ok)
{
    MessageQueueIF *pMsg = new posix_queues("/TestMsgQueue", pLogger);
    EXPECT_TRUE(pMsg != NULL);
    EXPECT_TRUE(pMsg->bOpenQueue(MQ_CREATE_OR_OPEN, true, 10, 10));
    EXPECT_TRUE(pMsg->bCloseQueue());
    EXPECT_TRUE(pMsg->bDeleteQueue());
    delete pMsg;
}

TEST(posixMessageQueueTest, Test_MsgQ_send_ng)
{
    MessageQueueIF *pMsg = new posix_queues("/TestMsgQueue", pLogger);
    EXPECT_TRUE(pMsg != NULL);
    char data_sent[] = "Hello world";
    EXPECT_FALSE(pMsg->bSendMessage(data_sent,strlen(data_sent)));
    delete pMsg;
}

TEST(posixMessageQueueTest, Test_MsgQ_send_rcv_ok)
{
    MessageQueueIF *pMsg = new posix_queues("/TestMsgQueue", pLogger);
    EXPECT_TRUE(pMsg != NULL);
    EXPECT_TRUE(pMsg->bOpenQueue(MQ_CREATE_OR_OPEN, false, 10, 10));
    char data_sent[] = "Hello";
    EXPECT_TRUE(pMsg->bSendMessage(data_sent,strlen(data_sent)));
    EXPECT_TRUE(pMsg->bCloseQueue());
    EXPECT_TRUE(pMsg->bOpenQueue(MQ_OPEN, true, 10, 10));
    char data_rcvd[10];
    EXPECT_TRUE(pMsg->bReceiveMessage(data_rcvd,10));
    EXPECT_EQ(0,strcmp(data_rcvd,data_sent));
    EXPECT_TRUE(pMsg->bCloseQueue());
    EXPECT_TRUE(pMsg->bDeleteQueue());
    delete pMsg;
}

int main(int argc, char* argv[])
{
    pLogger = Logger::getInstance();
    SET_LOG_FILE("test_log.txt");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
