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

  /* Fork off the parent process */
  pid = fork();

  /* An error occurred */
  if (pid < 0)
      exit(EXIT_FAILURE);

  printf("Forked successful: %d\n", pid);
  /* Success: Let the parent terminate */
  if (pid > 0)
      exit(EXIT_SUCCESS);

  printf("Terminating parent successful\n");
  /* On success: The child process becomes session leader */
  if (setsid() < 0)
      exit(EXIT_FAILURE);

  /* Catch, ignore and handle signals */
  //TODO: Implement a working signal handler */
  signal(SIGCHLD, SIG_IGN);
  signal(SIGHUP, SIG_IGN);

  /* Set new file permissions */
  umask(0);

  /* Change the working directory to the root directory */
  /* or another appropriated directory */
  if (chdir("/") < 0) {
    exit(EXIT_FAILURE);
  }
  printf("Change directory successful\n");
  /* Close all open file descriptors */
  int x;
  for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
  {
    close (x);
  }
  /* Open the log file */
  openlog ("autodim", LOG_PID, LOG_DAEMON);

  syslog (LOG_NOTICE, "autodim started.");

  if (autodim())  exit(EXIT_FAILURE);

  syslog (LOG_NOTICE, "autodim terminated.");
  closelog();

  return EXIT_SUCCESS;
}
