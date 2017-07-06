# 介绍

该项目提供由C SDK开发的demo程序.

此文档介绍该项目的目录结构和各个文件的功能, 如何编译和运行demo程序. 如果您想了解API的细节, 请阅读 [API](docs/API.pdf) 或者头文件 [datahub_sdk_c.h](include/datahub_sdk_c.h).

## 目录结构说明

该项目有四个目录: *demo*, *docs*, *include* 和 *lib*.

### demo目录

该目录下包含了一个使用Linux C SDK的[demo](demo/main.c)程序

### docs目录

*API.pdf* 包含各个API的详细介绍和用法, 以及常见问题

### include目录

*datahub_sdk_c.h* 是SDK的头文件

### lib目录

该目录下包含了SDK及依赖库.

## 线程安全

所有的API都是线程安全的.

## 如何编译和运行demo

根据提示下载最新的SDK, 再编译

```
$ make
```

运行

```
$ ./main
```
