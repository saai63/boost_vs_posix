#ifndef POSIX_MSG_QUEUE_H
#define POSIX_MSG_QUEUE_H

#include <string>
#include "MessageIF.h"
#include <mqueue.h>

class Logger;

class posix_queues : public MessageQueueIF{
public:
   posix_queues(std::string szQueueName, Logger* logger);
   bool bOpenQueue(eMqMode mode, bool bReadOnly, long max_msgs, long max_msg_size);
   bool bCloseQueue();
   bool bDeleteQueue();

   bool bSendMessage(void* buffer, long length);
   bool bReceiveMessage(void* buffer, long length);
private:
   std::string m_szQueueName;
   mqd_t m_hdl;
   int m_lastErrCode;
   Logger* m_logger;
};

#endif