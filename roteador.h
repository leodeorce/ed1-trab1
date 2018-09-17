#ifndef ROTEADOR_H_
#define ROTEADOR_H_
//Tipo do roteador, um item da celula (deve conter nome, operadora e ponteiro pra lista de roteadores na qual se conecta)
typedef struct roteador Roteador;

//Tipo da celula da lista de roteadores (deve conter tipo do roteador e prox)
typedef struct celRot CelRot;

//Tipo da lista de roteadores; (sugiro com sentinela pro retorno da maioria das funcoes ser void);
typedef struct lsRot LsRot;

/*Insere um roteador na lista de roteadores do NetMap na ultima posicao
* inputs: nome do roteador e da operadora
* output: nenhum
* pre-condicao: roteador existe
* pos-condicao: roteador inserido no netmap 
*/
void CadastraRoteador (char* rot, char* operadora, LsRot* listaRot);

/*Remove um roteador na lista de roteadores do NetMap
* inputs: nome do roteador
* output: nenhum
* pre-condicao: roteador existe
* pos-condicao: roteador removido do netmap 
*/
void RemoveRoteador (char* nomeRot);

/*Conecta um roteador ao outro
* inputs: nome dos roteadores
* output: nenhum
* pre-condicao: roteadores de entrada existem
* pos-condicao: roteador1 inserido na lista de roteadores do roteador2 e vice-versa
*/
void ConectaRoteadores (char* nomeRot1, char* nomeRot2);

/*Desconecta um roteador do outro
* inputs: nome dos roteadores
* output: nenhum
* pre-condicao: roteadores de entrada existem e estao conectados
* pos-condicao: roteador1 removido da lista de roteadores do roteador2 e vice-versa
*/
void DesconectaRoteadores (char* nomeRot1, char* nomeRot2);

/*Conta quantos roteadores são da operadora de entrada
* inputs: a string que contenha o nome da operadora
* output: quantidade de roteadores da operadora
* pre-condicao: nenhum
* pos-condicao: nenhum
*/
int FrequenciaOperadora (char* operadora);

LsRot* InicializaListaRot();
static Roteador* criaRoteador(char* rot, char* operadora);
static CelRot* BuscaRoteador (char* nomeRot, LsRot* listaRot);
static void DesencadeiaRoteador(CelRot* p, LsRot* listaRot);
static void LiberaListaRotConectados(LsRot* lsRot);
static void LiberaTipoRoteador(Roteador* rot);

#endif /* ROTEADOR_H_ */
