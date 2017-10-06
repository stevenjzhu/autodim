#include <autodim.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <fcntl.h>

int main(void)
{
  
  pid_t pid;

  pid = fork();

  if (pid < 0)
      exit(EXIT_FAILURE);

  printf("Forked successful: %d\n", pid);

  if (pid > 0)
      exit(EXIT_SUCCESS);

  printf("Terminating parent successful\n");

  if (setsid() < 0)
      exit(EXIT_FAILURE);

  //TODO: Implement signal handler
  signal(SIGCHLD, SIG_IGN);
  signal(SIGHUP, SIG_IGN);

  umask(0);

  if (chdir("/") < 0) {
    exit(EXIT_FAILURE);
  }
  printf("Change directory successful\n");


  for (int i = getdtablesize(); i>=0; i--)
    close (i);

  openlog ("autodim", LOG_PID, LOG_DAEMON);

  syslog (LOG_NOTICE, "autodim started.");


  if (autodim())  exit(EXIT_FAILURE);

  syslog (LOG_NOTICE, "autodim terminated.");
  closelog();

  return EXIT_SUCCESS;
}
