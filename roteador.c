#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "roteador.h"
#include "terminal.h"

static Roteador* criaRoteador (char* rot, char* operadora);
static void EncadeiaRoteador (CelRot* celR, LsRot* listaRot);
static void DesencadeiaRoteador (CelRot* p, LsRot* listaRot);
static void LiberaTipoRoteador (Roteador* rot);
static int BuscaNomeVet (char* nomerot, char vet[][25], int i);

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
	
	EncadeiaRoteador(celR, listaRot);
}

void RemoveRoteador (char* nomeRot, LsRot* listaRot, void* listaTerm){
	CelRot* p = BuscaRoteador(nomeRot, listaRot);   //busca o roteador no netmap
	
	if(p == NULL){
		char msg[50];
		sprintf(msg,"Erro: Roteador %s nao existe no NetMap", nomeRot);
		EscreveLOG(msg);
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
	
	DesconectaRoteador(p, listaTerm, listaRot);		//Desconecta roteador de todos terminais
	DesencadeiaRoteador(p, listaRot);    //Desencadeia da lista de roteadores do netmap
	LiberaTipoRoteador(p->rot);
	free(p);
}

void ConectaRoteadores (char* nomeRot1, char* nomeRot2, LsRot* listaRot){
	CelRot* c = BuscaRoteador(nomeRot1, listaRot);  //busca o roteador 1 no netmap
	if(c == NULL){
		char msg[50];
		sprintf(msg,"Erro: Roteador %s nao existe no NetMap", nomeRot1);
		EscreveLOG(msg);
		return;
	}
	Roteador* r1 = c->rot;   //guarda o ponteiro pro tipo roteador 1
	
	c = BuscaRoteador(nomeRot2, listaRot); //busca o roteador 2 no netmap
	if(c == NULL){
		char msg[50];
		sprintf(msg, "Erro: Roteador %s nao existe no NetMap", nomeRot2);
		EscreveLOG(msg);
		return;
	}
	Roteador* r2 = c->rot; //guarda o ponteiro pro tipo roteador 2
	
	CelRot* c1 = (CelRot*) malloc(sizeof(CelRot));  //cria celula 1 pra armazenar o roteador 1
	c1->rot = r1;
	CelRot* c2 = (CelRot*) malloc(sizeof(CelRot)); //cria uma celula 1 pra armazenar o roteador 2
	c2->rot = r2;
	
	EncadeiaRoteador(c2, c1->rot->rotConectados); //Encadeia celula 2 na lista de roteadores conectados do roteador 1
	EncadeiaRoteador(c1, c2->rot->rotConectados); //Encadeia celula 1 na lista de roteadores conectados do roteador 2
	
}

void DesconectaRoteadores (char* nomeRot1, char* nomeRot2, LsRot* listaRot){
	CelRot* celNP1 = BuscaRoteador(nomeRot1, listaRot);  //Encontra o roteador 1 no netmap
	if(celNP1 == NULL){
		char msg[50];
		sprintf(msg, "Erro: Roteador %s nao existe no NetMap", nomeRot1);
		EscreveLOG(msg);
		return;
	}
	CelRot* celNP2 = BuscaRoteador(nomeRot2, listaRot); //Encontra o roteador 2 no netmap
	if(celNP2 == NULL){
		char msg[50];
		sprintf(msg,"Erro: Roteador %s nao existe no NetMap", nomeRot2);
		EscreveLOG(msg);
		return;
	}
	
	CelRot* celRC = BuscaRoteador(nomeRot2, celNP1->rot->rotConectados); //Encontra o roteador 2 na lista de roteadores conectados do roteador 1
	if(celRC == NULL){
		char msg[50];
		sprintf(msg, "Erro: DesconectaRoteadores: Roteadores %s e %s nao estao conectados", nomeRot1, nomeRot2);
		EscreveLOG(msg);
		return;
	}
	DesencadeiaRoteador(celRC, celNP1->rot->rotConectados); //Desencadeia o roteador 2 da lista de roteadores conectados do roteador 1
	free(celRC);

	celRC = BuscaRoteador(nomeRot1, celNP2->rot->rotConectados); //Encontra o roteador 1 na lista de roteadores conectados do roteador 2
	DesencadeiaRoteador(celRC, celNP2->rot->rotConectados); //Desencadeia o roteador 2 da lista de roteadores conectados do roteador 1
	free(celRC);
	
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
	char msg[50];
	sprintf(msg, "FREQUENCIAOPERADORA %s %d", operadora, qtdRot);
	EscreveSAIDA(msg);
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

void LiberaListaRot (LsRot* listaRot){
	CelRot* p = listaRot->prim;
	CelRot* aux;
	
	while(p!=NULL){
		LiberaTipoRoteador(p->rot);
		aux = p->prox;
		free(p);
		p = aux;
	}
	
	free(listaRot);	
}

int funcaoBusca (CelRot* rot, char* nomerot, char vet[][25], int* i){  //vet armazenará o nome dos roteadores que já passaram pelo loop
	CelRot* p = rot;
	
	while (p!= NULL){
		if(BuscaNomeVet(p->rot->nome, vet, *i) == 1){ //se o roteador em questao tiver no vet, então passará para o próximo roteador da lista
			p = p->prox;
		}else{                                      
			if(strcmp(p->rot->nome, nomerot) == 0){ //verifica se o roteador é o que está procurando
				return 1;                          //encontrou o roteador
			}else{
				strcpy(vet[*i], p->rot->nome); //guarda o nome do roteador no vet, para não precisar analisar de novo
				(*i)++;                        // incrementa +1 no contador que indica a primeira posição vazia do vetor 
				int r = funcaoBusca (p->rot->rotConectados->prim, nomerot, vet, i);  //chamada recursiva da função para verificar, agora, na lista de roteadores conectados do roteador em questao, começando com o primeiro roteador da lista
				if(r == 1)            //Após buscar na lista de rotconectados ao rot em questao, verifica se o roteador já foi encontrado
					return 1;
				else                  //passa para o próximo roteador da lista em questao
					p = p->prox;			
			}
		}
	}	
	return 0;                      //Passou pelo loop e não encontrou
}

static int BuscaNomeVet (char* nomerot, char vet[][25], int i){
	int j;
	for(j=0; j<i; j++){
		if(strcmp(vet[j], nomerot) == 0)
			return 1;
	}
	return 0;
}

char* retornaNomeRot(CelRot* rot){
	return rot->rot->nome;
}

CelRot* retornaPrimRotCon(CelRot* celrot){
	return celrot->rot->rotConectados->prim;
}

/* Auxiliares Exclusivos */

static Roteador* criaRoteador(char* rot, char* operadora){ 
	Roteador* r = (Roteador*) malloc(sizeof(Roteador));	
	r->nome = (char*) malloc((strlen(rot)+1)*sizeof(char));
	r->operadora = (char*) malloc((strlen(operadora)+1)*sizeof(char));
	
	strcpy(r->nome, rot);
	strcpy(r->operadora, operadora);
	
	r->rotConectados = InicializaListaRot();
	
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