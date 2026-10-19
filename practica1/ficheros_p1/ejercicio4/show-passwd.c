#include <stdio.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
#include "defs.h"

/**
 * @brief  Make a copy of existing string allocating memory accordingly
 *
 * @param original
 * @return new string that is a clone of original
 **/
static inline char* clone_string(char* original)
{
	char* copy;
	copy=malloc(strlen(original)+1);
	strcpy(copy,original);
	return copy;
}


/**
 * @brief Parse /etc/passwd file and returns its contents as an array of entries.
 * The number of entries in the array is returned vie the nr_entries parameter.
 *
 * @param options
 * @param nr_entries
 * @return array of entries (passwd_entry_t*) and entry count (nr_entries parameter)
 */
passwd_entry_t* parse_passwd(struct options* options, int* nr_entries)
{
	FILE* passwd;
	char line[MAX_PASSWD_LINE+1];
	passwd_entry_t* entries;
	passwd_entry_t* cur_entry;
	int line_count=0;
	char* token;
	char* lineptr;
	token_id_t token_id;
	int entry_idx;
	int entry_count=0;
	int cur_line;

	/*
		if ((passwd=fopen("/etc/passwd","r"))==NULL) {
		fprintf(stderr, "/etc/passwd could not be opened: ");
		perror(NULL);
		return NULL;
	}
	*/

	//abrimos el fichero declarado en la opcion -i o /etc/passwd por defecto
	if ((passwd=fopen(options->infile,"r"))==NULL) {
		fprintf(stderr, "/etc/passwd could not be opened: ");
		perror(NULL);
		return NULL;
	}


	/* Figure out number of lines */
	while (fgets(line, MAX_PASSWD_LINE + 1, passwd) != NULL){
		line_count++;
		/* Discard lines that begin with # */
		if (line[0]!='#')
			entry_count++;
	}

	/* Rewind position indicator*/
	fseek(passwd,0,SEEK_SET);

	entries=malloc(sizeof(passwd_entry_t)*entry_count);
	/* zero fill the array of structures */
	memset(entries,0,sizeof(passwd_entry_t)*entry_count);

	/* Parse file */
	entry_idx=0;
	cur_line=1;
	while (fgets(line, MAX_PASSWD_LINE + 1, passwd) != NULL) {
		
		/* Discard lines that begin with # */
		if (line[0]=='#'){
			cur_line++;
			continue;
		}

		/* Point to the beginning of the line */
		lineptr=line;
		token_id=LOGIN_NAME_IDX;
		cur_entry=&entries[entry_idx];

		while((token = strsep(&lineptr, ":"))!=NULL) {
			switch(token_id) {
			case LOGIN_NAME_IDX:
				strcpy(cur_entry->login_name,token);
				break;
			case ENCRYPTED_PASS_IDX:
				//cur_entry->optional_encrypted_passwd=clone_string(token);
				cur_entry->optional_encrypted_passwd=strdup(token);
				break;
			case UID_IDX:
				if (sscanf(token,"%d",&cur_entry->uid)!=1) {
					fprintf(stderr, "Warning: Wrong format for UID field in line %d of /etc/passwd\n",cur_line);
					cur_entry->uid=0;
				}

				break;
			case GID_IDX:
				if (sscanf(token,"%d",&cur_entry->gid)!=1) {
					fprintf(stderr, "Warning: Wrong format for GID field in line %d of /etc/passwd\n",cur_line);
					cur_entry->gid=0;
				}
				break;
			case USER_NAME_IDX:
				//cur_entry->user_name=clone_string(token);
				cur_entry->user_name=strdup(token);
				break;
			case USER_HOME_IDX:
				//cur_entry->user_home=clone_string(token);
				cur_entry->user_home=strdup(token);

				break;
			case USER_SHELL_IDX:
				/* remove new line from token */
				token[strlen(token)-1]='\0';
				//cur_entry->user_shell=clone_string(token);
				cur_entry->user_shell=strdup(token);
				break;
			default:
				break;
			}
			token_id++;
		}
		if (token_id!=NR_FIELDS_PASSWD) {
			fprintf(stderr, "Could not process all tokens from line %d of /etc/passwd\n",entry_idx+1);
			return NULL;
		}
		cur_line++;
		entry_idx++;
	}
	(*nr_entries)=entry_count;
	return entries;
}

/**
 * @brief Free up the array of entries, including the 
 * memory of dynamically allocated strings
 * 
 * @param entries 
 * @param nr_entries 
 */
static void free_entries(passwd_entry_t* entries, int nr_entries)
{
	int i=0;
	passwd_entry_t* entry;

	for (i=0; i<nr_entries; i++) {
		entry=&entries[i]; /* Point to current entry */
		free(entry->optional_encrypted_passwd);
		free(entry->user_name);
		free(entry->user_home);
		free(entry->user_shell);
	}

	free(entries);
}

/**
 * @brief This function takes care of invoking the parser, 
 * and then displays the contents of /etc/passwd based on the 
 * selected output mode
 * 
 * @param options 
 * @return int 
 */
static int show_passwd(struct options* options)
{
	int nr_entries;
	int i;
	passwd_entry_t* entries=parse_passwd(options,&nr_entries);

	if (!entries)
		return EXIT_FAILURE;

	for (i=0; i<nr_entries; i++) {
		passwd_entry_t* e=&entries[i]; /* Point to current entry */
		switch (options->output_mode) {
		case VERBOSE_MODE:
			fprintf(options->outfile,"[Entry #%d]\n",i);
			fprintf(options->outfile,"\tlogin=%s\n\tenc_pass=%s\n\t"
			        "uid=%d\n\tgid=%d\n\tuser_name=%s\n\t"
			        "home=%s\n\tshell=%s\n",
			        e->login_name, e->optional_encrypted_passwd,
			        e->uid, e->gid,e->user_name,
			        e->user_home, e->user_shell);
			break;
		case PIPE_MODE:
			fprintf(options->outfile,"|%s|%s|%d|%d|%s|%s|%s|\n",
			        e->login_name, e->optional_encrypted_passwd,
			        e->uid, e->gid,e->user_name,
			        e->user_home, e->user_shell);
		
					//agregamos nuevo case para CSV mode
		case COMA_MODE:
			fprintf(options->outfile,"%s,%s,%d,%d,%s,%s,%s\n",
			        e->login_name, e->optional_encrypted_passwd,
			        e->uid, e->gid,e->user_name,
			        e->user_home, e->user_shell);
		}

	}
	free_entries(entries,nr_entries);
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	int retCode, opt;
	struct options options;

	/* Initialize default values for options */
	options.outfile=stdout;
	options.output_mode=VERBOSE_MODE;
	options.infile = "/etc/passwd";

	/* Parse command-line options */
	while((opt = getopt(argc, argv, "hvpo:i:c")) != -1) {
		switch(opt) {
		case 'h':
			fprintf(stderr,"Usage: %s [ -h | -v | -p | -o <output_file> | -i <input_file> | -c ]\n",argv[0]);
			exit(0);
		case 'v':
			options.output_mode=VERBOSE_MODE;
			break;
		case 'p':
			options.output_mode=PIPE_MODE;
			break;
		case 'o':
			if ((options.outfile=fopen(optarg,"wx"))==NULL) {
				fprintf(stderr, "The output file %s could not be opened: ",
				        optarg);
				perror(NULL);
				exit(EXIT_FAILURE);
			}
			break;

		case 'i':
			options.infile = optarg;
			break;
		
		case 'c':
			options.output_mode = COMA_MODE;
			break;

		default:
			exit(EXIT_FAILURE);
		}
	}

	retCode=show_passwd(&options);
	exit(retCode);
}

/*
La funcion de clone_string() es reservar memoria para hacer la copia de una cadena de caracteres

Si hiciera cur_entry->user_name = token;, todas las estructuras terminarían apuntando a la misma variable temporal line. Al salir de parse_passwd(), 
esa memoria de la pila se destruiría, dejando punteros colgados (dangling pointers) y corrompiendo los datos.
*/

/*
Hacer cur_entry->user_name = token; no copia el texto, solo copia la dirección de memoria a la que apunta token.

    El problema es que token apunta a una subcadena que vive dentro del array local line (char line[MAX_PASSWD_LINE+1]), ubicado en la pila (stack) de la función parse_passwd().

    Consecuencia 1: El buffer line se reutiliza y sobrescribe en cada vuelta del bucle fgets(). Todas las entradas apuntarían al mismo texto (la última línea leída).

    Consecuencia 2: Al finalizar parse_passwd(), la memoria de line en la pila se libera. Todos los campos de la estructura quedarían apuntando a memoria inválida (punteros colgados / dangling pointers).
*/

/*
La posibilidad de usar strcpy() depende exclusivamente de cómo está definido el campo dentro de la estructura passwd_entry_t (defs.h):

Caso A: Cuando el campo es un array (ejemplo: char login_name[MAX_LOGIN_NAME];)

    SÍ se puede usar strcpy(): La estructura ya reservó previamente 50 bytes de memoria continua. Hacer strcpy(cur_entry->login_name, token) escribe los caracteres directamente dentro de esa memoria propia.

Caso B: Cuando el campo es un puntero (ejemplo: char* user_name;)

    NO se puede usar strcpy() directamente: La estructura solo reserva espacio para la dirección de memoria (8 bytes en 64 bits), pero el puntero apunta a NULL o a memoria no inicializada.

    Consecuencia: Hacer strcpy(cur_entry->user_name, token) intentará escribir en una dirección inválida, provocando un fallo de segmentación (Segmentation Fault). Por eso es imprescindible llamar primero a clone_string() para reservar la memoria con malloc().
*/

/*
Tras invocaciones sucesivas de strsep(&lineptr, ":"), la cadena original almacenada en line y el puntero auxiliar lineptr sufren dos modificaciones principales en memoria:
1. Reemplazo de delimitadores por caracteres nulos ('\0')

Cada vez que strsep() encuentra un carácter delimitador (el carácter : en este caso), sobrescribe esa posición de memoria con un carácter nulo ('\0').

Esto fragmenta la memoria continua del buffer line en múltiples subcadenas independientes terminadas en \0, sin necesidad de mover o copiar caracteres a otra ubicación.
2. Desplazamiento del puntero de trabajo (lineptr)

strsep() modifica directamente el valor guardado en el puntero lineptr (por eso se le pasa su dirección &lineptr):

    Hace avanzar a lineptr para que apunte al primer carácter del siguiente token (justo después del : reemplazado por \0).

    Cuando no quedan más delimitadores en la cadena, strsep() asigna NULL a lineptr.
*/