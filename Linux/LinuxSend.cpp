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

    // 设置目标主机地址和端口号
    char *HostIP = "127.0.0.1";
    int HostPort = 1234;
    string HostIPtmp;
    cout << "请输入目标主机IP：";
    cin >> HostIPtmp;
    HostIP = &HostIPtmp[0];
    cout << "请输入目标主机端口：";
    cin >> HostPort;
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(HostPort);
    serverAddr.sin_addr.s_addr = inet_addr(HostIP);

    // 向目标主机地址发送打招呼消息
    char *hello = "该主机将目标地址和端口指向了您。";
    int dataSize = strlen(hello);
    int ret = sendto(sock, hello, dataSize, 0, (sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        cout << "打招呼失败！" << endl;
        close(sock);
        exit(1);
    }

    // 这里需要cin.ignore()
    // 原因在于下面的getline会获取到上面cin的最后一个回车，导致执行被跳过。
    // 但如果放在循环内会影响下次循环的字符获取
    cin.ignore();

    while (1)
    {
        // 基于当前系统的当前日期/时间
        time_t now = time(0);
        // 把 now 转换为字符串形式
        char *dt = ctime(&now);

        // 输入要发送的数据
        char *data = "\0";
        string datatmp = "";
        cout << endl
             << dt;
        cout << "【对方:" << HostIP << ":" << HostPort << "】@ ";

        getline(cin, datatmp);
        // cin >> datatmp;
        data = &datatmp[0];

        // 发送数据
        int dataSize = strlen(data);
        int ret = sendto(sock, data, dataSize, 0, (sockaddr *)&serverAddr, sizeof(serverAddr));
        if (ret < 0)
        {
            cout << "打招呼失败！" << endl;
            close(sock);
            exit(1);
        }
    }
    // 清理 Socket
    close(sock);
    return 0;
}