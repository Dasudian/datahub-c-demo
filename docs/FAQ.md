# Linux可能遇到的问题

默认使用gcc编译器

Q:
```
datahub_demo_send_syn.c:(.text+0x49): undefined reference to `datahub_create'
datahub_demo_send_syn.c:(.text+0x86): undefined reference to `datahub_connect'
datahub_demo_send_syn.c:(.text+0xf1): undefined reference to `datahub_sendrequest'
datahub_demo_send_syn.c:(.text+0x124): undefined reference to `datahub_disconnect'
datahub_demo_send_syn.c:(.text+0x137): undefined reference to `datahub_disconnect'
```

A: 需要添加链接选项 -ldatahub-sdk

Q:
```
/usr/bin/ld: cannot find -ldatahub-sdk
```

A: 需要指明SDK库所在的路径: -L lib64-linux

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

A: 需要链接库paho-mqtt3cs: -lpaho-mqtt3cs

Q:
```
./main: error while loading shared libraries: libdatahub-sdk.so: cannot open shared object file: No such file or directory
```

A: 需要指明运行时SDK库所在的路径: -Wl,-rpath=lib64-linux
