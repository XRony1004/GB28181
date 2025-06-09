#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdio.h>
#include <list>

enum MessageType
{
    MessageType_Register = 0,       //注册
    MessageType_KeepAlive = 1,      //心跳
    MessageType_Catalog = 2,        //目录查询
    MessageType_CallAnswer = 3,     //视频应答
    MessageType_CallFailed = 4,     //视频失败
    MessageType_Unregister = 5,     //注销
};

#endif // TYPES_H
