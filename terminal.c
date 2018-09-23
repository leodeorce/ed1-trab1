#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "terminal.h"

static Terminal* criaTerminal (char* nomeTerm, char* localizacao);
static void EscreveLOG ();
static void EscreveSAIDA ();

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
	
	printf ("Nome: %s\n", listaTerm->term->nome);
	printf ("Localizacao: %s", listaTerm->term->localizacao);
	
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
		// IMPRIME MENSAGEM DE ERRO
		printf("Erro: RemoveTerminal: Terminal nao encontrado");
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
	
	printf("Terminal %s removido", nomeTerm);
	
	return listaTerm;
}

void ConectaTerminal (char* nomeTerm, char* nomeRot, CelTerm* listaTerm, LsRot* listaRot){
	
	CelTerm* celT = BuscaTerminal(nomeTerm, listaTerm);
	
	if(celT == NULL){
		//Escreve no arquivo que o terminal nao existe.
		printf("Erro: ConectaTerminal: Terminal nao encontrado");
		return;
	}
	
	CelRot* celR = BuscaRoteador(nomeRot, listaRot);
	celT->term->rot = celR;
	
	printf("Terminal %s conectado ao roteador %s", nomeTerm, nomeRot);
}

void DesconectaTerminal (char* nomeTerm, CelTerm* listaTerm){
	
	CelTerm* celT = BuscaTerminal(nomeTerm, listaTerm);
	
	if(celT == NULL){
		// Escreve no arquivo que nomeTerm nao existe
		printf("Erro: DesconectaTerminal: Terminal nao encontrado");
		return;
		
	}else
		celT->term->rot = NULL;
	printf("Terminal %s desconectado", nomeTerm);
}

void EnviarPacotesDados (char* term1, char* term2, CelTerm* listaTerm){

	CelTerm* celT1 = BuscaTerminal(term1, listaTerm);
	if (celT1 == NULL){
		//ERRO, TERMINAL NAO CADASTRADO
		printf("Erro: EnviarPacotesDados: Terminal %s nao existe", term1);
		return;
	}
	
	CelTerm* celT2 = BuscaTerminal(term2, listaTerm);
	if (celT2 == NULL){
		//ERRO, TERMINAL NAO CADASTRADO
		printf("Erro: EnviarPacotesDados: Terminal %s nao existe", term2);
		return;
	}
	
	if (celT1->term->rot != NULL && celT2->term->rot != NULL){
		
		char* rotT1 = retornaNomeRot(celT1->term->rot);
		char* rotT2 = retornaNomeRot(celT2->term->rot);
		
		if (!strcmp(rotT1, rotT2)){
			printf("SIM");
			return;
		}
		
	}else{
		printf("NAO");
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
		puts("SIM");
		return;
	}
	
	if (c == NULL)
		printf("NAO");
	else
		printf("SIM");
}

void FrequenciaTerminal (char* localizacao, CelTerm* listaTerm){
	
	int qtdTerm = 0;
	CelTerm* celT = listaTerm;
	
	while(celT != NULL){
		
		if(!strcmp(celT->term->localizacao, localizacao))
			qtdTerm++;
		
		celT = celT->prox;
	}
	printf("Frequencia terminal %s: %d", localizacao, qtdTerm);
}

/* Auxiliares Compartilhados */

CelTerm* InicializaListaTerm (){
	
	CelTerm* listaTerm = NULL;
	puts("Lista de terminais inicializada");
	return listaTerm;
}

CelTerm* BuscaTerminal (char* nomeTerm, CelTerm* listaTerm){
	
	CelTerm* celT = listaTerm;
	
	while(celT != NULL){
		
		if(!strcmp(celT->term->nome, nomeTerm))
			return celT;
		
		celT = celT->prox;
	}
	puts("Erro: BuscaTerminal: Terminal nao encontrado");
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
		
		puts("Terminal destruido");
		listaTerm = aux;
	}
	free(listaTerm);
	puts("Lista de terminais destruida");
}

/* Auxiliares Exclusivos */

static Terminal* criaTerminal (char* nomeTerm, char* localizacao){
	
	Terminal* t = (Terminal*) malloc(sizeof(Terminal));
	t->nome = (char*) malloc(((strlen(nomeTerm))+1)*sizeof(char));
	t->localizacao = (char*) malloc(((strlen(localizacao))+1)*sizeof(char));
	
	strcpy(t->nome, nomeTerm);
	strcpy(t->localizacao, localizacao);
	t->rot = NULL;
	
	puts("Terminal criado");
	
	return t;
}

static void EscreveLOG (){
	// Chamado em qualquer comando que nao possa ser executado.
}

static void EscreveSAIDA (){
	// Chamado em EnviarPacotesDados e FrequenciaTerminal.
}
