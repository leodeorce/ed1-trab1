#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "terminal.h"
#include "roteador.h"

static Terminal* criaTerminal (char* nomeTerm, char* localizacao);

struct terminal{
	char* nome;
	char* localizacao;
	CelRot* rot;
};

struct celTerm{
	Terminal* term;
	CelTerm* prox;
};

/* Principais */

CelTerm* CadastraTerminal (char* nomeTerm, char* localizacao, CelTerm* listaTerm){
	
	CelTerm* aux = listaTerm;
	CelTerm* celT = (CelTerm*) malloc(sizeof(CelTerm));
	Terminal* t = criaTerminal(nomeTerm, localizacao);		// Cria tipo item de terminal.
	
	celT->term = t;
	celT->prox = NULL;					// Celula nova preenchida.
	
	if(aux != NULL){					// Verifica se lista vazia.
		
		while(aux->prox != NULL)		// Se nao, anda ateh ultima posicao.
			aux = aux->prox;
			
		aux->prox = celT;				// Insere nova celula na ultima posicao.
		
	}else
		listaTerm = celT;				// Se vazia, nova celula eh primeiro elemento.
	
	return listaTerm;
}

CelTerm* RemoveTerminal (char* nomeTerm, CelTerm* listaTerm){
	
	CelTerm* celT = BuscaTerminal(nomeTerm, listaTerm);
	CelTerm* auxProx = listaTerm;
	CelTerm* auxAnt;
	
	while(auxProx != NULL && auxProx != celT){		// Anda pela lista ate encontrar celula desejada ou fim.
		auxAnt = auxProx;
		auxProx = auxProx->prox;
	}
	
	if(auxProx == NULL){		// Caso nao ache celula desejada imprime mensagem de erro em log.txt.
		char msg[50];
		sprintf(msg,"Erro: Terminal %s inexistente no NetMap", nomeTerm);
		EscreveLOG(msg);
		return NULL;
	}
	
	if(auxProx == listaTerm)			// Procedimento caso a celula desejada for a primeira da lista.
		listaTerm = listaTerm->prox;
	else								// Procedimento caso nao seja.
		auxAnt->prox = auxProx->prox;
	
	free (auxProx->term->nome);
	free (auxProx->term->localizacao);
	free (auxProx->term);
	free (auxProx);
	
	return listaTerm;
}

void ConectaTerminal (char* nomeTerm, char* nomeRot, CelTerm* listaTerm, void* listaRot){
	
	CelTerm* celT = BuscaTerminal(nomeTerm, listaTerm);
	
	if(celT == NULL){		// Caso nao ache terminal desejado imprime mensagem de erro em log.txt.
		char msg[50];
		sprintf(msg,"Erro: Terminal %s inexistente no NetMap", nomeTerm);
		EscreveLOG(msg);
		return;
	}
	
	CelRot* celR = BuscaRoteador(nomeRot, listaRot);
	
	if(celR == NULL){		// Caso nao ache roteador desejado imprime mensagem de erro em log.txt.
		char msg[50];
		sprintf(msg,"Erro: Roteador %s inexistente no NetMap", nomeRot);
		EscreveLOG(msg);
		return;
	}
	
	celT->term->rot = celR;
}

void DesconectaTerminal (char* nomeTerm, CelTerm* listaTerm){
	
	CelTerm* celT = BuscaTerminal(nomeTerm, listaTerm);
	
	if(celT == NULL){		// Caso nao ache terminal desejado imprime mensagem de erro em log.txt.
		char msg[50];
		sprintf(msg,"Erro: Terminal %s inexistente no NetMap", nomeTerm);
		EscreveLOG(msg);
		return;
		
	}else
		celT->term->rot = NULL;		// Desconecta terminal de seu roteador.
}

void EnviarPacotesDados (char* nometerm1, char* nometerm2, CelTerm* listaTerm){

	CelTerm* celT1 = BuscaTerminal(nometerm1, listaTerm); //Busca terminal1 na lista de terminais do netmap
	if (celT1 == NULL){
		char msg[50];
		sprintf(msg,"Erro: Terminal %s inexistente no NetMap", nometerm1);
		EscreveLOG(msg);
		return;
	}
	
	CelTerm* celT2 = BuscaTerminal(nometerm2, listaTerm); //Busca terminal2 na lista de terminais do netmap
	if (celT2 == NULL){
		char msg[50];
		sprintf(msg,"Erro: Terminal %s inexistente no NetMap", nometerm2);
		EscreveLOG(msg);
		return;
	}
	
	if (celT1->term->rot != NULL && celT2->term->rot != NULL){ //Verifica se os terminais estao conectados a algum roteador
		
		char* rotT1 = retornaNomeRot(celT1->term->rot);
		char* rotT2 = retornaNomeRot(celT2->term->rot);
		
		if (!strcmp(rotT1, rotT2)){                     //Se sim, verifica se eh no mesmo roteador
			char msg[50];
			sprintf(msg,"ENVIARPACOTESDADOS %s %s: SIM", nometerm1, nometerm2);
			EscreveSAIDA(msg);
			return;
		}
												//Se nao for,
		char vet[100][25];                     //Inicializa vetor de strings para auxiliar na funcao de busca
		int i = 0;                            //Funcionara como um indicador da primeira posicao vazia do vet
		
		int k = funcaoBusca(celT1->term->rot, rotT2, vet, &i);  //Funcao de busca
		
		if(k == 1){
			char msg[50];
			sprintf(msg,"ENVIARPACOTESDADOS %s %s: SIM", nometerm1, nometerm2);
			EscreveSAIDA(msg);
		}else{
			char msg[50];
			sprintf(msg,"ENVIARPACOTESDADOS %s %s: NAO", nometerm1, nometerm2);
			EscreveSAIDA(msg);
		}
		
	}else{                                     //O terminal 1 ou o terminal 2 nao esta(o) conectado(s) a nenhum roteador
		char msg[50];
		sprintf(msg,"ENVIARPACOTESDADOS %s %s: NAO", nometerm1, nometerm2);
		EscreveSAIDA(msg);
		return;
	}
	
}

void FrequenciaTerminal (char* localizacao, CelTerm* listaTerm){
	
	int qtdTerm = 0;
	CelTerm* celT = listaTerm;
	
	while(celT != NULL){		// Anda pela lista com variavel auxiliar.
		
		if(!strcmp(celT->term->localizacao, localizacao))		// Conta +1 para cada terminal correspondente.
			qtdTerm++;
		
		celT = celT->prox;
	}
	
	char msg[50];
	sprintf(msg,"FREQUENCIATERMINAL %s: %d", localizacao, qtdTerm);
	EscreveSAIDA(msg);													// Imprime em saida.txt o resultado.
}

/* Auxiliares Compartilhados */

CelTerm* InicializaListaTerm (){
	
	CelTerm* listaTerm = NULL;
	return listaTerm;
}

CelTerm* BuscaTerminal (char* nomeTerm, CelTerm* listaTerm){
	
	CelTerm* celT = listaTerm;
	
	while(celT != NULL){		// Anda pela lista com variavel auxiliar.
		
		if(!strcmp(celT->term->nome, nomeTerm))		// Retorna a celula cujo terminal bater com o nome especificado.
			return celT;
		
		celT = celT->prox;
	}
	
	return NULL;		// Retorna NULL caso nao encontre.
}

void DesconectaRoteador (void* celR, CelTerm* listaTerm, void* listaRot){
	
	CelTerm* celT = listaTerm;
	
	while(celT != NULL){		// Anda pela lista de terminais com variavel auxiliar.
		
		if(celT->term->rot == celR){		// Caso o terminal esteja conectado ao roteador especificado,
											// desconecta o terminal.
			celT->term->rot = NULL;
		}
		celT = celT->prox;
	}
}

void EscreveLOG (char* mensagem){
	FILE *log;
	log = fopen("log.txt", "a+");			// Abre ou cria log.txt apontando para o final do arquivo.
	
	if(log == NULL){						// Exibe mensagem de erro caso falha.
		perror("Erro ao abrir o arquivo log.txt");
		exit(1);
	}else{
		fprintf(log, "%s\n", mensagem);		// Escreve mensagem em log.txt.
		fclose(log);
	}
}

void EscreveSAIDA (char* mensagem){
	FILE *saida;
	saida = fopen("saida.txt", "a+");		// Abre ou cria saida.txt apontando para o final do arquivo.
		
	if(saida == NULL){						// Exibe mensagem de erro caso falha.
		perror("Erro ao abrir o arquivo saida.txt");
		exit(1);
	}else{
		fprintf(saida, "%s\n", mensagem);	// Escreve mensagem em saida.txt.
		fclose(saida);
	}
}

void ImprimeTerm (FILE* grafo, CelTerm* listaTerm){
	
	CelTerm* aux = listaTerm;
	char* nomeRot;
	
	while(aux != NULL){
	
		fprintf(grafo, "\t%s", aux->term->nome);			// Escreve nome do terminal.
	
		if(aux->term->rot != NULL){							// Verifica se terminal estah conectado a um roteador.
	
			nomeRot = retornaNomeRot(aux->term->rot);
			fprintf(grafo, " -- %s;\n", nomeRot);			// Se sim, escreve roteador.
	
		}else
			fprintf(grafo, ";\n");							// Se nao, termina linha.
	
		aux = aux->prox;
	}
}

void LiberaListaTerm (CelTerm* listaTerm){
	
	CelTerm* aux;
	
	while (listaTerm != NULL){		// Anda pela lista com seu proprio ponteiro.
		
		aux = listaTerm->prox;		// Guarda a proxima celula ou NULL.
		
		free(listaTerm->term->nome);
		free(listaTerm->term->localizacao);
		free(listaTerm->term);
		free(listaTerm);
		
		listaTerm = aux;
	}
	
	free(listaTerm);
}

/* Auxiliares Exclusivos */

static Terminal* criaTerminal (char* nomeTerm, char* localizacao){
	
	Terminal* itemT = (Terminal*) malloc(sizeof(Terminal));
	itemT->nome = (char*) malloc(((strlen(nomeTerm)) + 1) * sizeof(char));
	itemT->localizacao = (char*) malloc(((strlen(localizacao)) + 1) * sizeof(char));
	
	strcpy(itemT->nome, nomeTerm);
	strcpy(itemT->localizacao, localizacao);
	itemT->rot = NULL;						// Terminal inicialmente desconectado.
	
	return itemT;
}
