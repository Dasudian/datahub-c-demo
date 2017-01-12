/*
 * This is the C SDK of dasudian.It provides a simple way to connect servers
 * of dasudian with protocol MQTT.Have fun with it.
 */
#ifndef DATAHUB_SDK_C_H
#define DATAHUB_SDK_C_H

#include <stdlib.h>
#include <string.h>

#define DATAHUB_TRUE 1
#define DATAHUB_FALSE 0

#define DEFAULT_SERVER_URL      "tcp://try.iotdatahub.net:1883"
#define DEFAULT_AUTO_RECON      DATAHUB_TRUE
#define DEFAULT_CLEAN_SES       DATAHUB_TRUE
//seconds
#define DEFAULT_CONN_TMOUT      (30)
//seconds
#define DEFAULT_COMM_TMOUT      (60)
#define DEFAULT_IGN_CERT        DATAHUB_TRUE
#define DEFAULT_DEF_CERT        DATAHUB_FALSE
#define DEFAULT_PRIVATE_CERT    "private_cert"
#define DEFAULT_CONTEXT         NULL
#define DEFAULT_CALLBACK        NULL

#define DATAHUB_OPTIONS_INITIALIZER {\
    DEFAULT_SERVER_URL,\
    DEFAULT_AUTO_RECON,\
    DEFAULT_CLEAN_SES,\
    DEFAULT_CONN_TMOUT,\
    DEFAULT_COMM_TMOUT,\
    DEFAULT_IGN_CERT,\
    DEFAULT_DEF_CERT,\
    DEFAULT_PRIVATE_CERT,\
    DEFAULT_CONTEXT,\
    DEFAULT_CALLBACK,\
    DEFAULT_CALLBACK,\
    DEFAULT_CALLBACK\
}

/* only one char '\0' */
#define DATAHUB_MESSAGE_INITIALIZER {\
    1,\
    ""\
}

#define DATAHUB_DT_INITIALIZER {0}

/* a token representing a message,just like id of a message.It is unique */
typedef int datahub_delivery_token;

/* representing a client,should be created by datahub_create() */
typedef void* datahub_client;

/*
 * description:
 *      a structure describing a message
 */
typedef struct datahub_message_s {
    /* len of message */
    int payload_len;
    /* start of message */
    void *payload;
}datahub_message;

/*
 * description:
 *      callback's prototype when delivering a message
 * parameter:
 *      context:just that option which is set in option "context",see datahub_options_s
 *          for help.
 *      dt:a token representing a message
 * example:
 *      void message_delivered_cb(void *context, datahub_delivery_token dt)
 *      {
 *          //some operations
 *      }
 */
typedef void MESSAGE_DELIVERED(void *context, datahub_delivery_token dt);

/*
 * description:
 *      callback's prototype when receiving a message
 * parameter:
 *      context:just that option which is set in option "context",see datahub_options_s
 *          for help.
 *      topic_name:name of topic the message belongs to.Must call function
 *          free() to release memory
 *      msg:describing a message.see structure datahub_message for details.
 *          Must call function datahub_message_free() to release memory
 * example:
 *      void message_received_cb(void *context, char *topic_name, datahub_message *msg)
 *       {
 *          //some operations
 *
 *           free(topic_name);
 *           datahub_message_free(msg);
 *       }
 */
typedef void MESSAGE_RECEIVED(void *context, char *topic_name, datahub_message *msg);

/*
 * description:
 *      callback's prototype when losing connection
 * parameter:
 *      context:just that option which is set in option "context",see datahub_options_s
 *          for help.
 *      cause:description of error.Must call function
 *          free() to release memory
 * example:
 *      void connection_lost_cb(void *context, char *cause)
 *       {
 *          //some operations
 *
 *           free(cause);
 *       }
 */
typedef void CONNECTION_LOST(void *context);

typedef struct datahub_options_s {
    /*
     * description:
     *      url of server.
     * value:
     *      "protocol://host:port".protocol supports tcp and ssl;host can be ip or domain
     *      default is DEFAULT_SERVER_URL
     */
    char *server_url;

    /*
     * description:
     *      whether or not reconnect automatically if connection is lost.
     *      When connection is lost,(1)if auto_reconnect is set DATAHUB_TRUE,then we will
     *      try reconnect to server in a few seconds,(2)If auto_reconnect is set DATAHUB_FALSE,
     *      callback "connection_lost_cb" will be called if it's set
     * value:
     *      DATAHUB_TRUE means yes and DATAHUB_FALSE means no.
     *      default is DEFAULT_AUTO_RECON
     */
    int auto_reconnect;

    /*
     * description:
     *      whether or not clean session after disconnecting
     * value:
     *      DATAHUB_TRUE means yes and DATAHUB_FALSE means no
     *      default is DEFAULT_CLEAN_SES
     */
    int clean_session;

    /*
     * description:
     *      timeout when connecting server(seconds)
     * value:
     *      works when value > 0
     *      default is DEFAULT_CONN_TMOUT
     */
    long long connection_timeout;

    /*
     * description:
     *      timeout when sending message synchronously
     * value:
     *      works when value > 0
     *      default is DEFAULT_COMM_TMOUT
     */
    long long command_timeout;

    /*
     * description:
     *      ignore certificate when connecting to server with security
     * value:
     *      DATAHUB_TRUE means yes and DATAHUB_FALSE means no
     *      default is DEFAULT_IGN_CERT
     *      note:must be DATAHUB_TRUE for now
     */
    int ignore_certificate;

    /*
     * description:
     *      use default certificate when connecting with security
     * value:
     *      DATAHUB_TRUE means yes and DATAHUB_FALSE means no
     *      default is DEFAULT_DEF_CERT
     *      note:not support yet,will be added in the future
     */
    int default_certificate;
   /*
     * description:
     *      use private certificate when connecting with security
     * value:
     *      NULL or path of certificate
     *      default is DEFAULT_PRIVATE_CERT
     *      note:not support yet,will be added in the future
     */
    char *private_certificate;

    /*
     * description:
     *      a pointer which will be passed as an argument to callbacks,
     *      including msg_delivered_cb,msg_received_cb,and connection_lost_cb.
     *      this three callbacks is defined below.
     */
    void *context;
     /*
     * description:
     *      a callback,called after delivering a message
     * value:
     *      NULL or a valid callback
     *      default is DEFAULT_CALLBACK
     */
    MESSAGE_DELIVERED *msg_delivered_cb;

    /*
     * description:
     *      a callback,called after receiving a message
     * value:
     *      NULL or a valid callback
     *      default is DEFAULT_CALLBACK
     */
    MESSAGE_RECEIVED *msg_received_cb;

    /*
     * description:
     *      a callback,called after losing connection.
     *      option "auto_reconnect" must be set DATAHUB_FALSE otherwise this callback
     *      will not be called.
     * value:
     *      NULL or a valid callback
     *      default is DEFAULT_CALLBACK
     */
    CONNECTION_LOST *connection_lost_cb;
}datahub_options;

enum datahub_error_code_s {
    /*
     * Return code: success
     */
    DE_OK = 0,
    /*
     * Return code: unacceptable protocol version
     */
    DE_UNACCEPT_PROTO_VER = 1,
    /*
     * Return code: identifier rejected
     */
    DE_IDENT_REJ = 2,
    /*
     * Return code: server unavailable
     */
    DE_SERVER_UNAVAIL = 3,
    /*
     * Return code: bad instance id or instance key
     */
    DE_BAD_UNAME_PASSWD = 4,
    /*
     * Return code: unathorized
     */
    DE_UNATHOR = 5,

    /*
     * Return code:server authorizing users is unavailable
     */
    DE_AUTHO_SERVER_UNAVAIL = 300,
     /*
     * Return code: other errors when connecting to server
     */
    DE_CONNECT_OTHER_ERROR = 301,
    /*
     * Return code: some essential arguments are NULL
     */
    DE_NULL = 302,
    /*
     * Return code: memory allocation failed
     */
    DE_MEM_ALLOC = 303,
    /*
     * Return code: create client of MQTT failed
     */
    DE_CREATE_MQTT_CLI = 304,
    /*
     * Return code: set callbacks of MQTT failed
     */
    DE_SET_CB_ERROR = 305,

    /*
     * Return code: message is wrong
     */
    DE_MESSAGE_ERROR = 306,
    /*
     * Return code: error occurs when sending a message
     */
    DE_SEND_MSG_ERROR = 307,

    /*
     * Return code: error occurs when subscribing a topic
     */
    DE_SUBSCRIBE_ERROR = 308,

    /*
     * Return code: error occurs when unsubscribing a topic
     */
    DE_UNSUBSCRIBE_ERROR = 309,

    /*
     * Return code: error occurs when disconnecting
     */
    DE_DISCON_FAIL = 310,

    /*
     * Return code: client is disconnected
     */
    DE_CLIENT_DISCON = 311,

    /*
     * Return code: connect fail,losing Internet or bad url of server
     *  can cause this
     */
    DE_CON_FAIL = 312
};

/*
 * description:This function creates a client which is ready to connect to server.You can
 *          use default options or set your own's.
 * parameter:
 *      client: a valid client returned by this function after being successfully created.
 *              Note:Can not be NULL.
 *      instance_id: a valid id for connecting to servers of dasudian.
 *              Note:Can not be NULL.
 *      instance_key: key of id for connecting to servers of dasudian.
 *              Note:Can not be NULL
 *      client_name: name of device.
 *              Note:Can not be NULL
 *      client_id: id of device.
 *              Note:Can not be NULL
 *      options: options of MQTT.see structure datahub_options for details.If you don't want
 *              to set them,just pass NULL.If you want to set some options,please use
 *              DATAHUB_OPTIONS_INITIALIZER init first.
 *              Note:Can be NULL
 *return value:
 *      DE_OK is returned when success,otherwise failed.Error codes are
 *      defined with a beginning of "DE_"
 */
extern int datahub_create(datahub_client *client,
        char *instance_id, char *instance_key, char *client_name, char *client_id,
        datahub_options *options);

/*
 * description:
 *      let client connect to server.
 * parameter:
 *      client:returned by datahub_create()
 *          Note:can not be NULL
 * return value:
 *      DE_OK is returned when success,otherwise failed.Error codes are
 *      defined with a beginning of "DE_"
 */
extern int datahub_connect(datahub_client *client);

/*
 * description:
 *      judge the status of client
 * parameter:
 *      client:returned by datahub_create()
 *          Note:can not be NULL
 * return value:
 *      DATAHUB_TRUE means connected and DATAHUB_FALSE means disconnected
 */
extern int datahub_isconnected(datahub_client *client);

/*
 * description:
 *      publish messages asynchronously
 * parameter:
 *      client:returned by datahub_create()
 *          Note:can not be NULL
 *      topic:name of topic the message belongs to
 *          Note:can not be NULL
 *      msg:define a message,use DATAHUB_MESSAGE_INITIALIZER to init first.
 *          then specfy your own data and length of data.
 *          Note:can not be NULL
 *      dt:a token representing a delivering message is returned by this function.
 *          If you do not care whether this message arrives server,NULL is fine.
 *          If you want to know whether this message is deliveried successfully,You
 *          can store this token and used in delivered callback(set option msg_delivered_cb)
 *          Note:can be NULL
 * return value:
 *      DE_OK is returned when success,otherwise failed.Error codes are
 *      defined with a beginning of "DE_"
 */

extern int datahub_publish(datahub_client *client, char *topic,
        datahub_message *msg, datahub_delivery_token *dt);

/*
 * description:
 *      send messages synchronously
 *      Note:program will block untill message is deliveried
 * parameter:
 *      client:returned by datahub_create()
 *          Note:can not be NULL
 *      topic:name of topic the message belongs to
 *          Note:can not be NULL
 *      msg:define a message,use DATAHUB_MESSAGE_INITIALIZER to init first.
 *          then specfy your own data and length of data.
 *          Note:can not be NULL
 * return value:
 *      DE_OK is returned when success,otherwise failed.Error codes are
 *      defined with a beginning of "DE_"
 */

extern int datahub_sendrequest(datahub_client *client, char *topic, datahub_message *msg);

/*
 * description:
 *      subscribe a topic(synchronously only)
 * parameter:
 *      client:returned by datahub_create()
 *          Note:can not be NULL
 *      topic:name of topic you want to subscribe
 *          Note:can not be NULL
 * return value:
 *      DE_OK is returned when success,otherwise failed.Error codes are
 *      defined with a beginning of "DE_"
 */
extern int datahub_subscribe(datahub_client *client, char *topic);

/*
 * description:
 *      unsubscribe a topic(synchronously only)
 * parameter:
 *      client:returned by datahub_create()
 *          Note:can not be NULL
 *      topic:name of topic you want to unsubscribe
 *          Note:can not be NULL
 * return value:
 *      DE_OK is returned when success,otherwise failed.Error codes are
 *      defined with a beginning of "DE_"
 */
extern int datahub_unsubscribe(datahub_client *client, char *topic);

/*
 * description:
 *      close session
 * parameter:
 *      client:returned by datahub_create()
 *          Note:can not be NULL
 * return value:
 *      DE_OK is returned when success,otherwise failed.Error codes are
 *      defined with a beginning of "DE_"
 */
extern int datahub_disconnect(datahub_client *client);

/*
 * description:
 *      free the memory the message occupies
 *      only useful in callback of receiving a message
 * parameter:
 *     msg:describing a message
 *     Note:can not be NULL
 * return value:
 *      none
 */
extern void datahub_message_free(datahub_message *msg);

#endif
