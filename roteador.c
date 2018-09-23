#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "roteador.h"

static Roteador* criaRoteador (char* rot, char* operadora);
static void EncadeiaRoteador (CelRot* celR, LsRot* listaRot);
static void DesencadeiaRoteador (CelRot* p, LsRot* listaRot);
static void LiberaTipoRoteador (Roteador* rot);
static void EscreveLOG ();
static void EscreveSAIDA ();

struct celRot{
	CelRot *prox, *ant;
	Roteador* rot;
};

struct roteador{
	char* nome;
	char* operadora;
	LsRot* rotConectados;
};

struct lsRot{
	CelRot *prim, *ult;
};

/* Principais */

void CadastraRoteador (char* rot, char* operadora, LsRot* listaRot){
	Roteador* r = criaRoteador(rot, operadora);
	CelRot* celR = (CelRot*) malloc(sizeof(CelRot));
	celR->rot = r;
	
	printf ("Nome: %s\n", celR->rot->nome);
	printf ("operadora: %s", celR->rot->operadora);	
	
	EncadeiaRoteador(celR, listaRot);
}

void RemoveRoteador (char* nomeRot, LsRot* listaRot){
	CelRot* p = BuscaRoteador(nomeRot, listaRot);   //busca o roteador no netmap
	
	if(p == NULL){
		//IMPRIME MENSAGEM DE ERRO
		printf("Erro: RemoveRoteador: Roteador nao encontrado");
		return;
	}
	
	CelRot* pConec;
	
	CelRot* prc = p->rot->rotConectados->prim;  //celula do primeiro roteador da lista de rot conectados
	
	while(prc != NULL){
		pConec = BuscaRoteador(nomeRot, prc->rot->rotConectados);  //busca a celula onde o roteador esta
		DesencadeiaRoteador(pConec, prc->rot->rotConectados); //Desencadeia da lista de roteadores conectados os quais faz parte
		free(pConec);        
		prc = prc->prox;
	}

	DesencadeiaRoteador(p, listaRot);    //Desencadeia da lista de roteadores do netmap
	LiberaTipoRoteador(p->rot);
	free(p);
	
	printf("Roteador %s removido", nomeRot);
}

void ConectaRoteadores (char* nomeRot1, char* nomeRot2, LsRot* listaRot){
	CelRot* c = BuscaRoteador(nomeRot1, listaRot);  //busca o roteador 1 no netmap
	if(c == NULL){
		//IMPRIME MENSAGEM DE ERRO
		printf("Erro: %s nao existe", nomeRot1);
		return;
	}
	Roteador* r1 = c->rot;   //guarda o ponteiro pro tipo roteador 1
	
	c = BuscaRoteador(nomeRot2, listaRot); //busca o roteador 2 no netmap
	if(c == NULL){
		//IMPRIME MENSAGEM DE ERRO
		printf("Erro: %s nao existe", nomeRot2);
		return;
	}
	Roteador* r2 = c->rot; //guarda o ponteiro pro tipo roteador 2
	
	CelRot* c1 = (CelRot*) malloc(sizeof(CelRot));  //cria celula 1 pra armazenar o roteador 1
	c1->rot = r1;
	CelRot* c2 = (CelRot*) malloc(sizeof(CelRot)); //cria uma celula 1 pra armazenar o roteador 2
	c2->rot = r2;
	
	EncadeiaRoteador(c2, c1->rot->rotConectados); //Encadeia celula 2 na lista de roteadores conectados do roteador 1
	EncadeiaRoteador(c1, c2->rot->rotConectados); //Encadeia celula 1 na lista de roteadores conectados do roteador 2
	
	printf("%s e %s conectados", nomeRot1, nomeRot2);
}

void DesconectaRoteadores (char* nomeRot1, char* nomeRot2, LsRot* listaRot){
	CelRot* celNP1 = BuscaRoteador(nomeRot1, listaRot);  //Encontra o roteador 1 no netmap
	if(celNP1 == NULL){
		//IMPRIME MENSAGEM DE ERRO
		printf("Erro: %s nao existe", nomeRot1);
		return;
	}
	CelRot* celNP2 = BuscaRoteador(nomeRot2, listaRot); //Encontra o roteador 2 no netmap
	if(celNP2 == NULL){
		//IMPRIME MENSAGEM DE ERRO
		printf("Erro: %s nao existe", nomeRot2);
		return;
	}
	
	CelRot* celRC = BuscaRoteador(nomeRot2, celNP1->rot->rotConectados); //Encontra o roteador 2 na lista de roteadores conectados do roteador 1
	if(celRC == NULL){
		//IMPRIME MENSAGEM DE ERRO
		printf("Erro: DesconectaRoteadores: Roteadores nao conectados");
		return;
	}
	DesencadeiaRoteador(celRC, celNP1->rot->rotConectados); //Desencadeia o roteador 2 da lista de roteadores conectados do roteador 1
	free(celRC);

	celRC = BuscaRoteador(nomeRot1, celNP2->rot->rotConectados); //Encontra o roteador 1 na lista de roteadores conectados do roteador 2
	DesencadeiaRoteador(celRC, celNP2->rot->rotConectados); //Desencadeia o roteador 2 da lista de roteadores conectados do roteador 1
	free(celRC);
	
	printf("%s e %s desconectados", nomeRot1, nomeRot2);
}

void FrequenciaOperadora (char* operadora, LsRot* listaRot){
	int qtdRot = 0;
	CelRot* p = listaRot->prim;
	
	while (p != NULL){
		if(strcmp(p->rot->operadora, operadora) == 0){
			qtdRot++;
		}
		p = p->prox;
	}
	printf("Frequencia operadora %s: %d", operadora, qtdRot);
}

/* Auxiliares Compartilhados */

LsRot* InicializaListaRot(){                    
	LsRot* listaRt = (LsRot*) malloc(sizeof(LsRot));
	listaRt->prim = NULL;
	listaRt->ult = NULL;
	
	return listaRt;
}

CelRot* BuscaRoteador (char* nomeRot, LsRot* listaRot){
	CelRot* p = listaRot->prim;
	
	while((p!=NULL) && (strcmp(p->rot->nome, nomeRot) != 0)){
		p = p->prox;
	}	
	return p;
}

char* retornaNomeRot(CelRot* rot){
	return rot->rot->nome;
}

LsRot* retornaRotConectados(CelRot* rot){
	return rot->rot->rotConectados;
}

CelRot* retornaPrim (LsRot* listaRot){
	return listaRot->prim;
}

CelRot* retornaProxCel (CelRot* celrot){
	return celrot->prox;
}

/* Auxiliares Exclusivos */

static Roteador* criaRoteador(char* rot, char* operadora){ 
	Roteador* r = (Roteador*) malloc(sizeof(Roteador));	
	r->nome = (char*) malloc((strlen(rot)+1)*sizeof(char));
	r->operadora = (char*) malloc((strlen(operadora)+1)*sizeof(char));
	
	strcpy(r->nome, rot);
	strcpy(r->operadora, operadora);
	
	r->rotConectados = InicializaListaRot();
	
	puts("Roteador criado");
	
	return r;
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

static void LiberaTipoRoteador(Roteador* rot){
	free(rot->nome);
	free(rot->operadora);
	
	CelRot* p = rot->rotConectados->prim;
	CelRot* aux;
	
	while(p != NULL){  //Libera as celulas da lista de roteadores conectados
		aux = p;
		p = p->prox;
		free(aux);
	}
	free(rot->rotConectados);  //Libera a lista de roteadores conectados
	free(rot);
}

static void EscreveLOG (){
	// Chamado em qualquer comando que nao possa ser executado.
}

static void EscreveSAIDA (){
	// Chamado em FrequenciaOperadora.
}
