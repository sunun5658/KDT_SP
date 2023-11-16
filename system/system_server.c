#include <stdio.h>
#include <sys/prctl.h>

#include <system_server.h>
#include <gui.h>
#include <input.h>
#include <web_server.h>
#include <unistd.h>

int system_server()
{
    printf("나 system_server 프로세스!\n");

    while (1) {
        sleep(1);
    }

    return 0;
}

int create_system_server()
{
    pid_t systemPid;
    const char *name = "system_server";
    int status;
    printf("여기서 시스템 프로세스를 생성합니다.\n");

    /* fork 를 이용하세요 */
    if(systemPid = fork())
    {
        pid_t childpid = wait(&status);
    }
    else
    {
        system_server();
        exit(EXIT_SUCCESS);
    }
    return 0;
}
