#include "logger.h"

#include <boost_queues.h>
#include <boost/interprocess/ipc/message_queue.hpp>

using namespace boost::interprocess;

boost_queues::boost_queues(std::string szQueueName, Logger* logger)
{
    m_szQueueName = szQueueName;
    m_logger = logger;
    m_hdl = NULL;
}

bool boost_queues::bOpenQueue(eMqMode mode, bool bReadOnly, long max_msgs, long max_msg_size)
{
    bool bReturn = true;
    try
    {
        switch (mode)
        {
        case MQ_CREATE:
            m_hdl = new message_queue(create_only, m_szQueueName.c_str(), max_msgs, max_msg_size);
            break;
        case MQ_CREATE_OR_OPEN:
            m_hdl = new message_queue(open_or_create, m_szQueueName.c_str(), max_msgs, max_msg_size);
            break;
        case MQ_OPEN:
            m_hdl = new message_queue(open_only, m_szQueueName.c_str());
            break;
        }
    }
    catch(const std::exception& e)
    {
        m_logger->DEBUG_L1("Exception caught %s\n", e.what());
        bReturn = false;
    }
    return bReturn;
}

/* NOP */
bool boost_queues::bCloseQueue()
{
   bool bReturn = true;
   return bReturn;
}

bool boost_queues::bDeleteQueue()
{
   return m_hdl->remove(m_szQueueName.c_str());
}

bool boost_queues::bSendMessage(void* buffer, long length)
{
   bool bReturn = true;
   if(!m_hdl)
   {
       DEBUG_L1("Queue is not open\n");
       bReturn = false;
   }
   else
   {
       try
       {
           m_hdl->send(buffer, length, 0);
       }
       catch (const std::exception &e)
       {
           m_logger->DEBUG_L1("Exception caught : %s\n", e.what());
           bReturn = false;
       }
   }
   return bReturn;
}

bool boost_queues::bReceiveMessage(void* buffer, long length)
{
   bool bReturn = true;
   try
   {
       message_queue::size_type rcvd_size;
       unsigned int prio = 0;
       m_hdl->receive(buffer, length, rcvd_size, prio);
   }
   catch(const std::exception& e)
   {
       m_logger->DEBUG_L1("Exception caught : %s\n", e.what());
       bReturn = false;
   }
   return bReturn;
}
