# Trabalho AB2 de IaC 2019.1

Trabalho sobre a visualização do uso de recursos da UCP (Unidade Central de Processamento) e da memória principal.

# Identificação

* Página do repositório do trabalho (https://github.com/Viihtorugo/teaching/tree/master/2019.1-IAC/AB2.1-TP) 

- [x] Discente 1
	* Nome: Eduardo Brasil Araujo
	* Matrícula: 19110979
	* Distribuição da nota (40%): 
- [x] Discente 2
	* Nome: Luiz Antonio Marques Guimarães
	* Matrícula: 19111162
	* Distribuição da nota (20%): 
- [x] Discente 3
	* Nome: Victor Hugo Silva Ângelo
	* Matrícula: 19111334
	* Distribuição da nota (40%): 
	
# Resultados

Gráfico com os resultados das medições do comportamento do processo filho:

![Uso da UCP e consumo de UCP e Memória](https://github.com/Viihtorugo/teaching/blob/master/2019.1-IAC/AB2.1-TP/src/graphic/cpu(ucp-mem).png)
![Uso e consumo da UCP e Memória](https://github.com/Viihtorugo/teaching/blob/master/2019.1-IAC/AB2.1-TP/src/graphic/cpu-mem(ucp-mem).png)


	


# Discussão

## Introdução

Para monitorar um processo, foi reescrito o arquivo [main.c](https://github.com/Viihtorugo/teaching/blob/master/2019.1-IAC/AB2.1-TP/src/main.c).

Primeiramente, é criado um processo filho que herda as caracteísticas do processo pai e a variável *pid* demonstra qual processo está sendo executado:

```c
pid_t pid; //pid_t é um tipo de dado para processos

pid = fork();

if (pid < 0)
{
//excluir processo filho, pois deu erro
}

if (pid != 0)
{
//processo pai sendo executado
}
else
{
//processo filho sendo executado
}
```

Quando o processo pai é executado, toda vez ele espera um segundo (na função *sleep(1)*) dentro de um laço *for()* que tem restrição de 10 repetições, logo a execução será em 10 segundos e após esse tempo o processo filho é terminado.

Quando o processo pai é executado, primeiramente ele verifica se vai abrir ou não um arquivo para salvar o histórico dos testes (opção do usuário):

```c
	short to_file = 1;

	FILE* data_out;
	
	if (argc < 3)
	{
		data_out = fopen("./usage.out", "a");
	}
	else
	{
		if (strcmp(argv[2], "-no") != 0)
		{
			data_out = fopen(argv[2], "a");
		}
		else 
		{
			to_file = 0;
		}
	}
}
```

Além disso, durante os 10 segundos o processo pai é responsável por manipular o arquivo de histórico (caso tenha arquivo) e imprimir no terminal o uso da memória e da UCP:

```c
	if (to_file == 1) fprintf(data_out, "\nInicializando monitoramento... (1 seg por chamada)\n");
	
	// laço de repetição que executa 10 vezes que executa o monitoramento do processo filho
	for (int i = 0; i < 10; i++)
	{
		sleep(1); //espera 1 segundo e o processador 

		FILE* pipe;
			
		// cria variável com comando que será utilizado no pipe
		snprintf(buffer, sizeof(buffer), "ps -o rss,pcpu -p %i | tail -n1", pid);

		// criando uma file stream da saída do bash (pipe da saída do bash)
		pipe = popen((const char*)buffer, "r");
		
		if (pipe == NULL) // checagem de erro do popen()
		{
			perror("pipe");
			exit(1);
		}

		// pega os valores do uso de memória em KiB e uso da UCP em %
		float uso_mem = 0, uso_cpu = 0;
		fscanf(pipe, "%f %f", &uso_mem, &uso_cpu);

		uso_mem *= 1.024; // transforma de KiB para KB
		pclose(pipe); // como não será mais utilzado, o pipe é fechado

		char formato[64] = "Uso da UCP: %.2f%%\tUso da memória: %.2f KB\n";
		printf(formato, uso_cpu, uso_mem); // escreve no terminal a linha obtida no pipe do bash

		// escreve no arquivo de saída deste processo
		if (to_file == 1) fprintf(data_out, formato, uso_cpu, uso_mem);
	}

	if (to_file == 1) fclose(data_out); // fecha arquivo de saída deste processo

	// mata o processo filho após o for loop acima (demora 10 segundos)
	snprintf(buffer, sizeof(buffer), "kill %i", pid);
	system(buffer);
```

Durante esse 1 segundo que o processo pai parou de ser executado, o processador começa executar o processo filho. O processo filho contém duas verificação.

* Ao executar o programa com `./bin cpu`, o processo filho irá utilizar a UCP intensamente com um laço "infinito":

```c
for(int i = 0;;i++) // utilização da UCP intesamente
{
	i += rand();
	if (i > i % rand() + i / rand())
		i = 2 * rand() + 43;
	i *= rand();
	i /= rand();
	for (int j = 0; j < 100000; j++);
}
```

* Ao executar o programa com `./bin cpu-mem`, o processo filho irá utilizar a  memória e UCP intensamente:
```c
for(int i = 0;;i++) // utilização da UCP intesamente
{
	i += rand();
	if (i > i % rand() + i / rand())
		i = 2 * rand() + 43;
	i *= rand();
	i /= rand();
	for (int j = 0; j < 100000; j++);

	malloc(1); // utilização da memória intesamente
}
```

## Utilização intensa da UCP

Observou-se que nos testes e no [gráfico](https://github.com/Viihtorugo/teaching/blob/master/2019.1-IAC/AB2.1-TP/src/graphic/cpu(ucp-mem).png):
	
* Nesse caso **a curva no uso da UCP pelo tempo em segundos** é uma variante devido a concorrência do processador em relação aos processos que irá ser executado. Além disso, não é possível usar 100% da potência da UCP, pois (caso isso aconteça) se o computador não tiver um sistema de resfriamento (conhecido também como [Cooler](https://pt.wikipedia.org/wiki/Cooler)) que dissipe o calor produzido pela UCP o processador é danificado. Contudo, a metrica do uso da CPU - nesse caso - não totalmente precisa, logo os valores do uso da CPU atribuidas na variável ([uso_mem](https://github.com/Viihtorugo/teaching/blob/master/2019.1-IAC/AB2.1-TP/src/main.c)) ajuda na variação. 
	
* Já **a curva no uso de Memória em KB pelo tempo em segundos** vai ser sempre constante, pois não está sendo alocado nenhuma memória a mais no processo filho. Logo, toda vez que o processo filho é executado durante 1 segundo, a memória alocada será refênciada com o valor quando o processo filho sair do processador.


## Utilização intensa da UCP e memória

Observou-se que nos testes e no [gráfico](https://github.com/Viihtorugo/teaching/blob/master/2019.1-IAC/AB2.1-TP/src/graphic/cpu-mem(ucp-mem).png):
	
* Nesse caso **a curva no uso da UCP pelo tempo em segundos** também é uma variante, porém a CPU vai executar a função `malloc(1)`, aumentando o uso do UCP no processador.
	
* Já **a "curva" no uso de Memória em KB pelo tempo em segundos** se tornou uma função afim, ou seja, uma reta. Essa característica foi visível devido a memória alocada constante da chamada do laço "infinito" com a função `malloc(1)`. Devido a isso, ao ser executado o processo filho vai adicionando memória devido a função `malloc(1);`, ao voltar a ser executado ele continua alocando a memória, pois o filho está em loop "infinito". Assim, a equação da função pode ser descrita como a soma da memória inicial do loop com o produto do número de laços pelo retorno do valor da função `malloc(1)`.

* Outro ponto interessante que foi analisado foi o sobrecarregamento da memória fez alguns computadores começaram a travarem, mesmo em 10 segundos. Assim, quando a memória é utilizada intensamente acontece um estouro na memória principal, ou seja, não há mais recursos para armazenar na memória principal. Essa anomalia acontece porquê a linguagem de programação C não se preocupa com o transbordamento de dados, assim sobrescrevendo a memória principal que continha outros processos importantes do Sistema Operacional.

## Conclusão

No monitoramento do processo filho é visível que: o processo filho herda as características do processo pai, a concorrência de processos (quando o pai "dorme" 1 segundo, o filho que era executado), a importância de uma política de segurança de dados para memória principal e caso o processo pai fosse morto, o processo filho também será morto.

