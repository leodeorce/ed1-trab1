#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "netmap.h"
#include "roteador.h"
#include "terminal.h"

static void Leitura (FILE* entrada);
static CelTerm* ExecutaComando (char** item, int i, CelTerm* listaTerm, LsRot* listaRot);

int CriaNetmap (int argv, char** argc){		// Recebe nome do arquivo como argumento.
	
	if(argv > 1){							// Verifica se houve argumento passado.

		FILE* entrada = fopen(argc[1], "r");	// Abre arquivo de entrada em modo leitura.

		if(entrada == 0)				// Verifica se houve falha ao abrir arquivo.
			return -2;
			
		else{
			
			Leitura(entrada);		// Chama funcao de leitura do arquivo .txt.
			fclose(entrada);		// Fecha o arquivo.
			return 0;
		}
	}
	else
		return -1;
}

static void Leitura (FILE* entrada){
	
	int 	i, aux;
	char	str[75];		// Declara string auxiliar de leitura do arquivo.
	char	*item[4];		// Declara mattriz de ponteiros que resgata itens lidos.
	char	*auxR, *auxN;
	
	CelTerm* listaTerm = InicializaListaTerm();		// Inicializa ambas as listas.
	LsRot* listaRot = InicializaListaRot();
	
	do{
		fgets(str, 75, entrada);			// str recebe linha atual do arquivo.
		
		item[0] = strtok(str, " ");			// item[0] recebe caracteres ate o primeiro espaco ou fim de linha.
		
		if(item[0] == NULL || memcmp(item[0], "FIM", 3) == 0)	    // Condicao de parada.
			break;                        							// Se FIM for lido, encerra.
		
		i = 0;
		
		while( strchr(item[i], '\n') == NULL ){
			
			i++;
			
			item[i] = strtok(NULL, " ");		// item[i] recebe caracteres de str a partir do ultimo strtok
		
			if( i == 2)
				break;
		}
		
		auxR = strchr(item[i], '\r');		// Verifica se ultimo item lido tem '\r'.
		auxN = strchr(item[i], '\n');		// Verifica se ultimo item lido tem '\n'.
		
		if( auxR != NULL ){					// Assumindo que, se '\r' existir, '\n' tambem existe.
			
			aux = strcspn( item[i], "\r" );		// Se tem '\r', finaliza item[i] na posicao anterior.
			item[i][aux] = '\0';
			
		}else if( auxN != NULL ){
			
			aux = strcspn( item[i], "\n" );		// Se tem '\n', finaliza item[i] na posicao anterior.
			item[i][aux] = '\0';
			
		}
		
		aux = strlen( item[i] );		// Caso existam espaco(s) apos o ultimo argumento em uma linha qualquer
		if( aux == 0 )					// no arquivo de entrada, este(s) ocupa(m) item[i] e sao retirados aqui.
			i--;
		
		i++;		// i agora guarda a quantidade de argumentos na matriz item.
		
		listaTerm = ExecutaComando(item, i, listaTerm, listaRot);		// Chama funcao de execucao dos comandos.
		
	}while(1);		// Condicao de parada ja presente no do-while.

	LiberaListaTerm(listaTerm);		// Libera lista de terminais.
	LiberaListaRot(listaRot);		// Libera lista de roteadores e suas relacoes.
}

static CelTerm* ExecutaComando (char** item, int i, CelTerm* listaTerm, LsRot* listaRot){
	
	switch(i){		// Aproveita a informacao de quantos argumentos foram lidos.

		case 1:			// Comandos sem parametros passados do arquivo.
		{
			if( !strcmp(item[0], "IMPRIMENETMAP"))
				ImprimeNetMap(listaTerm, listaRot);
			break;
		}

		case 2:			// Comandos com 1 parametro passado do arquivo.
		{
			if( !strcmp(item[0], "REMOVEROTEADOR")){
				RemoveRoteador(item[1], listaRot, listaTerm);
				break;
			}
			if( !strcmp(item[0], "REMOVETERMINAL")){
				listaTerm = RemoveTerminal(item[1], listaTerm);
				break;
			}
			if( !strcmp(item[0], "DESCONECTATERMINAL")){
				DesconectaTerminal(item[1], listaTerm);
				break;
			}
			if( !strcmp(item[0], "FREQUENCIAOPERADORA")){
				FrequenciaOperadora(item[1], listaRot);
				break;
			}
			if( !strcmp(item[0], "FREQUENCIATERMINAL")){
				FrequenciaTerminal(item[1], listaTerm);
				break;
			}
			break;
		}

		case 3:			// Comandos com 2 parametros passados do arquivo.
		{
			if( !strcmp(item[0], "CADASTRAROTEADOR")){
				CadastraRoteador(item[1], item[2], listaRot);
				break;
			}
			if( !strcmp(item[0], "CADASTRATERMINAL")){
				listaTerm = CadastraTerminal(item[1], item[2], listaTerm);
				break;
			}
			if( !strcmp(item[0], "CONECTAROTEADORES")){
				ConectaRoteadores(item[1], item[2], listaRot);
				break;
			}
			if( !strcmp(item[0], "CONECTATERMINAL")){
				ConectaTerminal(item[1], item[2], listaTerm, listaRot);
				break;
			}
			if( !strcmp(item[0], "ENVIARPACOTESDADOS")){
				EnviarPacotesDados(item[1], item[2], listaTerm);
				break;
			}
			if( !strcmp(item[0], "DESCONECTAROTEADORES")){
				DesconectaRoteadores(item[1], item[2], listaRot);
				break;
			}
			break;
		}
		
		default:
			break;
	}
	
	return listaTerm;
}

void ImprimeNetMap (void* listaTerm, void* listaRot){
	
	FILE *grafo;
	grafo = fopen("saida.dot", "a+");		// Abre ou cria saida.dot apontando para o final do arquivo.
	
	if(grafo == NULL){			// Exibe mensagem de erro caso falha.
		perror("Erro ao abrir o arquivo saida.dot");
		exit(1);
		
	}else{
		
		fprintf(grafo, "strict graph {\n");		// Inicia linguagem dot.
		
		ImprimeTerm(grafo, listaTerm);			// Funcoes que facilitam a escrita dos elementos do netmap.
		ImprimeRot(grafo, listaRot);
		
		fprintf(grafo, "}\n");					// Finaliza linguagem dot.
		
		fclose(grafo);		// Fecha saida.dot.
	}
}
