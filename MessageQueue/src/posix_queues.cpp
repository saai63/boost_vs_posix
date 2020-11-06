#include "posix_queues.h"
#include "logger.h"

#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <errno.h>

posix_queues::posix_queues(std::string szQueueName, Logger* logger)
{
   m_szQueueName = szQueueName;
   m_lastErrCode = 0;
   m_hdl = 0;
   m_logger = logger;
}

bool posix_queues::bOpenQueue(eMqMode mode, bool bReadOnly, long max_msgs, long max_msg_size)
{
   bool bReturn = false;

   int flags = 0;
   if(mode == MQ_CREATE)
   {
      flags = O_CREAT | O_EXCL;
   }

   if(bReadOnly)
   {
      flags = flags | O_RDONLY;
   }
   else
   {
      flags = flags | O_RDWR;
   }   

   struct mq_attr attr;
   attr.mq_flags = flags;
   attr.mq_maxmsg = max_msgs;
   attr.mq_msgsize =  max_msg_size;

   m_hdl = mq_open(m_szQueueName.c_str(), flags, 0666, &attr);

   if(m_hdl == -1)
   {
      m_lastErrCode = errno;
      m_logger->DEBUG_L1("Failed to open message queue with errno : %d.\n",errno);
   }
   else
   {
      bReturn = true;
   }
   return bReturn;
}

bool posix_queues::bCloseQueue()
{
   bool bReturn = false;
   if(0 == mq_close(m_hdl))
      bReturn = true;
   else
      m_lastErrCode = errno;
   return bReturn;
}

bool posix_queues::bDeleteQueue()
{
   bool bReturn = false;
   if(0 == mq_unlink(m_szQueueName.c_str()))
      bReturn = true;
   else
      m_lastErrCode = errno;
   return bReturn;
}

bool posix_queues::bSendMessage(void* buffer, long length)
{
   bool bReturn = false;
   if(m_hdl == 0)
   {
      m_logger->DEBUG_L1("Queue is not open\n");
   }
   else
   {
      if( -1 == mq_send(m_hdl,reinterpret_cast<const char*>(buffer), length, 1))
      {
         m_lastErrCode = errno;
         m_logger->DEBUG_L1("Failed to send msg\n");
      }
      else
      {
         bReturn = true;
      }
   }
   return bReturn;
}

bool posix_queues::bReceiveMessage(void* buffer, long length)
{
   bool bReturn = false;
   if(m_hdl == 0)
   {
      m_logger->DEBUG_L1("Queue is not open\n");
   }
   else
   {
      if( -1 == mq_receive(m_hdl,(char*)(buffer), length, NULL))
      {
         m_lastErrCode = errno;
         m_logger->DEBUG_L1("Failed to receive msg with errno %d\n", m_lastErrCode);
      }
      else
      {
         bReturn = true;
      }
   }
   return bReturn;
}
