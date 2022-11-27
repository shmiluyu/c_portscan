//
// Created by shmiluyu on 2022/11/27.
//

#ifndef PORTSCAN_COMMON_H
#define PORTSCAN_COMMON_H

#include <stdio.h>
#include <winsock2.h>

int timeout = 1000;
int thread_num = 50;

void scan_range(char *ip, int start_port, int end_port);

void usage() {
    printf("Usage:\n"
           "program TargetIp\n"
           "Example:portscan 192.168.1.254\n");
}

struct ScanData {
    char ip[20];
    int start_port;
    int end_port;
};

int wsa_init() {
    WSADATA wd;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wd);
    if (iResult != NO_ERROR) {
        printf("wsa startup error!\n");
        return iResult;
    }
    if (HIBYTE(wd.wVersion) != 2 && LOBYTE(wd.wVersion) != 2) {
        printf("wsa version error!\n");
        WSACleanup();
        return 1;
    }
    return NO_ERROR;
}

void *threadscan(void *sd) {
    struct ScanData *pa = (struct ScanData *) sd;
//    printf("%d - %d started!\n", pa->start_port, pa->end_port);
    scan_range(pa->ip, pa->start_port, pa->end_port);
    printf("%d - %d finished!\n", pa->start_port, pa->end_port);
    pthread_exit(NULL);
    return NULL;
}

void scan_range(char *ip, int start_port, int end_port) {
    SOCKET s;
    SOCKADDR_IN sockaddrIn;
    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_addr.S_un.S_addr = inet_addr(ip);

    int len = sizeof(int);
    int error = -1;
    for (int i = start_port; i <= end_port; i++) {
        struct timeval tm;
        fd_set set;
        unsigned long ul = 1;
        s = socket(AF_INET, SOCK_STREAM, 0);
        ioctlsocket(s, FIONBIO, &ul); //set as non-blocking
        sockaddrIn.sin_port = htons(i);
        if (connect(s, (SOCKADDR *) &sockaddrIn, sizeof(sockaddrIn)) == SOCKET_ERROR) {
            tm.tv_sec = 0; // timeout 1s
            tm.tv_usec = timeout;
            FD_ZERO(&set);
            FD_SET(s, &set);
            if (select(s + 1, NULL, &set, NULL, &tm) > 0) {
                getsockopt(s, SOL_SOCKET, SO_ERROR, (char *) &error, /*(socklen_t *)*/&len);
                if (error == 0) {
                    printf("%-10d  %s\n", i, "OPEN");
                }
            }
        }
        ul = 0;
        ioctlsocket(s, FIONBIO, &ul); //set as blocking
        closesocket(s);
    }
}

#endif //PORTSCAN_COMMON_H
