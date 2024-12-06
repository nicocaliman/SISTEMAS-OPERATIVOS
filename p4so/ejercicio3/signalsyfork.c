#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

/*programa que temporiza la ejecución de un proceso hijo */

pid_t child_pid;

void handle_alarm(int sig)
{	
	kill(child_pid, SIGKILL);		
}

void usage(char *progname)
{
	fprintf(stderr, "Usage: %s program\n", progname);
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;

	if(sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	child_pid = fork();

	//proceso hijo
	if(child_pid == 0)
	{
		execvp(argv[1], &argv[1]);	//reemplazar proceso hijo por el comando especificado
	}	

	else if(child_pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	else
	{
        struct sigaction sa;
        sa.sa_handler = handle_alarm;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;

        // Establecer el manejador de la señal SIGALRM
        sigaction(SIGALRM, &sa, NULL);

        // Ignorar la señal SIGINT
        signal(SIGINT, SIG_IGN);

        // Programar una alarma para 5 segundos
        alarm(5);

        // Esperar a que el hijo termine
        int status;
        waitpid(child_pid, &status, 0);

        // Comprobar cómo terminó el proceso hijo
        if (WIFEXITED(status)) 
            printf("El proceso hijo terminó normalmente.\n");
        else if (WIFSIGNALED(status)) 
            printf("El proceso hijo fue terminado por la señal.\n");        
	}
	
	return 0;
}
