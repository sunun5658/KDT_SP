#include <stdio.h>
#include <sys/wait.h>

#include <system_server.h>
#include <gui.h>
#include <input.h>
#include <web_server.h>


/* 여기서 sigchld 핸들러 구현 */
static void
sigchldHandler(int sig)
{
    /* 구현 */
    int status, savedErrno;
    pid_t childPid;

    /* UNSAFE: This handler uses non-async-signal-safe functions
       (printf(), printWaitStatus(), currTime(); see Section 21.1.2) */

    savedErrno = errno;         /* In case we modify 'errno' */

    printf("%s handler: Caught SIGCHLD %d \n", currTime("%T"),(long) childPid);

    if (childPid == -1 && errno != ECHILD)
        errMsg("waitpid");
    
    printf("%s handler: returning\n", currTime("%T"));
    errno = savedErrno;
}



int main()
{
    pid_t spid, gpid, ipid, wpid;
    int status, savedErrno;
    
    int sigCnt;
    sigset_t blockMask, emptyMask;
    struct sigaction sa;

    /* 여기서 SIGCHLD 시그널  등록 */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigchldHandler;
    if(sigaction(SIGCHLD,&sa,NULL) == -1)
        errExit("sigaction");

    /* Block SIGCHLD to prevent its delivery if a child terminates
    before the parent commences the sigsuspend() loop below */
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGCHLD);
    if (sigprocmask(SIG_SETMASK, &blockMask, NULL) == -1)
        errExit("sigprocmask");
    
    
    printf("메인 함수입니다.\n");
    printf("시스템 서버를 생성합니다.\n");
    spid = create_system_server();
    printf("웹 서버를 생성합니다.\n");
    wpid = create_web_server();
    printf("입력 프로세스를 생성합니다.\n");
    ipid = create_input();
    printf("GUI를 생성합니다.\n");
    gpid = create_gui();

    waitpid(spid, &status, 0);
    waitpid(gpid, &status, 0);
    waitpid(ipid, &status, 0);
    waitpid(wpid, &status, 0);

    return 0;
}
