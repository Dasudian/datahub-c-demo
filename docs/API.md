# API Definition of Dasudian IoT DataHub C SDK

| Author | Date | Version | Note |
|---|---|---|---|
| Eden Wang | 1/13/2017 | 1.0.0 | first version |
| Eden Wang | 2/14/2017 | 1.1.0 | add option debug|
| Eden Wang | 2/16/2017 | 1.2.0 | Length of message should be less than 512K|

## General Info
These are the C APIs which use protocol MQTT to tranfer massively real-time data to Dasudian IoT Cloud

Most APIs are asynchronous.

You can publish the infomation collecting from a device to the cloud. Also you can subscribe to
a topic, receive a message and handle the message.

To use the SDK:

1.  Create a client object

2.  Set or not set the options for connecting to a cloud instance

3.  Subsrcibe to topics depending on whether the client needs to receive messages

4.  Repeatedly publishing messages or handling messages

5.  Disconnect the client object

## API

### create client of datahub

```
/*
 * description:This function creates a client which is ready to connect to cloud. You can
 *          use default options or set your own's.
 * parameter:
 *      client: a valid client object returned by this function after being successfully created.
 *              Note:Can not be NULL.
 *      instance_id: a valid id for connecting to cloud of dasudian, provided by Dasudian
 *              Note:Can not be NULL.
 *      instance_key: a secure key for connecting to cloud of dasudian, provided by Dasudian
 *              Note:Can not be NULL
 *      client_name: name of a device/client, usually describe the device/client
 *              Note:Can not be NULL
 *      client_id: id of device/client, uniquely identifying a device/client
 *              Note:Can not be NULL
 *      options: options of MQTT. See structure datahub_options for details. If you don't want
 *              to set them, just pass NULL. If you want to set some options, please use
 *              DATAHUB_OPTIONS_INITIALIZER to initialize first.
 *              Note:Can be NULL
 *return value:
 *      DE_OK is returned when success,otherwise failed. Error codes are
 *      defined with a beginning of "DE_"
 */
extern int datahub_create(datahub_client *client,
        char *instance_id, char *instance_key, char *client_name, char *client_id,
        datahub_options *options);
```

### connect to cloud

```
/*
 * description:
 *      let client connect to cloud of Dasudian.
 * parameter:
 *      client: returned by datahub_create()
 *          Note: can not be NULL
 * return value:
 *      DE_OK is returned when success, otherwise failed. Error codes are
 *      defined with a beginning of "DE_"
 */
extern int datahub_connect(datahub_client *client);
```

### get the status of client

```
/*
 * description:
 *      get the status of client
 * parameter:
 *      client: returned by datahub_create()
 *          Note: can not be NULL
 * return value:
 *      DATAHUB_TRUE means connected and DATAHUB_FALSE means disconnected
 */
extern int datahub_isconnected(datahub_client *client);
```

### send messages (asynchronous)

```
/*
 * description:
 *      publish messages asynchronously
 * parameter:
 *      client: returned by datahub_create()
 *          Note:can not be NULL
 *      topic: name of topic the message belongs to or regards
 *          Note:can not be NULL
 *      msg: define a message, use DATAHUB_MESSAGE_INITIALIZER to init first.
 *          then specify your own data and length of data. Remember, the length of message
 *          should be less than 512K, or an error code is returned
 *          Note: can not be NULL
 *      dt: a token representing a message being delivered, returned by this function.
 *          If you do not care whether this message arrives cloud, set it as NULL.
 *          Otherwise, this token should be defined and used in delivered callback.See
 *          option msg_delivered_cb for details.
 *          Note:can be NULL
 * return value:
 *      DE_OK is returned when success,otherwise failed.Error codes are
 *      defined with a beginning of "DE_"
 */

extern int datahub_publish(datahub_client *client, char *topic,
        datahub_message *msg, datahub_delivery_token *dt);
```
### send messages (synchronous)

```
/*
 * description:
 *      send messages synchronously
 *      Note: program will block untill message is delivered
 * parameter:
 *      client: returned by datahub_create()
 *          Note: can not be NULL
 *      topic: name of topic the message belongs to or regards
 *          Note: can not be NULL
 *      msg: define a message, use DATAHUB_MESSAGE_INITIALIZER to init first.
 *          then specify your own data and length of data. Remember, the length of message
 *          should be less than 512K, or an error code is returned
 *          Note:can not be NULL
 * return value:
 *      DE_OK is returned when success,otherwise failed.Error codes are
 *      defined with a beginning of "DE_"
 */

extern int datahub_sendrequest(datahub_client *client, char *topic, datahub_message *msg);
```

### subscribe to a topic (synchronous)

```
/*
 * description:
 *      subscribe to a topic(synchronously only)
 * parameter:
 *      client: returned by datahub_create()
 *          Note:can not be NULL
 *      topic: name of topic you want to subscribe to
 *          Note: can not be NULL
 * return value:
 *      DE_OK is returned when success, otherwise failed. Error codes are
 *      defined with a beginning of "DE_"
 */
extern int datahub_subscribe(datahub_client *client, char *topic);
```

### unsubscribe a topic(synchronous)

```
/*
 * description:
 *      unsubscribe a topic(synchronously only)
 * parameter:
 *      client: returned by datahub_create()
 *          Note: can not be NULL
 *      topic: name of topic you want to unsubscribe
 *          Note: can not be NULL
 * return value:
 *      DE_OK is returned when success,otherwise failed.Error codes are
 *      defined with a beginning of "DE_"
 */
extern int datahub_unsubscribe(datahub_client *client, char *topic);
```

### close session(synchronous)

```
/*
 * description:
 *      close session
 * parameter:
 *      client: returned by datahub_create()
 *          Note: can not be NULL
 * return value:
 *      DE_OK is returned when success,otherwise failed. Error codes are
 *      defined with a beginning of "DE_"
 */
extern int datahub_disconnect(datahub_client *client);
```

### free memory of message

````
/*
 * description:
 *      free the memory the message occupies
 *      only useful in callback of receiving a message
 * parameter:
 *     msg: describing a message
 *     Note: can not be NULL
 * return value:
 *      none
 */
extern void datahub_message_free(datahub_message *msg);
````

## Data structure

### datahub_options

```
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
     *      switch to open/close debug. For debug
     *  value:
     *      DATAHUB_FALSE or DATAHUB_TRUE
     *      default is DATAHUB_FALSE
     */
    int isdebug;

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
```

### datahub_message

```
typedef struct datahub_message_s {
    /* len of message */
    int payload_len;
    /* start of message */
    void *payload;
}datahub_message;
```

## Error codes

```
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
```
## Glossary

### client_id

*client_id* is used to identify a device. For example, if you have 2 sensors which will connect to server, one sensor's *client_name* can be "sensor1" and *client_id* can be "1". Another sensor's *client_name* can be "sensor2"(it can be set "sensor1" too) and *client_id* must **not** be "1". You can set it as "2". That means the server identifies a sensor by *client_id*.

It also means if you want to create several connections with server, you should set a **unique** *client_id* for every connection.

### auto_reconnect

It will take about 60 seconds to know a connection is lost. When connection is lost, we will try to reconnect to server if you set auto_reconnect as DATAHUB_TRUE. If it still fails, retry will start in 2s,then 4s,8s,...The maxmine interval is 64s.

## Development environment

### Linux

.Platform: Ubuntu 16.04 LTS 4.4.0-59-generic x86_64

. Compiler: gcc 5.4.0

### Windows


. Platform: Windows 7 Home Basic x86_64

. Compiler: Code Blocks 16.01


## Running environment

### Linux

It should work on all Linux distributions. But if the version of some dependencies on your Linux distribution are less than what the development environment depends, it may not work. Please upgrade your machine or contact support@dasudian.com.

### Windows

It should work on both windows x86 and windows x86_64. But if the version of some dependencies are less than what the development environment depends, it may not work. Please upgrade your machine or contact support@dasudian.com.

## Q&A

### Ubuntu

We use gcc as compiler default.

Q:
```
datahub_demo_send_syn.c:(.text+0x49): undefined reference to `datahub_create'
datahub_demo_send_syn.c:(.text+0x86): undefined reference to `datahub_connect'
datahub_demo_send_syn.c:(.text+0xf1): undefined reference to `datahub_sendrequest'
datahub_demo_send_syn.c:(.text+0x124): undefined reference to `datahub_disconnect'
datahub_demo_send_syn.c:(.text+0x137): undefined reference to `datahub_disconnect'
```

A: You should add compilation option: -ldatahub-sdk

Q:
```
/usr/bin/ld: cannot find -ldatahub-sdk
```

A: You should add compilation option: -L lib64-linux

Q:
```
lib64-linux//libdatahub-sdk.so: undefined reference to `MQTTClient_subscribe'
lib64-linux//libdatahub-sdk.so: undefined reference to `MQTTClient_disconnect'
lib64-linux//libdatahub-sdk.so: undefined reference to `MQTTClient_setCallbacks'
lib64-linux//libdatahub-sdk.so: undefined reference to `MQTTClient_publishMessage'
lib64-linux//libdatahub-sdk.so: undefined reference to `MQTTClient_isConnected'
lib64-linux//libdatahub-sdk.so: undefined reference to `MQTTClient_connect'
lib64-linux//libdatahub-sdk.so: undefined reference to `MQTTClient_unsubscribe'
lib64-linux//libdatahub-sdk.so: undefined reference to `MQTTClient_destroy'
lib64-linux//libdatahub-sdk.so: undefined reference to `MQTTClient_free'
lib64-linux//libdatahub-sdk.so: undefined reference to `MQTTClient_waitForCompletion'
lib64-linux//libdatahub-sdk.so: undefined reference to `MQTTClient_create'
lib64-linux//libdatahub-sdk.so: undefined reference to `MQTTClient_freeMessage'
```

A: You should add compilation option: -lpaho-mqtt3cs

Q:
```
./main: error while loading shared libraries: libdatahub-sdk.so: cannot open shared object file: No such file or directory
```

A: You should add compilation option: -Wl,-rpath=lib64-linux

### Windows

We use Code Blocks as compiler default.

Q:
```
xxx.c: fatal error: datahub_sdk_c.h: No such file or directory
```

A:Path of searching directories is set wrongly. Try:

Your Projects -> Build options -> Search directories -> Compiler -> Add, then choose the directory including "datahub_sdk_c.h"(default is *include*)

Q:
```
xxx.c: undefined reference to `datahub_create'
xxx.c: undefined reference to `datahub_connect'
xxx.c: undefined reference to `datahub_sendrequest'
xxx.c: undefined reference to `datahub_disconnect'
xxx.c: undefined reference to `datahub_disconnect'
```
A:Path of searching dynamic libraries is set wrongly. Try:

Projects -> Build options -> Search directories -> Linker -> Add, then choose the directory including "datahub_sdk.dll"(default is *lib32-win*)

Projects -> Build options -> Linker Settings -> Add, then choose the file "datahub_sdk.dll" in directory lib32-win.(default filter is .so or .a, you should change it to "All files(\*)". So "datahub_sdk.dll" is found)

Q:无法启动此程序，因为计算机中丢失paho-mqtt3c.dll。尝试重新安装该程序以解决此问题

A:Please copy all .dll files in directory *lib32-win* to the same path of the application

Q:无法启动此程序，因为计算机中丢失LIBEAY32.dll。尝试重新安装该程序以解决此问题

A:Please copy all .dll files in directory *lib32-win* to  the same path of the application

Q:无法启动此程序，因为计算机中丢失SSLEAY32.dll。尝试重新安装该程序以解决此问题

A:Please copy all .dll files in directory *lib32-win* to  the same path of the application

## Support

If there are other problems or advice, Please send a email to support@dasudian.com.
