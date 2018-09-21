#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "terminal.h"

struct terminal{
	char* nome;
	char* localizacao;
	CelRot* rot;		// BuscaRoteador retorna a celula
};

struct celTerm{
	Terminal* term;
	struct celTerm* prox;
};

CelTerm* CadastraTerminal (char* nomeTerm, char* localizacao, CelTerm* listaTerm){
	
	CelTerm* celT = (CelTerm*) malloc(sizeof(CelTerm));
	Terminal* t = criaTerminal(nomeTerm, localizacao);
	
	celT->term = t;
	celT->prox = listaTerm;
	listaTerm = celT;
	
	//printf ("Nome:\t%s\n", listaTerm->term->nome);
	//printf ("Localizacao:\t%s\n", listaTerm->term->localizacao);
	
	return listaTerm;
}

void ConectaTerminal (char* nomeTerm, char* nomeRot, CelTerm* listaTerm, LsRot* listaRot){
	
	Terminal* t = BuscaTerminal(nomeTerm, listaTerm);
	
	if(t == NULL){
		//Escreve no arquivo que o terminal nao existe.
		//puts("Erro: CT 1");
		return;
	}
	if(t->rot != NULL){
		//Escreve no arquivo que o terminal ja esta conectado.
		//puts("Erro: CT 2");
		
	}else{
		CelRot* r = BuscaRoteador(nomeRot, listaRot);
		t->rot = r;
		//puts("Conectado ao roteador");
	}
}

void DesconectaTerminal (char* nomeTerm, CelTerm* listaTerm){
	
	Terminal* t = BuscaTerminal(nomeTerm, listaTerm);
	
	if(t == NULL){
		// Escreve no arquivo que nomeTerm nao existe
		return;
	}else
		t->rot = NULL;
}

CelTerm* RemoveTerminal (char* nomeTerm, CelTerm* listaTerm){
	
}

void EnviarPacoteDados (char* nomeTerm1, char* nomeTerm2, CelTerm* listaTerm){
	
}

void FrequenciaTerminal (char* localizacao, CelTerm* listaTerm){
	
}

//Auxiliares

CelTerm* InicializaListaTerm (){
	
	CelTerm* listaTerm = NULL;
	//puts("Lista inicializada");
	return listaTerm;
}

void LiberaListaTerm (CelTerm* listaTerm){
	
	CelTerm* aux;
	while (listaTerm != NULL){
		
		aux = listaTerm->prox;
		
		free(listaTerm->term->nome);
		free(listaTerm->term->localizacao);
		free(listaTerm->term);
		free(listaTerm);
		
		//puts("Tirei um terminal");
		listaTerm = aux;
	}
	//puts("Tirei tudo");
}

void DesconectaRoteador (char* nomeRot, CelTerm* listaTerm, LsRot* listaRot){
	
	CelRot* r = BuscaRoteador(nomeRot, listaRot);
	CelTerm* aux = listaTerm;
	
	while(aux != NULL){
		
		if(aux->term->rot == r){
			aux->term->rot = NULL;
			//puts("Desconectei roteador de um terminal");
		}
		aux = aux->prox;
	}
}

Terminal* BuscaTerminal (char* nomeTerm, CelTerm* listaTerm){
	
	CelTerm* aux = listaTerm;
	
	while(aux != NULL){
		
		if(!strcmp(aux->term->nome, nomeTerm))
			return aux->term;
		aux = aux->prox;
	}
	//puts("Erro: BT");
	return NULL;
}

static Terminal* criaTerminal (char* nomeTerm, char* localizacao){
	
	Terminal* t = (Terminal*) malloc(sizeof(Terminal));
	t->nome = (char*) malloc(((strlen(nomeTerm))+1)*sizeof(char));
	t->localizacao = (char*) malloc(((strlen(localizacao))+1)*sizeof(char));
	
	strcpy(t->nome, nomeTerm);
	strcpy(t->localizacao, localizacao);
	t->rot = NULL;
	
	//puts("Terminal criado");
	
	return t;
}