/*
 * send message asynchronously
 */

#include "datahub_sdk_c.h"
#include <string.h>

/* instance id, gived by dasudian */
#define INSTANCE_ID    "dsd_9FmYSNiqpFmi69Bui0_A"
/* instance key, gived by dasudian */
#define INSTANCE_KEY   "238f173d6cc0608a"
/* name of device */
#define USER_NAME      "sensor1"
/* id of device */
#define CLIENT_ID      "1"

static char *topic = "topic";

static char *send_message = {
    "hello world"
};

/* identify a message */
static datahub_delivery_token g_dt = DATAHUB_DT_INITIALIZER;
/* 1 means main thread can exit */
static int g_exit = 0;

/* called when message is delivered successfully */
void datahub_publish_msg_deliv_cb(void *context, datahub_delivery_token dt)
{
    if (dt == g_dt) {
        fprintf(stdout, "delivered with argument dt\n");
        g_exit = 1;
    }
}

int main()
{
    datahub_client client;
    int ret;
    datahub_message msg = DATAHUB_MESSAGE_INITIALIZER ;
    datahub_options options = DATAHUB_OPTIONS_INITIALIZER;

    /* set callback when message is delivered successfully */
    options.msg_delivered_cb = datahub_publish_msg_deliv_cb;

    /* create a client object */
    ret = datahub_create(&client,
            INSTANCE_ID, INSTANCE_KEY, USER_NAME, CLIENT_ID,
            &options);
    if (DE_OK != ret) {
        fprintf(stdout, "create client failed\n");
        return 1;
    }

    /* client connects to server */
    ret = datahub_connect(&client);
    if (DE_OK != ret) {
        fprintf(stdout, "client connects to server failed\n");
        return 1;
    }

    /* message to send */
    msg.payload = send_message;
    /* len of message */
    msg.payload_len = strlen(send_message) + 1;//including '\0'

    /* send message asynchronously */
    ret = datahub_publish(&client, topic, &msg, &g_dt);
    if (DE_OK != ret) {
        fprintf(stdout, "asynchronously send message failed\n");
        datahub_disconnect(&client);
        return 1;
    }

   /* wait for g_exit equals 1 */
   while(0 == g_exit) {};

    /* disconnect this session and free memory */
    datahub_disconnect(&client);

   return 0;
}
