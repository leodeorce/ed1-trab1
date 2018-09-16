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
	
	EncadeiaRoteador(celR, listaRot);
};

void RemoveRoteador (char* nomeRot, LsRot* listaRot){
	CelRot* p = BuscaRoteador(nomeRot, listaRot);
	CelRot* pConec;
	
	CelRot* prc = p->rot->rotConectados->prim;
	
	while(prc != NULL){
		pConec = BuscaRoteador(nomeRot, prc->rot->rotConectados);
		DesencadeiaRoteador(pConec, prc->rot->rotConectados);   //Desencadeia da lista de roteadores conectados os quais faz parte
		prc = prc->prox;
	}
	
	DesencadeiaRoteador(p, listaRot);
	LiberaTipoRoteador(p->rot);
	free(p);
	
}

void ConectaRoteadores (char* nomeRot1, char* nomeRot2, LsRot* listaRot){
	CelRot* c = BuscaRoteador(nomeRot1, listaRot);
	Roteador* r1 = c->rot;
	c = BuscaRoteador(nomeRot2, listaRot);
	Roteador* r2 = c->rot;
	
	CelRot* c1 = (CelRot*) malloc(sizeof(CelRot));
	c1->rot = r1;
	CelRot* c2 = (CelRot*) malloc(sizeof(CelRot));
	c2->rot = r2;
	
	EncadeiaRoteador(c2, c1->rot->rotConectados);
	EncadeiaRoteador(c1, c2->rot->rotConectados);
	
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

static void EncadeiaRoteador(CelRot* celR, LsRot* listaRot){
	celR->prox = NULL;
	
	if(listaRot->prim == NULL){
		listaRot->prim = celR;
		listaRot->ult = celR;
			
		celR->prox = NULL;
		celR->ant = NULL;
	}else{
		listaRot->ult->prox = celR;
		celR->ant = listaRot->ult;
		listaRot->ult = celR;		
	}
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
		listaRot->prim = p->prox;
		p->prox->ant = NULL;
		return;
	}
	if(p == listaRot->ult){
		listaRot->ult = p->ant;
		p->ant->prox = NULL;
		return;
	}else{
		p->ant->prox = p->prox;
		p->prox->ant = p->ant;
		return;
	}
}

static void LiberaListaRot(LsRot* lsRot){
	CelRot* p = lsRot->prim;
	CelRot* k;
	
	while(p != NULL){
		p->rot = NULL;
		k = p;
		p = p->prox;
		free(k);
	}
	lsRot->prim = NULL;
	lsRot->ult = NULL;
}

static void LiberaTipoRoteador(Roteador* rot){
	free(rot->nome);
	free(rot->operadora);
	LiberaListaRot(rot->rotConectados);
}
