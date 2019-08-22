#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {
	pid_t pid;
	char mode = 0;
	if (argc < 2) // certifies than has excecution arguments
	{
		fprintf(stderr, "uso: %s <modo> <output>\n", argv[0]);
		fprintf(stderr, "ouput padrão: usage.out\n");
		fprintf(stderr, "digite %s help para mais detalhes\n", argv[0]);
		exit(-1);
	}
	else if (strcmp(argv[1], "help") == 0) // help options
	{
		printf("O programa monitora o uso da memória e da UCP de uma simulação de uso intenso dos mesmos.\n");
		printf("uso: %s [modo] [arquivo de despejo]\n", argv[0]);
		printf("\tmodos:\tucp\t<uso intenso da ucp>\n\t\tucp-mem\t<uso intenso da ucp e memória principal>\n");
		printf("\toutput:\tO ouput padrão de arquivo é o arquivo usage.out\n\t\tCaso queira outro arquivo, especificá-lo no argumento de terminal.\n");
		printf("\t\tCaso não queira um arquivo de output, digitar -no no argumento de terminal.\n");
		exit(0);
	}
	else
	{
		// this part of the code creates a prevent way about the invalid options that was put by user
		if (strcmp(argv[1], "ucp-mem") == 0) // intern use by ucp and memory (using flag)
			mode = 1;
		else if (strcmp(argv[1], "ucp") == 0) // intern use by memory (using flag)
			mode = 2;
		else
		{
			fprintf(stderr, "Opções inválidas, digite \"%s help\" (sem aspas) para saber mais.\n", argv[0]);
			exit(-1);
		}
		pid = fork(); // process reapplication
	}

	// if fork doesnt work, end the program with (-1) error code
	if (pid < 0)
	{ 
		perror("Erro: ");
		exit(-1);
	}

	char buffer[64];
	if(pid != 0) // if i am the father process
	{
		short to_file = 1;
		// a pattern out file is open or another specified by user
		FILE* data_out;
		if (argc < 3)
			data_out = fopen("./usage.out", "a");
		else
		{
			if (strcmp(argv[2], "-no") != 0)
				data_out = fopen(argv[2], "a");
			else to_file = 0;
		}

		// the repetition work executes 10 times the child process monitoring
		if (to_file == 1) fprintf(data_out, "\nInicializando monitoramento... (1 seg por chamada)\n");
		for (int i = 0; i < 10; i++)
		{
			sleep(1); //waits a secod
			FILE* pipe;
			
			// creates a variable with a command that will be used in pipe
			snprintf(buffer, sizeof(buffer), "ps -o rss,pcpu -p %i | tail -n1", pid);

			// creates a file stream of bash out  (pipe bash out) 
			pipe = popen((const char*)buffer, "r");
			if (pipe == NULL) // checking popen() error
			{
				perror("pipe");
				exit(1);
			}

			// gets the values from memory (Kib) and UCP usage (%)
			float uso_mem = 0, uso_cpu = 0;
			fscanf(pipe, "%f %f", &uso_mem, &uso_cpu);

			uso_mem *= 1.024; // turns from KiB into KB
			pclose(pipe); // how pipe will be not used, can close it 

			char formato[64] = "Uso da UCP: %.2f%%\tUso da memória: %.2f KB\n";
			printf(formato, uso_cpu, uso_mem); // writes in terminal the line getten in bash pipe

			// writes in out file of this process
			if (to_file == 1) fprintf(data_out, formato, uso_cpu, uso_mem);
		}

		if (to_file == 1) fclose(data_out); // closes the out file of process

		// kill the child process before the previous loop (during 10 seconds)
		snprintf(buffer, sizeof(buffer), "kill %i", pid);
		system(buffer);
	}
	else	// if not, i am the child process ( pid  == 0)
	{
		if (mode == 1) // intern usage of cpu and memory
		{
			for(int i = 0;;i++) 
			{
				i += rand();
				if (i > i % rand() + i / rand())
					i = 2 * rand() + 43;
				i *= rand();
				i /= rand();
				for (int j = 0; j < 100000; j++);
				malloc(1);
			}
		}
		else if (mode == 2) //only intern cpu usage
		{
			for(int i = 0;;i++)
			{
				i += rand();
				if (i > i % rand() + i / rand())
					i = 2 * rand() + 43;
				i *= rand();
				i /= rand();
				for (int j = 0; j < 100000; j++);
			}
		}
	}

	puts("Terminado.");
	return 0; // ends the process returning 0 ( success)
}
