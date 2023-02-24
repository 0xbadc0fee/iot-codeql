#include <stdlib.h> //exit, NULL
#include <stdio.h> //stdin, stdout, stderr
#include <string.h> //strerro
#include <syslog.h> //syslog(), openlog(), closelog()
#include <signal.h> // signal()
#include <unistd.h> //getpid(),chdir(),close(), fork(), access(), usleep()
#include <sys/stat.h> //umask()
#include <time.h> // time_t()
#include <errno.h> //errno()







#define ERROR_FORMAT "error no: %s"
#define DAEMON_NAME "mod4logd"
#define true 1
#define LOG_FILE_NAME "/var/log/messages"

enum error_t{OK=0, ERROR_TIME_FORMAT, ERROR_USLEEP_FORMAT, ERR_CHDIR,
  ERR_DISABLE_PRINT, ERR_FORK, ERR_NO_LOG_FILE, ERR_SAVE_VAR, ERR_SETSID, ERR_TIME,
  ERR_USLEEP, ERR_WTF};






static void _signal_handler(const int signal) {
  switch(signal) {
  case SIGHUP:
    break;
  case SIGTERM:
    syslog(LOG_INFO, "received SIGTERM, exiting.");
    closelog();
    exit(OK);
    break;
  default:
    syslog(LOG_INFO, "received unhandled signal");
  }
}

//
//
static void _do_work(void) {
  for(int i = 0; true; i++) {
    syslog(LOG_INFO,  "interaction: %d", i);
    sleep(1);
  }
}

int main(void){
  //
  //
  //
  //
  //
  //
  //
  //
  openlog(DAEMON_NAME, LOG_PID | LOG_NDELAY | LOG_NOWAIT, LOG_DAEMON);
  syslog(LOG_INFO, "starting sampled");

  //
  //
  pid_t pid = fork();

  //
  //
  if (pid < 0) {
    syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
    return ERR_FORK;
  }

  //
  //
  if (pid > 0) {
    return OK;
  }

  //
  //
  if(setsid() < -1) {
    syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
    return ERR_SETSID;
  }

  //
  //
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  //
  //
  //
  umask(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  //
  //
  if (chdir("/") < 0){
    syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
    return ERR_CHDIR;
  }

  //
  //
  /*add check for log file presence*/
  if(access(LOG_FILE_NAME, F_OK) != 0){
    syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
    return ERR_NO_LOG_FILE;
  }


  signal(SIGTERM, _signal_handler);
  signal(SIGHUP, _signal_handler);

  //
  //
  //
  //
  _do_work();

  //
  //
  //
  return ERR_WTF;
}








