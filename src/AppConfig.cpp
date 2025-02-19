#include "AppConfig.h"

#if defined(WIN32)
#include <winsock2.h>
#include <windows.h>
#include <direct.h>
#include <io.h> //C (Windows)    access
#else
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

#include <string.h>


void Sleep(long mSeconds)
{
    usleep(mSeconds * 1000);
}

#endif

int AppConfig::VERSION = 1;
char AppConfig::VERSION_NAME[32] = "1.0.0";

GB28181Server *AppConfig::gGB28181Server = nullptr;

AppConfig::AppConfig()
{

}

void AppConfig::mkdir(char *dirName)
{
#if defined(WIN32)
    ///如果目录不存在 则创建
    if (access(dirName, 0)!=0)
    {
        _mkdir(dirName);
    }
#else
    ///如果目录不存在 则创建
    if (access(dirName, R_OK)!=0)
    {
        char cmd[128] = {0};
        sprintf(cmd,"mkdir %s", dirName);
        system(cmd);
    }
#endif
}

void AppConfig::mkpath(char *path)
{
#if defined(WIN32)
        ///windows创建文件夹命令 路径得是反斜杠 因此这里需要替换一下
        char dirPath[128] = {0};
        strcpy(dirPath, path);

        AppConfig::replaceChar(dirPath, '/', '\\');

        ///如果目录不存在 则创建它
        if (access(dirPath, 0)!=0)
        {
    //        _mkdir(dirPath);
            char cmd[128];
            sprintf(cmd,"mkdir %s", dirPath);
            system(cmd);
        }

#else
    ///如果目录不存在 则创建它
    if (access(path,R_OK)!=0)
    {
        char cmd[128];
        sprintf(cmd,"mkdir %s -p",path);
        system(cmd);
    }
#endif
}

void AppConfig::removeDir(char *dirName)
{
    if (strlen(dirName) <= 0) return;

    if (access(dirName, 0) != 0 ) //文件夹不存在
    {
        return;
    }

#if defined(WIN32)

    ///删除本地文件
    char cmd[128];
    sprintf(cmd,"rd /s /q \"%s\"", dirName);
    system(cmd);

#else

    char cmd[128];
    sprintf(cmd,"rm -rf \"%s\"",dirName);
    system(cmd);

#endif
}

void AppConfig::removeFile(const char *filePath)
{
    if (filePath == NULL || strlen(filePath) <= 0) return;

#if defined(WIN32)

        ///删除本地文件
        remove(filePath);

#else
        ///删除本地文件
        char cmd[128] = {0};
        sprintf(cmd,"rm -rf \"%s\"",filePath);
        system(cmd);
#endif
}

void AppConfig::copyFile(const char *srcFile, const char *destFile)
{

#if defined(WIN32)
        CopyFileA(srcFile, destFile, FALSE);
#else

        ///将文件拷贝到远端服务器
        char copyfilecmd[512];
        sprintf(copyfilecmd,"cp \"%s\" \"%s\"", srcFile, destFile);
        system(copyfilecmd);

#endif
}

void AppConfig::replaceChar(char *string, char oldChar, char newChar)
{
    int len = strlen(string);
    int i;
    for (i = 0; i < len; i++){
        if (string[i] == oldChar){
            string[i] = newChar;
        }
    }
}


std::string AppConfig::removeFirstAndLastSpace(std::string &s)
{
    if (s.empty())
    {
        return s;
    }
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

void AppConfig::mSleep(int mSecond)
{
#if defined(WIN32)
    Sleep(mSecond);
#else
    usleep(mSecond * 1000);
#endif
}

int64_t AppConfig::getTimeStamp_MilliSecond()
{

    int mSecond = 0; //当前毫秒数

#if defined(WIN32)

    SYSTEMTIME sys;
    GetLocalTime( &sys );

    mSecond = sys.wMilliseconds;

#else

    struct timeval    tv;
    struct timezone tz;

    struct tm         *p;

    gettimeofday(&tv, &tz);
    p = localtime(&tv.tv_sec);

    mSecond = tv.tv_usec / 1000;


#endif

    int64_t timeStamp = ((int64_t)time(NULL)) * 1000 + mSecond;

    return timeStamp;

}


#include "EventHandle.h"
#include "GB28181Server.h"

GB28181Server *g_server = nullptr;

class MyEventHandler : public GB28181ServerEventHandle
{
    ///用于输出到界面上的回调函数
public:
    void onDeviceRegisted(const CameraDevice &device) override; //设备注册成功
    void onDeviceUpdate(const CameraDevice &device) override;   //设备更新，catalog请求返回的设备信息更新
    void onReceiveMessage(const char *deviceID, const MessageType &type, const char *msgBody) override;  //接收到消息

};

void MyEventHandler::onDeviceRegisted(const CameraDevice &device)
{
    printf("onDeviceRegisted\n");
}

void MyEventHandler::onDeviceUpdate(const CameraDevice &device)
{
    printf("onDeviceUpdate\n");

    // std::this_thread::sleep_for(std::chrono::seconds(1));

    // g_server->doSendVideoParamConfig(device);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    g_server->do_control_VideoParamConfig(device);

}

void MyEventHandler::onReceiveMessage(const char *deviceID, const MessageType &type, const char *msgBody)
{
    printf("onReceiveMessage\n");
}



int main()
{
    // GB28181Server *server = new GB28181Server();
    g_server = new GB28181Server();

    g_server->setLocalIp("192.168.2.6", 5060);
    g_server->setGBServerInfo("34020000002000000001", "12345678", "3402000000");

    g_server->setEventHandle(new MyEventHandler());

    g_server->start();

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }


    return 0;
}
