/*
 * send message synchronously
 */

#include "datahub_sdk_c.h"
#include <string.h>
#include <stdio.h>

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

int main()
{
    datahub_client client;
    int ret;
    datahub_message msg = DATAHUB_MESSAGE_INITIALIZER ;

    /* create a client object */
    ret = datahub_create(&client,
            INSTANCE_ID, INSTANCE_KEY, USER_NAME, CLIENT_ID,
            NULL);
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

    /* send message synchronously */
    ret = datahub_sendrequest(&client, topic, &msg);
    if (DE_OK != ret) {
        fprintf(stdout, "synchronously send message failed\n");
        datahub_disconnect(&client);
        return 1;
    }

    /* disconnect this session and free memory */
    datahub_disconnect(&client);

    return 0;
}

