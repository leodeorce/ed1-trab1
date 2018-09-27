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
	
	CelTerm* celT = (CelTerm*) malloc(sizeof(CelTerm));
	Terminal* t = criaTerminal(nomeTerm, localizacao);
	
	celT->term = t;
	celT->prox = listaTerm;
	listaTerm = celT;
	
	return listaTerm;
}

CelTerm* RemoveTerminal (char* nomeTerm, CelTerm* listaTerm){
	
	CelTerm* celT = BuscaTerminal(nomeTerm, listaTerm);
	CelTerm* auxProx = listaTerm;
	CelTerm* auxAnt;
	
	while(auxProx != NULL && auxProx != celT){
		auxAnt = auxProx;
		auxProx = auxProx->prox;
	}
	
	if(auxProx == NULL){
		char msg[50];
		sprintf(msg,"Erro: Terminal %s nao existe no NetMap", nomeTerm);
		EscreveLOG(msg);
		return NULL;
	}
	
	if(auxProx == listaTerm)
		listaTerm = listaTerm->prox;
	else
		auxAnt->prox = auxProx->prox;
	
	free (auxProx->term->nome);
	free (auxProx->term->localizacao);
	free (auxProx->term);
	free (auxProx);
	
	return listaTerm;
}

void ConectaTerminal (char* nomeTerm, char* nomeRot, CelTerm* listaTerm, void* listaRot){
	
	CelTerm* celT = BuscaTerminal(nomeTerm, listaTerm);
	
	if(celT == NULL){
		char msg[50];
		sprintf(msg,"Erro: Terminal %s nao existe no NetMap", nomeTerm);
		EscreveLOG(msg);
		return;
	}
	
	CelRot* celR = BuscaRoteador(nomeRot, listaRot);
	celT->term->rot = celR;
}

void DesconectaTerminal (char* nomeTerm, CelTerm* listaTerm){
	
	CelTerm* celT = BuscaTerminal(nomeTerm, listaTerm);
	
	if(celT == NULL){
		char msg[50];
		sprintf(msg,"Erro: Terminal %s nao existe no NetMap", nomeTerm);
		EscreveLOG(msg);
		return;
		
	}else
		celT->term->rot = NULL;
}

void EnviarPacotesDados (char* nometerm1, char* nometerm2, CelTerm* listaTerm){

	CelTerm* celT1 = BuscaTerminal(nometerm1, listaTerm);
	if (celT1 == NULL){
		char msg[50];
		sprintf(msg,"Erro: Terminal %s nao existe no NetMap", nometerm1);
		EscreveLOG(msg);
		return;
	}
	
	CelTerm* celT2 = BuscaTerminal(nometerm2, listaTerm);
	if (celT2 == NULL){
		char msg[50];
		sprintf(msg,"Erro: Terminal %s nao existe no NetMap", nometerm2);
		EscreveLOG(msg);
		return;
	}
	
	if (celT1->term->rot != NULL && celT2->term->rot != NULL){
		
		char* rotT1 = retornaNomeRot(celT1->term->rot);
		char* rotT2 = retornaNomeRot(celT2->term->rot);
		
		if (!strcmp(rotT1, rotT2)){
			char msg[50];
			sprintf(msg,"ENVIARPACOTESDADOS %s %s: SIM", nometerm1, nometerm2);
			EscreveSAIDA(msg);
			return;
		}
		
	}else{
		char msg[50];
		sprintf(msg,"ENVIARPACOTESDADOS %s %s: NAO", nometerm1, nometerm2);
		EscreveSAIDA(msg);
		return;
	}
	
	char* rotT2 = retornaNomeRot(celT2->term->rot);
	LsRot* ls = retornaRotConectados(celT1->term->rot);
	CelRot* c = BuscaRoteador(rotT2, ls);

	if (c == NULL){
		CelRot* p = retornaPrim(ls);
		LsRot* aux;
		int i =0;
	
		while (p != NULL && c== NULL){
			aux = retornaRotConectados(p);
			c = BuscaRoteador (rotT2, aux);
			p = retornaProxCel(p);
		}	
	}else{
		char msg[50];
		sprintf(msg,"ENVIARPACOTESDADOS %s %s: SIM", nometerm1, nometerm2);
		EscreveSAIDA(msg);
		return;
	}
	
	if (c == NULL){
		char msg[50];
		sprintf(msg,"ENVIARPACOTESDADOS %s %s: NAO", nometerm1, nometerm2);
		EscreveSAIDA(msg);
	}else{
		char msg[50];
		sprintf(msg,"ENVIARPACOTESDADOS %s %s: SIM", nometerm1, nometerm2);
		EscreveSAIDA(msg);
	}
}

void FrequenciaTerminal (char* localizacao, CelTerm* listaTerm){
	
	int qtdTerm = 0;
	CelTerm* celT = listaTerm;
	
	while(celT != NULL){
		
		if(!strcmp(celT->term->localizacao, localizacao))
			qtdTerm++;
		
		celT = celT->prox;
	}
	char msg[50];
	sprintf(msg,"FREQUENCIATERMINAL %s: %d", localizacao, qtdTerm);
	EscreveSAIDA(msg);
}

/* Auxiliares Compartilhados */

CelTerm* InicializaListaTerm (){
	
	CelTerm* listaTerm = NULL;
	return listaTerm;
}

CelTerm* BuscaTerminal (char* nomeTerm, CelTerm* listaTerm){
	
	CelTerm* celT = listaTerm;
	
	while(celT != NULL){
		
		if(!strcmp(celT->term->nome, nomeTerm))
			return celT;
		
		celT = celT->prox;
	}
	
	return NULL;
}

void LiberaListaTerm (CelTerm* listaTerm){
	
	CelTerm* aux;
	
	while (listaTerm != NULL){
		
		aux = listaTerm->prox;
		
		free(listaTerm->term->nome);
		free(listaTerm->term->localizacao);
		free(listaTerm->term);
		free(listaTerm);
		
		listaTerm = aux;
	}
	free(listaTerm);
}

void DesconectaRoteador (void* celR, CelTerm* listaTerm, void* listaRot){
	
	CelTerm* celT = listaTerm;
	
	while(celT != NULL){
		
		if(celT->term->rot == celR){
			
			celT->term->rot = NULL;
		}
		celT = celT->prox;
	}
}

void EscreveLOG (char* mensagem){
	FILE *log;
	log = fopen("log.txt", "a+");
	
	if(log == NULL){
		perror("Erro ao abrir o arquivo log.txt");
		exit(1);
	}else{
		fprintf(log, "%s\n", mensagem);
		fclose(log);
	}
}

void EscreveSAIDA (char* mensagem){
	FILE *saida;
	saida = fopen("saida.txt", "a+");
		
	if(saida == NULL){
		perror("Erro ao abrir o arquivo saida.txt");
		exit(1);
	}else{
		fprintf(saida, "%s\n", mensagem);
		fclose(saida);
	}
}

/* Auxiliares Exclusivos */

static Terminal* criaTerminal (char* nomeTerm, char* localizacao){
	
	Terminal* t = (Terminal*) malloc(sizeof(Terminal));
	t->nome = (char*) malloc(((strlen(nomeTerm))+1)*sizeof(char));
	t->localizacao = (char*) malloc(((strlen(localizacao))+1)*sizeof(char));
	
	strcpy(t->nome, nomeTerm);
	strcpy(t->localizacao, localizacao);
	t->rot = NULL;
	
	return t;
}
