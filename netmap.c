#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "netmap.h"
#include "roteador.h"
#include "terminal.h"

static void Leitura (FILE* entrada);
static CelTerm* ExecutaComando (char** item, CelTerm* listaTerm, LsRot* listaRot);

int CriaNetmap (int argv, char** argc){		// Recebe nome do arquivo como argumento.
	
	if(argv > 1){							// Verifica se houve argumento passado.

		FILE* entrada = fopen(argc[1], "r");	// Abre arquivo de entrada em modo leitura.

		if(entrada == 0)				// Verifica se houve falha ao abrir arquivo.
			return -2;
			
		else{
			
			Leitura(entrada);		// Chama funcao de leitura do arquivo .txt.
			
			printf("\n");
			fclose(entrada);		// Fecha o arquivo.
			return 0;
		}
	}
	else
		return -1;
}

static void Leitura (FILE* entrada){
	
	int 		i, aux;
	int 		k = 0;			// Debug.
	char		str[75];		// Declara string auxiliar de leitura do arquivo.
	char*		item[4];		// Declara mattriz de ponteiros que resgata itens lidos.
	
	CelTerm* listaTerm = InicializaListaTerm();		// Inicializa ambas as listas.
	LsRot* listaRot = InicializaListaRot();
	
	do{
		fgets(str, 75, entrada);		// str recebe linha atual do arquivo.
		item[0] = strtok(str, " ");		// item[0] recebe caracteres ate o primeiro espaco ou fim de linha.
		
		printf("Linha: %d", ++k);	    // Debug: numero da linha comecando em 1.

		if(item == NULL || memcmp(item[0], "FIM", 3) == 0)	    // Condicao de parada.
			break;                        						// Se FIM for lido, encerra.
		
		i = 0;
		do{
			i++;
			item[i] = strtok(NULL, " ");		// [1] item[i] recebe caracteres de str a partir do
		}while(item[i] != NULL);				// [2] ultimo strtok ate espaco ou fim de linha.
		
		aux = strlen(item[i-1]);				// aux recebe a posicao de '\0' no ultimo item lido.

		if(strchr(item[i-1], '\r') != NULL)	    // Verifica se tem '\r' nesse item.
			item[i-1][aux-2] = '\0';			// Se sim, remove '\r' e '\n'.
		else
			item[i-1][aux-1] = '\0';			// Se nao, remove somente '\n'.
		
		listaTerm = ExecutaComando(item, listaTerm, listaRot);		// Chama funcao de execucao dos comandos.
		printf("\n");
		
	}while(1);		// Condicao de parada ja presente no do-while.
	
	printf("\n");
	LiberaListaTerm(listaTerm);		// Libera lista de terminais.
	LiberaListaRot(listaRot);		// Libera lista de roteadores e suas relacoes.
}

static CelTerm* ExecutaComando (char** item, CelTerm* listaTerm, LsRot* listaRot){
	
	int j, i = 0;
	
	while(item[i] != NULL)
		i++;
	
	switch(i){			// O valor de i diz a quantidade de itens lidos.

		case 1:			// Comandos sem parametros passados do arquivo.
		{
			if( !strcmp(item[0], "IMPRIMENETMAP")){
				for(j=0; j<i; j++)						// Debug: para ver se entrou de acordo.
					printf(" %s", item[j]);
				ImprimeNetMap(listaTerm, listaRot);
			}
			break;
		}

		case 2:			// Comandos com 1 parametro passado do arquivo.
		{
			if( !strcmp(item[0], "REMOVEROTEADOR")){
				for(j=0; j<i; j++)
					printf(" %s", item[j]);
				RemoveRoteador(item[1], listaRot, listaTerm);
				break;
			}
			if( !strcmp(item[0], "REMOVETERMINAL")){
				for(j=0; j<i; j++)
					printf(" %s", item[j]);
				listaTerm = RemoveTerminal(item[1], listaTerm);
				break;
			}
			if( !strcmp(item[0], "DESCONECTATERMINAL")){
				for(j=0; j<i; j++)
					printf(" %s", item[j]);
				DesconectaTerminal(item[1], listaTerm);
				break;
			}
			if( !strcmp(item[0], "FREQUENCIAOPERADORA")){
				for(j=0; j<i; j++)
					printf(" %s", item[j]);
				FrequenciaOperadora(item[1], listaRot);
				break;
			}
			if( !strcmp(item[0], "FREQUENCIATERMINAL")){
				for(j=0; j<i; j++)
					printf(" %s", item[j]);
				FrequenciaTerminal(item[1], listaTerm);
				break;
			}
			break;
		}

		case 3:			// Comandos com 2 parametros passados do arquivo.
		{
			if( !strcmp(item[0], "CADASTRAROTEADOR")){
				for(j=0; j<i; j++)
					printf(" %s", item[j]);
				CadastraRoteador(item[1], item[2], listaRot);
				break;
			}
			if( !strcmp(item[0], "CADASTRATERMINAL")){
				for(j=0; j<i; j++)
					printf(" %s", item[j]);
				listaTerm = CadastraTerminal(item[1], item[2], listaTerm);
				break;
			}
			if( !strcmp(item[0], "CONECTAROTEADORES")){
				for(j=0; j<i; j++)
					printf(" %s", item[j]);
				ConectaRoteadores(item[1], item[2], listaRot);
				break;
			}
			if( !strcmp(item[0], "CONECTATERMINAL")){
				for(j=0; j<i; j++)
					printf(" %s", item[j]);
				ConectaTerminal(item[1], item[2], listaTerm, listaRot);
				break;
			}
			if( !strcmp(item[0], "ENVIARPACOTESDADOS")){
				for(j=0; j<i; j++)
					printf(" %s", item[j]);
				EnviarPacotesDados(item[1], item[2], listaTerm);
				break;
			}
			if( !strcmp(item[0], "DESCONECTAROTEADORES")){
				for(j=0; j<i; j++)
					printf(" %s", item[j]);
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
	grafo = fopen("saida.dot", "a+");
	
	if(grafo == NULL){
		perror("Erro ao abrir o arquivo saida.dot");
		exit(1);
		
	}else{
		
		fprintf(grafo, "strict graph {\n");
		
		ImprimeTerm(grafo, listaTerm);
		ImprimeRot(grafo, listaRot);
		
		fprintf(grafo, "}\n");
		
		fclose(grafo);
	}
}
