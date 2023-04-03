// MIT License
// by Si Xiaolong (GitHub:@Direct5dom E-mail:sixiaolong2021@gmail.com)

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    // 创建 Socket
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)
    {
        cout << "创建 Socket 失败！" << endl;
        exit(1);
    }

    // 绑定本地地址和端口号
    sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(1234);
    localAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (sockaddr *)&localAddr, sizeof(localAddr)) < 0)
    {
        cout << "绑定本地地址和端口号失败！" << endl;
        close(sock);
        exit(1);
    }

    while (1)
    {
        // 基于当前系统的当前日期/时间
        time_t now = time(0);
        // 把 now 转换为字符串形式
        char *dt = ctime(&now);

        // 接收数据
        char recvBuf[1024] = {0};
        int recvSize;
        sockaddr_in fromAddr;
        int fromAddrLenInt = sizeof(fromAddr);
        socklen_t fromAddrLen = fromAddrLenInt;
        recvSize = recvfrom(sock, recvBuf, sizeof(recvBuf), 0, (sockaddr *)&fromAddr, &fromAddrLen);
        if (recvSize < 0)
        {
            cout << "接收数据失败！" << endl;
            close(sock);
            exit(1);
        }

        // 输出接收到的数据
        cout << endl
             << dt
             << "从 " << inet_ntoa(fromAddr.sin_addr) << ":" << ntohs(fromAddr.sin_port)
             << " 接收到数据：" << recvBuf << endl;
    }
    // 关闭 Socket 和 Winsock
    close(sock);

    return 0;
}