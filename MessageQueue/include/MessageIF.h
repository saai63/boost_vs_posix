#ifndef MSG_QUEUE_IF_H
#define MSG_QUEUE_IF_H

typedef unsigned short MsgPortID;

struct msg{
    MsgPortID sender_port;
    char msg_data[4000];
};

typedef enum mq_mode{
    MQ_CREATE = 0,
    MQ_CREATE_OR_OPEN,
    MQ_OPEN
}eMqMode;

class MessageQueueIF{
public:
   virtual bool bOpenQueue(mq_mode mode, bool bReadOnly, long max_msgs, long max_msg_size) = 0;
   virtual bool bCloseQueue() = 0;
   virtual bool bDeleteQueue() = 0;

   virtual bool bSendMessage(void* buffer, long length) = 0;
   virtual bool bReceiveMessage(void* buffer, long length) = 0;
};

#endif