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


#include "Device/CameraDevice.h"
#include "EventHandle/EventHandle.h"
#include "GB28181Server.h"

GB28181Server *g_server = nullptr;



#define TEST_MODE 1 // 测试模式开关，1表示开启测试功能

#if TEST_MODE

#include <thread>
#include <chrono>
#include <atomic>
std::atomic<bool> g_testRunning{false};
CameraDevice g_currentDevice;
std::thread g_testThread;


// 显示测试菜单
void showTestMenu() {
    printf("\n=== GB28181 测试菜单 ===\n");
    printf("---------查询类功能---------:\n");
    printf("  0. 设备目录查询\n");
    printf("  1. 设备信息查询\n");
    printf("  2. 设备状态查询\n");
    printf("  3. 设备基本参数查询\n");
    printf("  4. 视频参数当前配置查询\n");
    printf("  5. 视频参数配置范围查询\n");
    printf("  6. 音频参数配置范围查询\n");
    printf("  7. 音频参数当前配置查询\n");
    printf("  8. OSD参数当前配置查询\n");
    printf("---------控制类功能---------\n");
    printf("  9. 设备重复注册下线(指定某个设备强制下线)\n");
    printf(" 10. OSD取消参数配置\n");
    printf(" 11. 设备基本参数配置\n");
    printf(" 12. 设备基本参数配置(组播)\n");
    printf(" 13. 视频参数配置(取消组播)\n");
    printf(" 14. 视频参数配置\n");
    printf(" 15. 音频参数配置\n");
    printf(" 16. OSD叠加参数配置\n");
    printf(" 17. 设备重启\n");
    printf("---------报警类功能---------\n");
    printf(" 18. 报警布防\n");
    printf(" 19. 报警撤防\n");
    printf(" 20. 报警订阅\n");
    printf("---------系统功能-----------\n");
    printf(" 21. 显示菜单\n");
    printf(" 99. 退出测试\n");
    printf("========================\n");
}

// 执行指定的测试功能
void executeTestFunction(int functionIndex) {
    switch (functionIndex) {
        case 0:
            printf("=== [设备目录查询] ===\n");
            g_server->doSendCatalog(g_currentDevice);
            break;
        case 1:
            printf("=== [设备信息查询] ===\n");
            g_server->doSendDeviceInfo(g_currentDevice);
            break;
        case 2:
            printf("=== [设备状态查询] ===\n");
            g_server->doSendDeviceStatus(g_currentDevice);
            break;
        case 3:
            printf("=== [设备基本参数查询] ===\n");
            g_server->doSendBasicParam(g_currentDevice);
            break;
        case 4:
            printf("=== [视频参数当前配置查询] ===\n");
            g_server->doSendVideoParamConfig(g_currentDevice);
            break;
        case 5:
            printf("=== [视频参数配置范围查询] ===\n");
            g_server->doSendVideoParamOpt(g_currentDevice);
            break;
        case 6:
            printf("=== [音频参数配置范围查询] ===\n");
            g_server->doSendAudioParamOpt(g_currentDevice);
            break;
        case 7:
            printf("=== [音频参数当前配置查询] ===\n");
            g_server->doSendAudioParamConfig(g_currentDevice);
            break;
        case 8:
            printf("=== [OSD参数当前配置查询] ===\n");
            g_server->doSendOSDParamConfig(g_currentDevice);
            break;
        case 9:
#if 0
            printf("=== [设备强制下线] ===\n");
            g_server->doSendKickOffline(g_currentDevice, 1);
#else
            printf("=== [设备重复注册下线(指定某个设备强制下线)] ===\n");
            g_server->doSendKickOffline(g_currentDevice, 2, g_currentDevice.IPAddress.c_str(), g_currentDevice.Port);
#endif
            // 等待设备下线确认
            printf("等待设备下线确认...\n");
            Sleep(5000);
            break;
        case 10:
            printf("=== [OSD取消参数配置] ===\n");
            g_server->do_control_OSDParamConfig_Close(g_currentDevice);
            break;
        case 11:
            printf("=== [设备基本参数配置] ===\n");
            g_server->do_control_BasicParamConfig(g_currentDevice);
            break;
        case 12:
            printf("=== [设备基本参数配置(组播)] ===\n");
            g_server->do_control_DeviceMultiCastConfig(g_currentDevice);
            break;
        case 13:
            printf("=== [视频参数配置(取消组播)] ===\n");
            g_server->do_control_VideoParamConfig_CloseMultiCast(g_currentDevice);
            break;
        case 14:
            printf("=== [视频参数配置] ===\n");
            g_server->do_control_VideoParamConfig(g_currentDevice);
            break;
        case 15:
            printf("=== [音频参数配置] ===\n");
            g_server->do_control_AudioParamConfig(g_currentDevice);
            break;
        case 16:
            printf("=== [OSD叠加参数配置] ===\n");
            g_server->do_control_OSDParamConfig(g_currentDevice);
            break;
        case 17:
            printf("=== [设备重启] ===\n");
            g_server->doSendDeviceReboot(g_currentDevice);
            printf("等待设备重启...\n");
            Sleep(5000);
            break;
        case 18:
            printf("=== [报警布防] ===\n");
            g_server->do_control_SetGuard(g_currentDevice);
            break;
        case 19:
            printf("=== [报警撤防] ===\n");
            g_server->do_control_ResetGuard(g_currentDevice);
            break;
        case 20:
            printf("=== [报警订阅] ===\n");
            g_server->do_subscribe_Alarm(g_currentDevice);
            break;
        case 21:
            showTestMenu();
            break;
        case 99:
            printf("退出测试模式\n");
            g_testRunning = false;
            break;
        default:
            printf("无效的功能编号: %d\n", functionIndex);
            printf("请输入 0-21 或 99 退出\n");
            break;
    }
}

// 交互式测试函数
void runTestLoop() {
    printf("设备注册成功，进入交互式测试模式\n");
    showTestMenu();
    
    while (g_testRunning) {
        printf("\n请输入功能编号 (输入21显示菜单, 99退出): ");
        fflush(stdout);
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            // 清理输入缓冲区
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("输入错误，请输入数字\n");
            continue;
        }
        
        // 清理输入缓冲区中的换行符
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        if (!g_testRunning) break;
        
        executeTestFunction(choice);
        
        if (choice == 99) {
            break;
        }
    }
    
    printf("测试循环结束\n");
}
#endif


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

#if TEST_MODE
    printf("设备注册成功，开始测试循环\n");
    // 保存设备信息
    g_currentDevice = device;
    
    // 启动测试循环
    if (!g_testRunning) {
        g_testRunning = true;
        g_testThread = std::thread(runTestLoop);
    }
#endif

}

void MyEventHandler::onDeviceUpdate(const CameraDevice &device)
{
    printf("onDeviceUpdate\n");

#if TEST_MODE
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // 更新设备信息
    g_currentDevice = device;

    // 查询类
    // g_server->doSendCatalog(device);                           // 设备目录查询
    // g_server->doSendDeviceInfo(device);                         // 设备信息查询
    // g_server->doSendDeviceStatus(device);                       // 设备状态查询
    // g_server->doSendBasicParam(device);                          // 设备基本参数查询
    // g_server->doSendVideoParamConfig(device);                   // 视频参数 当前配置 查询
    // g_server->doSendVideoParamOpt(device);                      // 视频参数配置范围查询
    // g_server->doSendAudioParamOpt(device);                      // 音频参数 配置范围 查询
    // g_server->doSendAudioParamConfig(device);                   // 音频参数 当前配置 查询
    // g_server->doSendOSDParamConfig(device);                      // OSD参数 当前配置 查询

    // 控制类
    // g_server->do_control_BasicParamConfig(device);                       // 设备基本参数配置
    // g_server->do_control_DeviceMultiCastConfig(device);                   // 设备基本参数配置(组播)
    // g_server->do_control_VideoParamConfig_CloseMultiCast(device);         // 视频参数配置(取消组播)
    // g_server->do_control_VideoParamConfig(device);                         // 视频参数配置
    // g_server->do_control_AudioParamConfig(device);                         // 音频参数配置
    // g_server->do_control_OSDParamConfig(device);                         // OSD 叠加参数配置(特殊十字符╋ 空字符)
    // g_server->do_control_OSDParamConfig_Close(device);                   // OSD 取消参数配置(特殊十字符╋ 空字符)


#else

    std::this_thread::sleep_for(std::chrono::seconds(1));
    g_server->doSendVideoParamConfig(device);

#endif
    

}

void MyEventHandler::onReceiveMessage(const char *deviceID, const MessageType &type, const char *msgBody)
{
    printf("onReceiveMessage\n");
}



int main()
{
    // GB28181Server *server = new GB28181Server();
    g_server = new GB28181Server();

    g_server->setLocalIp("192.168.2.6", 15060);
    g_server->setGBServerInfo("34020000002000000002", "12345678", "3402000000");

    g_server->setEventHandle(new MyEventHandler());

    g_server->start();

    printf("GB28181 服务器已启动，等待设备连接...\n");

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

#if TEST_MODE
    if (g_testRunning) {
        g_testRunning = false;
        if (g_testThread.joinable()) {
            g_testThread.join();
        }
    }
#endif

    return 0;
}
