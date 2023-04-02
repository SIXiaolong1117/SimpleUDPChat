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

    // 设置服务器地址和端口号
    char *HostIP = "127.0.0.1";
    int HostPort = 1234;
    string HostIPtmp;
    cout << "请输入目标主机IP：" << endl;
    cin >> HostIPtmp;
    HostIP = &HostIPtmp[0];
    cout << "请输入目标主机端口：" << endl;
    cin >> HostPort;
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1234);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 输入要发送的数据
    char *data = "\0";
    string datatmp;
    cout << "请输入要发送的内容：" << endl;
    cin >> datatmp;
    data = &datatmp[0];

    // 发送数据
    int dataSize = strlen(data);
    int ret = sendto(sock, data, dataSize, 0, (sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret == SOCKET_ERROR)
    {
        cout << "发送数据失败！" << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    cout << "发送数据成功！" << endl;
    cout << "发送的数据为：" << data << endl;

    // 关闭 Socket 和 Winsock
    closesocket(sock);
    WSACleanup();

    return 0;
}