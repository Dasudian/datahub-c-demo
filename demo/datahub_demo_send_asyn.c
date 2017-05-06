/*
 * 异步发送消息
 */

#include "datahub_sdk_c.h"
#include <string.h>
#include <stdio.h>

/* instance id, 由大数点提供 */
#define INSTANCE_ID    "dsd_9ITRIalNEYUJMm4Hr6_A"
/* instance key, 由大数点提供 */
#define INSTANCE_KEY   "84c07c2339e5cc17"
/* 设备的名字 */
#define CLIENT_NAME      "sensor1"
/* 设备的id */
#define CLIENT_ID      "1"

/* 发送的topic */
static char *topic = "topic";

/* 发送的消息 */
static char *send_message = {
    "hello world"
};

int main()
{
    datahub_client client;
    int ret;
    datahub_message msg = DATAHUB_MESSAGE_INITIALIZER ;
    datahub_options options = DATAHUB_OPTIONS_INITIALIZER;

    /* 设置服务器地址 */
    options.server_url = "tcp://localhost:1883";

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

    /* 发送的消息 */
    msg.payload = send_message;
    /* 消息的长度 */
    msg.payload_len = strlen(send_message) + 1;//包含结尾符\0

    /* 异步发送qos1消息 */
    ret = datahub_publish(&client, topic, &msg, 1);
    if (ERROR_NONE != ret) {
        fprintf(stdout, "asynchronously send message failed\n");
    } else {
        fprintf(stdout, "asynchronously send message success\n");
    }

    /* 断开连接并销毁客户端 */
    datahub_destroy(&client);
    fprintf(stdout, "destroy client success\n");

    return 0;
}
