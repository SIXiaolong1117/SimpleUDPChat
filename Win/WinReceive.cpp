// MIT License
// by Si Xiaolong (GitHub:@Direct5dom E-mail:sixiaolong2021@gmail.com)

#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    // 初始化 Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "Winsock 初始化失败！" << endl;
        return 1;
    }

    // 创建 Socket
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET)
    {
        cout << "创建 Socket 失败！" << endl;
        return 1;
    }

    int HostPort;

    cout << "请输入本地监听端口：";
    cin >> HostPort;
    cout << "设定的本地监听端口：" << HostPort << endl;

    // 绑定本地地址和端口号
    sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(HostPort);
    localAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (sockaddr *)&localAddr, sizeof(localAddr)) == SOCKET_ERROR)
    {
        cout << "绑定本地地址和端口号失败！" << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    cout << "等待接受数据中。" << endl;

    while (1)
    {
        // 接收数据
        char recvBuf[1024] = {0};
        int recvSize;
        sockaddr_in fromAddr;
        int fromAddrLen = sizeof(fromAddr);
        recvSize = recvfrom(sock, recvBuf, sizeof(recvBuf), 0, (sockaddr *)&fromAddr, &fromAddrLen);
        if (recvSize == SOCKET_ERROR)
        {
            cout << "接收数据失败！" << endl;
            closesocket(sock);
            WSACleanup();
            return 1;
        }

        // 基于当前系统的当前日期/时间
        time_t now = time(0);
        // 把 now 转换为字符串形式
        string dt = ctime(&now);
        dt.pop_back();

        // 输出接收到的数据
        cout << endl
             << "\033[36m┌[\033[0m\033[32m" << dt << "\033[0m\033[36m] - [\033[0m\033[33m来自：" << inet_ntoa(fromAddr.sin_addr) << ":" << ntohs(fromAddr.sin_port) << "\033[0m\033[36m]\033[0m" << endl
             << "\033[36m└[@\033[0m " << recvBuf << endl;
    }
    // 关闭 Socket 和 Winsock
    closesocket(sock);
    WSACleanup();

    return 0;
}