#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <iostream>

#define PROJECT_ID 10
#define PERMISSION 0666

struct msgbuf2 
{
    long mtype;       /* message type, must be > 0 */
    char mtext[100];    /* message data */
};


int main()
{
    msgbuf2 message_t;
    key_t key = ftok("/home/student0/chen-rubin/cpp_fs/ws/ipc/message_q/message.txt", 
                                                                        PROJECT_ID);
    int msgId = msgget(key, PERMISSION | IPC_CREAT);
    message_t.mtype = 1;

    msgrcv(msgId, &message_t, sizeof(message_t), 1, 0);

    std::cout << "message = " << message_t.mtext << std::endl;

    return 0;
}