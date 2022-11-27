#include <stdio.h>
#include <pthread.h>
#include "common.h"


int main(int argc, char **argv) {

    if (argc != 2) {
        usage();
        return 1;
    }
    int iResult = wsa_init();
    if (iResult != NO_ERROR) {
        return 1;
    }
    char *ip = argv[argc - 1];

    printf("%-10s  %s\n", "PORT", "STATUS");

    pthread_attr_t t_c;            //线程属性
    pthread_attr_init(&t_c);                                    //初始化线程属性
    pthread_attr_setdetachstate(&t_c, PTHREAD_CREATE_DETACHED); //设置线程属性

    int group_num = 65535 / thread_num + (65535 % thread_num > 0 ? 1 : 0);
    pthread_t t[thread_num];
    for (int i = 0; i < thread_num; ++i) {
        int start_port = 1 + i * group_num;
        int end_port = group_num + i * group_num;
        if (end_port > 65535)end_port = 65535;
        struct ScanData sd;
        strcpy(sd.ip, ip);
        sd.start_port = start_port;
        sd.end_port = end_port;
        if (pthread_create(&t[i], &t_c, threadscan, (void *) &sd) != 0) {
            printf("\nCREATE THREAD ERROR\n");
        } else {
            pthread_join(t[i], NULL);
        }
        Sleep(10);
//        printf("%d - %d\n", start_port, end_port);
    }
    pthread_exit(NULL);
    WSACleanup();
    return 0;
}

