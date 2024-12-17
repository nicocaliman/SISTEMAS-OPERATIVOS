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

	child_pid = fork();	//proceso padre crea un proceso hijo

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

	//proceso padre
	else
	{
        struct sigaction sa;
        sa.sa_handler = handle_alarm;
        sigemptyset(&sa.sa_mask);	//configura el conjunto de señales a vacio, es decir, inicialmente no hay señales
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

/*
sigemptyset()
se utiliza para inicializar un conjunto de señales, configurándolo para que esté vacío (es decir, que no contenga ninguna señal)

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

permite especificar cómo un programa manejará una señal particular. Es una función más avanzada 
y flexible que el antiguo signal, ya que proporciona un mayor control sobre el comportamiento del programa al recibir señales.

Parámetros
signum: Es el número de la señal que quieres manejar. Las señales están definidas en la biblioteca estándar, como SIGINT, SIGTERM, etc.
act: Es un puntero a una estructura sigaction que define la nueva acción para la señal especificada.
oldact: Es un puntero a una estructura sigaction donde se almacenará la acción anterior asociada a la señal. Si no necesitas esta información, puedes pasar NULL.

Estructura sigaction

struct sigaction {
    void (*sa_handler)(int);           // Puntero a la función que maneja la señal
    void (*sa_sigaction)(int, siginfo_t *, void *); // Alternativa para manejo avanzado
    sigset_t sa_mask;                  // Máscara de señales a bloquear mientras se ejecuta el manejador
    int sa_flags;                      // Opciones adicionales
};

Campos principales:

sa_handler: Puede ser
	Una función de manejo de señales (como un puntero a una función que toma un int).
	SIG_IGN: Ignora la señal.
	SIG_DFL: Restaura el comportamiento por defecto de la señal.
sa_sigaction: Se usa en lugar de sa_handler si el flag SA_SIGINFO está activado, lo que permite manejar señales con más información.
sa_mask: Es un conjunto de señales que se bloquearán automáticamente mientras se ejecuta el manejador.
sa_flags: Opciones como:
	SA_RESTART: Reinicia ciertas llamadas al sistema interrumpidas por esta señal.
	SA_SIGINFO: Usa sa_sigaction en lugar de sa_handler.

 Retorno
 
Devuelve 0 si la operación es exitosa.
Devuelve -1 en caso de error, estableciendo la variable global errno.

void (*signal(int signum, void (*handler)(int)))(int);

se utiliza para definir cómo un programa debería responder cuando recibe una señal específica. 
Es una versión más sencilla y menos flexible que sigaction, aunque cumple con el mismo propósito básico.

Parámetros
signum: Es el número de la señal que se desea manejar. Estas señales están predefinidas, como SIGINT, SIGTERM, SIGKILL, etc.
handler: Es un puntero a la función que actuará como manejador de la señal. Puede ser:
Una función definida por el usuario que toma un argumento de tipo int (el número de la señal recibida).
	SIG_IGN: Para ignorar la señal.
	SIG_DFL: Para restablecer el comportamiento predeterminado de la señal.

 Retorno
Devuelve un puntero al manejador de señales previamente configurado (si lo hubo).
Si hay un error, devuelve SIG_ERR.

unsigned int alarm(unsigned int seconds);
se utiliza para programar una alarma que envía la señal SIGALRM al proceso después de un número especificado de segundos.

#include <sys/types.h>
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int *status, int options);
se utiliza para esperar que un proceso hijo específico cambie de estado. 
Es una versión más flexible de wait, ya que permite esperar por un hijo en particular, entre otras opciones.

Parámetros
pid: Indica qué proceso hijo debe ser esperado:

	> 0: Espera al proceso hijo con el PID especificado.
	-1: Espera a cualquier proceso hijo (comportamiento similar a wait).
	0: Espera a cualquier hijo del mismo grupo de procesos.
	< -1: Espera a cualquier hijo cuyo grupo de procesos sea el valor absoluto de pid.

status: Un puntero a un entero donde se almacena el estado del proceso hijo. Se utiliza con macros como WIFEXITED, WEXITSTATUS, WIFSIGNALED, etc.

options: Modifica el comportamiento de la función. Puede ser:

	0: Comportamiento normal; espera a que el proceso hijo cambie de estado.
	WNOHANG: No bloquea el proceso padre; retorna inmediatamente si no hay cambios de estado.
	WUNTRACED: Retorna si el proceso hijo se detiene (solo para procesos que no están en segundo plano).
	WCONTINUED: Retorna si el proceso hijo reanuda su ejecución tras ser detenido.

 WIFEXITED y WIFSIGNALED son macros que se utilizan para analizar el estado de un proceso hijo después de que cambia de estado, 
 generalmente tras una llamada a funciones como wait o waitpid. Estas macros ayudan a determinar cómo terminó el proceso hijo.

 WIFEXITED
Propósito: Verifica si el proceso hijo terminó normalmente, es decir, mediante una llamada a exit, _exit o retornando del main.

WIFSIGNALED
Propósito: Verifica si el proceso hijo terminó porque recibió una señal no manejada (por ejemplo, SIGKILL, SIGTERM).


#include <sys/types.h>
#include <signal.h>

int kill(pid_t pid, int sig);

 se utiliza para enviar señales a procesos o grupos de procesos. 
 Puede enviar cualquier señal válida, como SIGTERM, SIGKILL, SIGUSR1, entre otras. Es ampliamente utilizada para controlar la ejecución de procesos desde el programa.

 Parámetros
pid:

	> 0: Envía la señal al proceso con el PID especificado.
	0: Envía la señal a todos los procesos del mismo grupo de procesos que el proceso que llama a kill.
	< -1: Envía la señal a todos los procesos en el grupo de procesos con el ID absoluto de pid (es decir, -pid).
	-1: Envía la señal a todos los procesos a los que el usuario tiene permiso para enviar señales, excepto a sí mismo y al proceso init.
sig:

Especifica la señal que se enviará. Puede ser una constante definida en <signal.h>, como:
	SIGTERM: Solicita una terminación "limpia" del proceso (comportamiento por defecto).
	SIGKILL: Termina el proceso inmediatamente (no puede ser ignorada ni manejada).
	SIGSTOP: Detiene el proceso (no puede ser ignorada ni manejada).
	SIGUSR1, SIGUSR2: Señales definidas por el usuario.
Si sig = 0, no se envía una señal, pero se verifica si el proceso existe y si el proceso que llama tiene permisos para enviar señales.
*/
