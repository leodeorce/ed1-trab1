#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "terminal.h"

struct terminal{
	char* nome;
	char* localizacao;
	CelRot* rot;
};

struct celTerm{
	Terminal* term;
	CelTerm* prox;
};

CelTerm* CadastraTerminal (char* nomeTerm, char* localizacao, CelTerm* listaTerm){
	
	CelTerm* celT = (CelTerm*) malloc(sizeof(CelTerm));
	Terminal* t = criaTerminal(nomeTerm, localizacao);
	
	celT->term = t;
	celT->prox = listaTerm;
	listaTerm = celT;
	
	printf ("Nome:\t%s\n", listaTerm->term->nome);
	printf ("Localizacao:\t%s\n", listaTerm->term->localizacao);
	
	return listaTerm;
}

void ConectaTerminal (char* nomeTerm, char* nomeRot, CelTerm* listaTerm, LsRot* listaRot){
	
	CelTerm* celT = BuscaTerminal(nomeTerm, listaTerm);
	
	if(celT == NULL){
		//Escreve no arquivo que o terminal nao existe.
		puts("Erro: ConectaTerminal: Terminal nao encontrado");
		return;
	}
	
	if(celT->term->rot != NULL){
		//Escreve no arquivo que o terminal ja esta conectado.
		puts("Erro: ConectaTerminal: Terminal ja conectado");
		
	}else{
		CelRot* celR = BuscaRoteador(nomeRot, listaRot);
		celT->term->rot = celR;
		puts("Conectado ao roteador");
	}
}

void DesconectaTerminal (char* nomeTerm, CelTerm* listaTerm){
	
	CelTerm* celT = BuscaTerminal(nomeTerm, listaTerm);
	
	if(celT == NULL){
		// Escreve no arquivo que nomeTerm nao existe
		puts("Erro: DesconectaTerminal: Terminal nao encontrado");
		return;
		
	}else
		celT->term->rot = NULL;
	puts("Terminal desconectado");
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
		puts("Erro: RemoveTerminal: Terminal nao encontrado");
		return NULL;
	}
	
	if(auxProx == listaTerm){
		puts("Terminal removido: primeiro da lista");
		listaTerm = listaTerm->prox;
	}else{
		puts("Terminal removido: meio ou final da lista");
		auxAnt->prox = auxProx->prox;
	}
	
	free (auxProx->term->nome);
	free (auxProx->term->localizacao);
	free (auxProx->term);
	free (auxProx);
	
	return listaTerm;
}

void EnviarPacotesDados (char* term1, char* term2, CelTerm* listaTerm){

	CelTerm* celT1 = BuscaTerminal(term1, listaTerm);
	if (celT1 == NULL){
		//ERRO, TERMINAL NAO CADASTRADO
		return;
	}
	
	CelTerm* celT2 = BuscaTerminal(term2, listaTerm);
	if (celT2 == NULL){
		//ERRO, TERMINAL NAO CADASTRADO
		return;
	}
	
	if (celT1->term->rot != NULL && celT2->term->rot != NULL){
		
		char* rotT1 = retornaNomeRot(celT1->term->rot);
		char* rotT2 = retornaNomeRot(celT2->term->rot);
		
		if (!strcmp(rotT1, rotT2)){
			puts("SIM");
			return;
		}
		
	}else{
		puts("NAO");
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
		puts("NAO");
	else
		puts("SIM");
}

void FrequenciaTerminal (char* localizacao, CelTerm* listaTerm){
	
	int qtdeTerm = 0;
	CelTerm* celT = listaTerm;
	
	while(celT != NULL){
		
		if(!strcmp(celT->term->localizacao, localizacao))
			qtdeTerm++;
		
		celT = celT->prox;
	}
	printf("qtdeTerm:\t%d\n", qtdeTerm);
}

//Auxiliares

CelTerm* InicializaListaTerm (){
	
	CelTerm* listaTerm = NULL;
	puts("Lista inicializada");
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
		
		puts("Terminal desruido");
		listaTerm = aux;
	}
	free(listaTerm);
	puts("Lista de terminais destruida");
}

void DesconectaRoteador (char* nomeRot, CelTerm* listaTerm, LsRot* listaRot){
	
	CelRot* celR = BuscaRoteador(nomeRot, listaRot);
	CelTerm* celT = listaTerm;
	
	if(celR != NULL){
		while(celT != NULL){
			
			if(celT->term->rot == celR){
				celT->term->rot = NULL;
				puts("Desconectei roteador de um terminal");
			}
			celT = celT->prox;
		}
	}else
		puts("Erro: DesconectaRoteador: Roteador nao encontrado");
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
