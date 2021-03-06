#ifndef ROTEADOR_H_
#define ROTEADOR_H_

//Tipo do roteador, um item da celula
typedef struct roteador Roteador;

//Tipo da celula da lista de roteadores
typedef struct celRot CelRot;

//Tipo da lista de roteadores;
typedef struct lsRot LsRot;

/*  Principais */

/*Insere um roteador na lista de roteadores do NetMap na ultima posicao
* inputs: nome do roteador e da operadora e ponteiro pra lista de roteadores
* output: nenhum
* pre-condicao: roteador existe
* pos-condicao: roteador inserido no netmap
*/
void CadastraRoteador (char* rot, char* operadora, LsRot* listaRot);

/*Remove um roteador na lista de roteadores do NetMap
* inputs: nome do roteador e o ponteiro pra lista de roteadores
* output: nenhum
* pre-condicao: roteador existe
* pos-condicao: roteador removido do netmap
*/
void RemoveRoteador (char* nomeRot, LsRot* listaRot, void* listaTerm);

/*Conecta um roteador ao outro
* inputs: nome dos roteadores e ponteiro pra lista de roteadores em que estão
* output: nenhum
* pre-condicao: roteadores de entrada existem
* pos-condicao: roteador1 inserido na lista de roteadores do roteador2 e vice-versa
*/
void ConectaRoteadores (char* nomeRot1, char* nomeRot2, LsRot* listaRot);

/*Desconecta um roteador do outro
* inputs: nome dos roteadores e ponteiro pra lista de roteadores
* output: nenhum
* pre-condicao: roteadores de entrada existem e estao conectados
* pos-condicao: roteador1 removido da lista de roteadores do roteador2 e vice-versa
*/
void DesconectaRoteadores (char* nomeRot1, char* nomeRot2, LsRot* listaRot);

/*Conta quantos roteadores são da operadora de entrada
* inputs: a string que contenha o nome da operadora e ponteiro pra lista de roteadores
* output: quantidade de roteadores da operadora
* pre-condicao: nenhum
* pos-condicao: nenhum
*/
void FrequenciaOperadora (char* operadora, LsRot* listaRot);

/*  Auxiliares */

/*Inicializa lista de roteadores
* inputs: nenhum
* output: ponteiro pro tipo lista de roteadores
* pre-condicao: nenhum
* pos condicao: lista de roteadores alocada
*/
LsRot* InicializaListaRot();

/*Busca um roteador  numa lista de roteadores
* inputs: nome do roteador e ponteiro pra lista de roteadores
* output: ponteiro pra celula onde o roteador esta
* pre-condicao: nenhum
* pos-condicao: roteador encontrado ou retorna NULL
*/
CelRot* BuscaRoteador (char* nomeRot, LsRot* listaRot);

/*Percorre o netmap partindo de um roteador atraves de suas conexoes buscando caminho para um roteador especifico
* inputs: celula do roteador de partida, nome do roteador a ser alcancado, vetor guardando historico de busca e inteiro
guardando a primeira posicao vazia do vetor
* output: 1 caso encontre um caminho para nomerot, 0 caso contrario
* pre-condicao: celula nao nula
* pos-condicao: netmap inalterado
*/
int funcaoBusca (CelRot* rot, char* nomerot, char vet[][25], int* i);

/*Retorna nome do roteador referenciado na celula recebida
* inputs: celula do roteador em qualquer lista de roteadores
* output: nome do roteador
* pre-condicao: celula nao nula
* pos-condicao: nenhum
*/
char* retornaNomeRot(CelRot* rot);

/*Imprime roteadores e seus enlaces em linguagem DOT em arquivo saida.dot
* inputs: ponteiro para arquivo aberto e lista de roteadores
* output: nenhum
* pre-condicao: lista inicializada
* pos-condicao: roteadores e seus enlaces impressos (caso existam)
*/
void ImprimeRot (FILE* grafo, LsRot* listaRot);

/*Destroi lista de roteadores e estruturas tipo roteador
* inputs: lista de roteadores
* output: nenhum
* pre-condicao: lista de roteadores inicializada
* pos-condicao: lista de roteadores e estruturas tipo roteador destruidos
*/
void LiberaListaRot (LsRot* listaRot);

#endif /* ROTEADOR_H_ */
