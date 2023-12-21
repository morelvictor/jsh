#include "shell.h"

int shell_pgid;
int shell_terminal;

job **jobs = NULL;

/* Make sure the shell is running interactively as the foreground job
   before proceeding. */


void
init_shell ()
{
	struct sigaction ignore;
	memset(&ignore, 0, sizeof(struct sigaction));
	ignore.sa_handler = SIG_IGN;


  /* See if we are running interactively.  */
  shell_terminal = STDIN_FILENO;

      /* Ignore interactive and job-control signals.  */
//      sigaction (SIGINT, &ignore, NULL);
//      sigaction (SIGQUIT, &ignore, NULL);
//      sigaction (SIGTSTP, &ignore, NULL);
//      sigaction (SIGTTIN, &ignore, NULL);
//      sigaction (SIGTTOU, &ignore, NULL);
//      sigaction (SIGCHLD, &chld, NULL);
      /* Put ourselves in our own process group.  */
      // shell_pgid = getpid();
      // if (setpgid (shell_pgid, shell_pgid) < 0)
        // {
          // perror ("Couldn't put the shell in its own process group");
          // exit (110);
        // }
      // setsid();

      /* Grab control of the terminal.  */
      //tcsetpgrp (shell_terminal, shell_pgid);

}
