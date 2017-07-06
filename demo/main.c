/*
 * demo源代码
 */

#include "datahub_sdk_c.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

/* instance id, 标识客户的唯一ID，请联系大数点商务support@dasudian.com获取 */
#define INSTANCE_ID    "your_instance_id"
/* instance key, 与客户标识相对应的安全密钥，请联系大数点商务support@dasudian.com获取 */
#define INSTANCE_KEY   "your_instance_key"
/* 设备的名字 */
#define CLIENT_NAME    "sensor1"
/* 设备的id */
#define CLIENT_ID      "1"

/* 发送的topic */
static char *topic = "topic";

/* 发送的消息 */
static char *send_message = {
    "hello world"
};

/* 接收到消息后的回调函数 */
static void message_received(void *context, char *topic, datahub_message *msg)
{
    fprintf(stdout, "receive a message: ");
    fprintf(stdout, "topic: %s ", topic);
    fprintf(stdout, "payload: %.*s\n", msg->payload_len, (char *)msg->payload);

    datahub_callback_free(topic, msg);
}

int main()
{
    datahub_client client;
    int ret;
    datahub_message msg = DATAHUB_MESSAGE_INITIALIZER ;
    datahub_options options = DATAHUB_OPTIONS_INITIALIZER;

     /* 大数点IoT DataHub云端地址，请联系大数点商务support@dasudian.com获取 */
    options.server_url = "tcp://server_url:1883";
    /* 设置接收到消息后的回调函数 */
    options.message_received = message_received;

    /* 创建客户端实例 */
    ret = datahub_create(&client,
            INSTANCE_ID, INSTANCE_KEY, CLIENT_NAME, CLIENT_ID,
            &options);
    if (ERROR_NONE != ret) {
        fprintf(stdout, "create client failed\n");
        return 1;
    } else {
        fprintf(stdout, "create client success\n");
    }

    /* 订阅待发送消息的topic, 最大以qos1接收, 这样就可以接收到自己发送的消息 */
    ret = datahub_subscribe(&client, topic, 1, 5);
    if (ERROR_NONE != ret) {
        fprintf(stdout, "subscribe topic %s failed\n", topic);
    } else {
        fprintf(stdout, "subscribe topic %s success\n", topic);
    }

    /* 发送的消息 */
    msg.payload = send_message;
    /* 消息的长度 */
    msg.payload_len = strlen(send_message) + 1;//包含结尾符\0

    /* 同步发送qos2消息 */
    ret = datahub_sendrequest(&client, topic, &msg, 1, 2);
    if (ERROR_NONE != ret) {
        fprintf(stdout, "synchronously send message failed\n");
    } else {
        fprintf(stdout, "synchronously send message success\n");
    }

    /* 睡眠以便于接收完服务器下发的消息 */
    sleep(1);
    /* 断开连接并销毁客户端 */
    datahub_destroy(&client);
    fprintf(stdout, "destroy client success\n");

    return 0;
}
