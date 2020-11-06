#ifndef BOOST_MESSAGE_QUEUE_H
#define BOOST_MESSAGE_QUEUE_H

#include <string>
#include "MessageIF.h"
#include <boost/interprocess/ipc/message_queue.hpp>

class Logger;

class boost_queues : public MessageQueueIF{
public:
   boost_queues(std::string szQueueName, Logger* logger);
   bool bOpenQueue(eMqMode mode, bool bReadOnly, long max_msgs, long max_msg_size);
   bool bCloseQueue();
   bool bDeleteQueue();

   bool bSendMessage(void* buffer, long length);
   bool bReceiveMessage(void* buffer, long length);
private:
   std::string m_szQueueName;
   boost::interprocess::message_queue* m_hdl;
   Logger* m_logger;
};

#endif
