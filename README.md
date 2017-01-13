# Introduction

This project provides demo applications written by Dasudian DataHub C SDK.

This document introduces the structure of this directory and function of every file, how to compile and run demo applications ,Q&A. If you want to know the details of APIs, please read file *docs/API.md* or header file *include/datahub_sdk_c.h*.

## Directory Introduction

There are four directories: *demo*, *docs*, *include*, *lib32-win* and *lib64-linux*.

### demo directory

This directory includes 4 examples which simply uses APIs.

*datahub_demo_send_async.c* is a demo application which creates a client object, connects to server of IoT DataHub, publishes a "hello world" message to server *asynchronously*, and disconnects the session with server.

*datahub_demo_send_sync.c* is similar with *datahub_demo_send_async.c* except it publishs a "hello world" message to server *synchronously*.

*datahub_demo_subscribe.c* subcribes to a topic user defines. If the server receives a message along with the subcribed topic ,the server will relay the message to this demo client.

*datahub_demo_unsubscribe.c* unsubcribes to the exact topic it subcribes before. If the server receives a message along with the topic, it will *not* relay the message to this demo client.

### docs directory

*API.md* introduces APIs. You can also read file *include/datahub_sdk_c.h* instead.

### include directory

*datahub_sdk_c.h* is the header you should include in your project.

### lib32-win directory

This directory includes all the .dll files. Useful for Windows.

### lib64-linux directory

This directory includes all the .so files. Useful for Linux.

## How to compile and run demo applications

if we want to compile *demo/datahub_demo_send_asyn.c*,

on Ubuntu with gcc:

```
$gcc demo/datahub_demo_send_asyn.c -o main -I include -L lib64-linux -ldatahub-sdk -lpaho-mqtt3cs -lssl -lcrypto -Wl,-rpath=lib64-linux
...
$./main
delivered with argument dt
$
```
It should work, Otherwise you can see Q&A for help.

on Windows with Code Blocks:

Firstly, you should built a project including *demo/datahub_demo_send_asyn.c*

Secondly, you should set the environment of compiling and linking :

Compiling environment:

Projects -> Build options -> Search directories -> Compiler -> Add, then choose the directory including "datahub_sdk_c.h"(default is *include*)

Linking environment:

1. Projects -> Build options -> Search directories -> Linker -> Add, then choose the directory including "datahub_sdk_c.dll"(default is *lib32-win*)

2. Projects -> Build options -> Linker Settings -> Add, then choose the file "datahub_sdk.dll" in directory lib32-win.(default filter is .so or .a, you should change it to "All files(\*)". Thus "datahub_sdk.dll" should be found.)

Finally, click 'build' and then 'run'. It should work, Otherwise you can see Q&A for help.

## Test environment of demo applications

### Ubuntu

1. Platform:Ubuntu 16.04LTS 4.4.0-31-generic x86_64

2. Compiler:gcc 5.4.0

### Windows

1. Platform: Windows x86_64

2. Compiler: Code Blocks 16.01

## FAQ

### Ubuntu

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
