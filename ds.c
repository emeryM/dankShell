// Sup dude dankShell is dank
/*  myshell.c
         This program is a simple command interpreter that uses execlp() to 
         execute commands typed in by the user.
     */
     #include <stdio.h>
     #define  EVER   ;;

     int main()
     {
        int process;
        char line[81];

        for (EVER)
           {
           fprintf(stderr, "cmd: ");
              if ( gets (line) == (char *) NULL)      /* blank line input */
                 exit (0);

        /* create a new process */

           process = fork ();

           if (process > 0)             /* parent */
              wait ((int *) 0);       /* null pointer - return value not saved */
           else if (process == 0)       /* child */
              {                         /* execute program */
              execlp (line, line, (char *) NULL);
                                     /* some problem if exec returns */
              fprintf (stderr, "Can't execute %s\n", line);
              exit (1);
              }
           else if ( process == -1)     /* can't create a new process */
              {
              fprintf (stderr, "Can't fork!\n");
              exit (2);
              }
           }
     }

