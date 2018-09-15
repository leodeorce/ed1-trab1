#include <stdio.h>
#include <string.h>
// #include "roteador.h"
// #include "terminal.h"

int main (int argv, char* argc[]){				// Recebe nome do arquivo como argumento.

	if(argv > 1){								// Verifica se houve argumento passado.

		FILE* entrada = fopen(argc[1], "r");	// Abre arquivo de entrada em modo leitura.

		if(entrada == 0)						// Verifica se houve falha ao abrir arquivo.
			puts("Erro: Falha ao abrir arquivo de entrada");
		else{

			int 	i, j;
			int		tamanho, k = 0;		// Debug
			char	str[50];		// Declara string auxiliar de leitura do arquivo.
			char*	item[3];		// Declara mattriz de ponteiros que resgata itens lidos.

			do{
				fgets(str, 50, entrada);		// str recebe linha atual do arquivo.
				item[0] = strtok(str, " ");		// item[0] recebe caracteres ate o primeiro espaco ou fim de linha.

				tamanho = strlen(item[0]);
				printf("Iteracao: %d", ++k);	    // Debug: numero da linha comecando em 1.
				printf("\tTamanho: %d", tamanho);	// Debug: tamanho do primeiro item.

				if(memcmp(item[0], "FIM", 3) == 0)	    // Compara o item com FIM.
					break;                              // Se FIM for lido, encerra o do-while.

				i = 0;
				while(item[i] != NULL){
					i++;
					item[i] = strtok(NULL, " ");		// [1] item[i] recebe caracteres de str a partir do
				}										// [2] ultimo strtok ate espaco ou fim de linha.

				int aux;
				aux = strlen(item[i-1]);				// aux recebe a posicao de '\0' no ultimo item lido.

				if(strchr(item[i-1], '\r') != NULL)	    // Verifica se tem '\r' nesse item.
					item[i-1][aux-2] = '\0';			// Se sim, remove '\r' e '\n'.
				else
					item[i-1][aux-1] = '\0';			// Se nao, remove somente '\n'.

				printf("\ti: %d", i);		// Debug: para ver quantidade de itens lidos, e se FIM parou antes disso.

				switch(i){			// O valor de i diz a quantidade de itens lidos.

					case 1:			// Comandos sem parametros passados do arquivo.
					{
						if( !strcmp(item[0], "IMPRIMENETMAP")){
							for(j=0; j<i; j++)						// Debug: para ver se entrou de acordo.
								printf("\t%s", item[j]);
							//ImprimeNetmap();
						}
						break;
					}

					case 2:			// Comandos com 1 parametro passado do arquivo.
					{
						if( !strcmp(item[0], "REMOVEROTEADOR")){
							for(j=0; j<i; j++){
								if(j==1) printf("\t");
								printf("\t%s", item[j]);
							}
							//RemoveRoteador();
							break;
						}
						if( !strcmp(item[0], "REMOVETERMINAL")){
							for(j=0; j<i; j++){
								if(j==1) printf("\t");
								printf("\t%s", item[j]);
							}
							//RemoveTerminal();
							break;
						}
						if( !strcmp(item[0], "DESCONECTATERMINAL")){
							for(j=0; j<i; j++)
								printf("\t%s", item[j]);
							//DesconectaTerminal();
							break;
						}
						if( !strcmp(item[0], "FREQUENCIAOPERADORA")){
							for(j=0; j<i; j++)
								printf("\t%s", item[j]);
							//FrequenciaOperadora();
							break;
						}
						if( !strcmp(item[0], "FREQUENCIATERMINAL")){
							for(j=0; j<i; j++)
								printf("\t%s", item[j]);
							//FrequenciaTerminal();
							break;
						}
						break;
					}

					case 3:			// Comandos com 2 parametros passados do arquivo.
					{
						if( !strcmp(item[0], "CADASTRAROTEADOR")){
							for(j=0; j<i; j++)
								printf("\t%s", item[j]);
							//CadastraRoteador();
							break;
						}
						if( !strcmp(item[0], "CADASTRATERMINAL")){
							for(j=0; j<i; j++)
								printf("\t%s", item[j]);
							//CadastraTerminal();
							break;
						}
						if( !strcmp(item[0], "CONECTAROTEADORES")){
							for(j=0; j<i; j++)
								printf("\t%s", item[j]);
							//ConectaRoteadores();
							break;
						}
						if( !strcmp(item[0], "CONECTATERMINAL")){
							for(j=0; j<i; j++){
								if(j==1) printf("\t");
								printf("\t%s", item[j]);
							}
							//ConectaTerminal();
							break;
						}
						if( !strcmp(item[0], "ENVIARPACOTESDADOS")){
							for(j=0; j<i; j++)
								printf("\t%s", item[j]);
							//EnviarPacoteDados();
							break;
						}
						if( !strcmp(item[0], "DESCONECTAROTEADORES")){
							for(j=0; j<i; j++)
								printf("\t%s", item[j]);
							//DesconectaRoteadores();
							break;
						}
						break;
					}

					default:
						break;
				}
				printf("\n");		// Debug: ajeitando saida.

			}while(1);		// Condicao de parada ja presente no do-while.

			fclose(entrada);		// Fecha o arquivo.
		}
		printf("\n");		// Debug: ajeitando saida.
	}
	else
		puts ("Erro: Arquivo de entrada nao detectado");
	return 0;
}
