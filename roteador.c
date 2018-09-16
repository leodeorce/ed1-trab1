#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "roteador.h"
#include "terminal.h"

typedef struct celRot CelRot;
struct celRot{
	CelRot *prox, *ant;
	Roteador* rot;
};

typedef struct roteador Roteador;
struct roteador{
	char* nome;
	char* operadora;
	LsRot* rotConectados;
};

typedef struct lsRot LsRot;
struct lsRot{
	CelRot *prim, *ult;
};

void CadastraRoteador (char* rot, char* operadora, LsRot* listaRot){
	Roteador* r = criaRoteador(rot, operadora);
	CelRot* celR = (CelRot*) malloc(sizeof(CelRot));
	celR->rot = r;
	celR->prox = NULL;
	
	if(listaRot->prim == NULL){
		listaRot->prim = celR;
		listaRot->ult = celR;
		
		celR->ant = NULL;
	}else{
		listaRot->ult->prox = celR;
		celR->ant = listaRot->ult;
		listaRot->ult = celR;
		
	}
};

void RemoveRoteador (char* nomeRot, LsRot* listaRot){
	CelRot* p = BuscaRoteador(nomeRot, listaRot);
	
	DesencadeiaRoteador(p, listaRot);         //Desencadeia da lista de roteadores do netmap
	
	CelRot* prc = p->rot->rotConectados->prim;
	
	while(prc!=NULL){
		DesencadeiaRoteador(p, prc->rot->rotConectados); //Desencadeia da lista de roteadores conectados os quais faz parte
		prc = prc->prox;
	}
	
	LiberaTipoRoteador(p->rot);
	free(p);
	
}

void ConectaRoteadores (char* nomeRot1, char* nomeRot2){
	
}

LsRot* InicializaListaRot(){                    
	LsRot* listaRt = (LsRot*) malloc(sizeof(LsRot));
	listaRt->prim = NULL;
	listaRt->ult = NULL;
	
	return listaRt;
	
}

//Auxiliares

static Roteador* criaRoteador(char* rot, char* operadora){ 
	Roteador* r = (Roteador*) malloc(sizeof(Roteador));	
	r->nome = (char*) malloc((strlen(rot)+1)*sizeof(char));
	r->operadora = (char*) malloc((strlen(operadora)+1)*sizeof(char));
	
	strcpy(r->nome, rot);
	strcpy(r->operadora, operadora);
	
	r->rotConectados = InicializaListaRot();
	
	return r;
}

static CelRot* BuscaRoteador (char* nomeRot, LsRot* listaRot){
	CelRot* p = listaRot->prim;
	
	while((p!=NULL) && (strcmp(p->rot->nome, nomeRot) != 0)){
		p = p->prox;
	};	
	return p;
}

static void DesencadeiaRoteador(CelRot* p, LsRot* listaRot){
	if(p == NULL){
		//IMPRIMIR MENSAGEM DE ERRO AQUI
		return;
	}
	if(p == listaRot->prim && p == listaRot->ult){
		listaRot->prim = NULL;
		listaRot->ult = NULL;
		return;
	}
	if(p == listaRot->prim){
		listaRot->prim = listaRot->prim->prox;
		listaRot->prim->ant = NULL;
	}
	if(p == listaRot->ult){
		listaRot->ult = listaRot->ult->ant;
		listaRot->ult->prox = NULL;
	}else{
		p->ant->prox = p->prox;
		p->prox->ant = p->ant;
	}
}

static void LiberaListaRot(LsRot* lsRot){
	CelRot* p = lsRot->prim;
	CelRot* k;
	
	while(p != NULL){
		LiberaTipoRoteador(p->rot);
		k = p;
		free(k);
		p = p->prox;
	}
}

static void LiberaTipoRoteador(Roteador* rot){
	free(rot->nome);
	free(rot->operadora);
	LiberaListaRot(rot->rotConectados);
}
