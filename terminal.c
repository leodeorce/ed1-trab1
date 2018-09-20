#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "arquivo.h"
#include "roteador.h"
#include "terminal.h"

struct terminal{
	char* nome;
	char* localizacao;
	struct roteador* rot;
};

struct celTerm{
	Terminal* term;
	struct celTerm* prox;
};

CelTerm* CadastraTerminal (char* nomeTerm, char* localizacao){
	
	CelTerm* celT = (CelTerm*) malloc(sizeof(CelTerm));
	Terminal* t = criaTerminal(nomeTerm, localizacao);
	
	celT->term = t;
	celT->prox = listaTerm;
	listaTerm = celT;
	
	return listaTerm;
}

void ConectaTerminal (char* nomeTerm, char* nomeRot, CelTerm* listaTerm){
	
}

void DesconectaTerminal (char* nomeTerm, CelTerm* listaTerm){
	
}

CelTerm* RemoveTerminal (char* nomeTerm, CelTerm* listaTerm){
	
}

void EnviarPacoteDados (char* nomeTerm1, char* nomeTerm2, CelTerm* listaTerm){
	
}

void FrequenciaTerminal (char* localizacao, CelTerm* listaTerm){
	
}

CelTerm* InicializaListaTerm (){
	
	CelTerm* listaTerm = (CelTerm*) malloc(sizeof(CelTerm));
	listaTerm->term = NULL;
	listaTerm->prox = NULL;
	
	return listaTerm;
}

//Auxiliares

static Terminal* criaTerminal (char* nomeTerm, char* localizacao){
	
	Terminal* t = (Terminal*) malloc(sizeof(Terminal));
	t->nome = (char*) malloc((sizeof(strlen(nomeTerm))+1)*sizeof(char));
	t->localizacao = (char*) malloc((sizeof(strlen(localizacao))+1)*sizeof(char));
	
	strcpy(t->nome, nomeTerm);
	strcpy(t->localizacao, localizacao);
	t->rot = NULL;
	
	return t;
}
