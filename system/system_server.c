#include <stdio.h>
#include <sys/prctl.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>


#include <system_server.h>
#include <gui.h>
#include <input.h>
#include <web_server.h>

#define USEC_PER_SECOND         1000000  /* one million */ 
#define USEC_PER_MILLISEC       1000     /* one thousand */
#define NANOSEC_PER_SECOND      1000000000 /* one BILLLION */
#define NANOSEC_PER_USEC        1000     /* one thousand */
#define NANOSEC_PER_MILLISEC    1000000  /* one million */
#define MILLISEC_PER_TICK       10
#define MILLISEC_PER_SECOND     1000


static int toy_timer = 0;
static void sigalrmHandler(int sig)
{
    printf("5 Seconds laters \n");
}
int posix_sleep_ms(unsigned int timeout_ms)
{
    struct timespec sleep_time;

    sleep_time.tv_sec = timeout_ms / MILLISEC_PER_SECOND;
    sleep_time.tv_nsec = (timeout_ms % MILLISEC_PER_SECOND) * (NANOSEC_PER_USEC * USEC_PER_MILLISEC);

    return nanosleep(&sleep_time, NULL);
}

int system_server()
{
    struct itimerspec ts;
    struct sigaction  sa;
    struct sigevent   sev;
    timer_t *tidlist;

    printf("나 system_server 프로세스!\n");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigalrmHandler;
    if (sigaction(SIGRTMAX, &sa, NULL) == -1)
        printf("sigaction");
    
    tidlist = calloc(1, sizeof(timer_t));

    sev.sigev_notify = SIGEV_SIGNAL;    /* Notify via signal */
    sev.sigev_signo = SIGRTMAX;        /* Notify using this signal */
    
    ts.it_value.tv_sec = 5;
    ts.it_value.tv_nsec = 0;

    ts.it_interval.tv_sec = 1;  // 주기 값 설정 (예시로 초기 값과 동일하게 설정)
    ts.it_interval.tv_nsec = 0;
    // ts.it_interval = 5;
    // ts.it_value = 0;
    

    sev.sigev_value.sival_ptr = &tidlist[0];
            /* Allows handler to get ID of this timer */

    if (timer_create(CLOCK_REALTIME, &sev, &tidlist[0]) == -1)
        printf("timer_create");
    if (timer_settime(tidlist[0], 0, &ts, NULL) == -1)
        printf("timer_set");


    while (1) {
        posix_sleep_ms(5000);
    }

    return 0;
}

int create_system_server()
{
    pid_t systemPid;
    const char *name = "system_server";

    printf("여기서 시스템 프로세스를 생성합니다.\n");

    /* fork 를 이용하세요 */
    switch (systemPid = fork()) {
    case -1:
        printf("fork failed\n");
    case 0:
        /* 프로세스 이름 변경 */
        if (prctl(PR_SET_NAME, (unsigned long) name) < 0)
            perror("prctl()");
        system_server();
        break;
    default:
        break;
    }

    return 0;
}