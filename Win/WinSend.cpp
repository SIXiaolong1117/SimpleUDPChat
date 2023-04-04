// MIT License
// by Si Xiaolong (GitHub:@Direct5dom E-mail:sixiaolong2021@gmail.com)

#include <iostream>
#include <winsock2.h>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

string resolveDNS(string domain_s)
{
    char *domain = &domain_s[0];

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "Winsock 初始化失败！" << endl;
        return "解析失败";
    }

    ADDRINFOA hints;
    ZeroMemory(&hints, sizeof(hints));
    // 使用IPv4地址
    hints.ai_family = AF_INET;
    // 使用TCP协议
    hints.ai_socktype = SOCK_STREAM;

    ADDRINFOA *result = NULL;
    int iResult = getaddrinfo(domain, NULL, &hints, &result);
    if (iResult != 0)
    {
        std::cerr << "getaddrinfo 失败并出现错误：" << iResult << std::endl;
        WSACleanup();
        return "解析失败";
    }

    char ipstr[INET_ADDRSTRLEN];
    for (ADDRINFOA *ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        void *addr;
        // 获取IP地址
        if (ptr->ai_family == AF_INET)
        {
            sockaddr_in *ipv4 = (sockaddr_in *)ptr->ai_addr;
            addr = &(ipv4->sin_addr);
        }
        else
        {
            sockaddr_in6 *ipv6 = (sockaddr_in6 *)ptr->ai_addr;
            addr = &(ipv6->sin6_addr);
        }
        // 将IP地址转换为字符串
        inet_ntop(ptr->ai_family, addr, ipstr, sizeof(ipstr));
        std::cout << domain << " 解析为 " << ipstr << std::endl;
    }

    freeaddrinfo(result);
    WSACleanup();

    return ipstr;
}

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

    // 设置目标主机地址和端口号
    char *HostIP;
    int HostPort;
    string HostIPtmp;

    cout << "请输入目标主机IP：";
    cin >> HostIPtmp;
    HostIPtmp = resolveDNS(HostIPtmp);
    HostIP = &HostIPtmp[0];
    cout << "设定的目标主机IP：" << HostIP << endl;

    cout << "请输入目标主机端口：";
    cin >> HostPort;
    cout << "设定的目标主机端口：" << HostPort << endl;

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(HostPort);
    serverAddr.sin_addr.s_addr = inet_addr(HostIP);

    // 向目标主机地址发送打招呼消息
    char *hello = "该主机将目标地址和端口指向了您。";
    int dataSize = strlen(hello);
    int ret = sendto(sock, hello, dataSize, 0, (sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret == SOCKET_ERROR)
    {
        cout << "打招呼失败！" << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
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
        string dt = ctime(&now);
        dt.pop_back();

        // 输入要发送的数据
        char *data = "\0";
        string datatmp = "";
        cout << endl
             << "\033[36m┌[\033[0m\033[32m" << dt << "\033[0m\033[36m] - [\033[0m\033[33m目标：" << HostIP << ":" << HostPort << "\033[0m\033[36m]\033[0m" << endl;
        cout << "\033[36m└[@\033[0m ";

        getline(cin, datatmp);
        // cin >> datatmp;
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
    }

    // 关闭 Socket 和 Winsock
    closesocket(sock);
    WSACleanup();
    return 0;
}