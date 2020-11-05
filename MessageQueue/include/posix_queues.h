#ifndef POSIX_MSG_QUEUE_H
#define POSIX_MSG_QUEUE_H

#include "MessageIF.h"

class LoggerIF;

class posix_queues : public MessageQueueIF{
public:
   posix_queues(std::string szQueueName, LoggerIF* logger);
   bool bOpenQueue(int flags, bool bBlock, long max_msgs, long max_msg_size);
   bool bCloseQueue();
   bool bDeleteQueue();

   bool bSendMessage(void* buffer, long length);
   bool bReceiveMessage(void* buffer, long length);
private:
   std::string m_szQueueName;
   mqd_t m_hdl;
   int m_lastErrCode;
   LoggerIF* m_logger;
};

#endif