#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <signal.h>
#include <setjmp.h>

// DECLARE EXTERNAL FUNCTIONS
#if defined(__linux__)
extern  char            *strdup(const char *);
#endif

#define	EXIT_TIMEOUT		59
#define	DEF_PATH_EXITSTATUS	"/tmp/run4-exitstatus"
#define	DEF_MAX_SECONDS		10

static	jmp_buf			env;
static	int			pid;
static	int			status;

static  char                    *path_exitstatus    = DEF_PATH_EXITSTATUS;
static	int			max_seconds         = DEF_MAX_SECONDS;

static void took_too_long(int sig)
{
    longjmp(env, 1);
}

static void write_exitstatus(int s)
{
    FILE *fp    = fopen(path_exitstatus, "w");
    if(fp) {
        fprintf(fp, "%i\n", s);
        fclose(fp);
    }
}

static void wait4it(void)
{
    while(wait(&status) > 0)
	;

    if(WIFEXITED(status))
	status =  WEXITSTATUS(status);
    else if(WIFSIGNALED(status))
	status =  WTERMSIG(status);
    else
	status = 1;
    write_exitstatus(status);
}

static int start(int argc, char *argv[])
{
#if	defined(__linux__)
    extern int kill(pid_t pid, int sig);
#endif

    if(setjmp(env)) {
	setitimer(ITIMER_REAL, NULL, NULL);
	kill(pid, 9);
	wait4it();

        write_exitstatus(EXIT_TIMEOUT);
	exit(EXIT_TIMEOUT);
	return 1;
    }

    switch ((pid = fork())) {
    case -1:
	exit(2);
	break;

    case 0:
	execvp(argv[0], argv);
	exit(2);
	break;

    default: {
	struct	itimerval	timeout;

	timeout.it_interval.tv_sec	= 0;
	timeout.it_interval.tv_usec	= 0;
	timeout.it_value.tv_sec		= max_seconds;
	timeout.it_value.tv_usec	= 0;

	setitimer(ITIMER_REAL, &timeout, NULL);
	signal(SIGALRM, took_too_long);

	wait4it();
	exit(status);
	break;
      }
    }

    setitimer(ITIMER_REAL, NULL, NULL);
    return 0;
}

int main(int argc, char *argv[])
{
    char *env;
    int  rtn = 1;

    env   = getenv("RUN4_EXITSTATUS");
    if(env != NULL) {
        path_exitstatus = strdup(env);
    }
    unlink(path_exitstatus);

    env   = getenv("RUN4_TIMEOUT");
    if(env != NULL) {
        max_seconds = atoi(env);
        if(max_seconds < 1)
            max_seconds	= DEF_MAX_SECONDS;
    }

    if(argc > 1) {
	--argc; ++argv;
	rtn = start(argc, argv);
    }
    return rtn;
}
