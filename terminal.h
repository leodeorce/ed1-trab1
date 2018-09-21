#ifndef TERMINAL_H_
#define TERMINAL_H_

#include "roteador.h"

//Tipo do terminal (sugiro uma estrutura com nome, localizacao e roteador)
typedef struct terminal Terminal;

//Tipo da celula da lista de terminais; (deve conter prox e tipo do terminal);
typedef struct celTerm CelTerm;

/*Insere um terminal na lista de terminais do NetMap na ultima posicao
* inputs: nome do terminal e localizacao
* output: lista de terminais
* pre-condicao: terminal existe
* pos-condicao: terminal inserido no netmap 
*/
CelTerm* CadastraTerminal (char* nomeTerm, char* localizacao, CelTerm* listaTerm);

/*Conecta um terminal a um roteador
* inputs: nome do terminal e do roteador
* output: nenhum
* pre-condicao: terminal e roteador existem
* pos-condicao: terminal conectado no roteador
*/
void ConectaTerminal (char* nomeTerm, char* nomeRot, CelTerm* listaTerm, LsRot* listaRot);

/*Desconecta um terminal de um roteador
* inputs: nome do terminal
* output: nenhum
* pre-condicao: terminal existe e esta conectado a algum roteador
* pos-condicao: terminal desconectado do roteador
*/
void DesconectaTerminal (char* nomeTerm, CelTerm* listaTerm);

/*Remove um terminal da lista de terminais do netmap
* inputs: nome do terminal
* output: lista de terminais
* pre-condicao: terminal existe
* pos-condicao: terminal removido do netmap
*/
CelTerm* RemoveTerminal (char* nomeTerm, CelTerm* listaTerm);

/*Verifica se e possivel enviar dados para outro terminal
* inputs: nome do terminal origem e destino, respectivamente
* output: SIM se for possivel e NAO, caso contrario
* pre-condicao: terminais existem
* pos-condicao: terminais não sao modificados e foram feitas as verificacoes
*/
void EnviarPacoteDados (char* nomeTerm1, char* nomeTerm2, CelTerm* listaTerm);

/*Conta quantos terminais estão na localizacao de entrada
* inputs: a string que contenha uma localizacao
* output: quantidade de terminais naquela regiao
* pre-condicao: nenhum
* pos-condicao: nenhum
*/
void FrequenciaTerminal (char* localizacao, CelTerm* listaTerm);

/*Inicializa lista de terminais
* inputs: nenhum
* output: lista de terminais
* pre-condicao: nenhum
* pos-condicao: lista de terminais inicializada
*/
CelTerm* InicializaListaTerm ();

/*Destroi lista de terminais
* inputs: lista de terminais
* output: nenhum
* pre-condicao: lista de terminais inicializada
* pos-condicao: lista de terminais destruida
*/
void LiberaListaTerm (CelTerm* listaTerm);

/*Encontra terminal com o nome passado na lista de terminais
* inputs: nome do terminal e lista onde procurar
* output: tipo do terminal encontrado ou NULL caso nao encontre
* pre-condicao: lista de terminais inicializada
* pos-condicao: lista de terminais inalterada
*/
Terminal* BuscaTerminal (char* nomeTerm, CelTerm* listaTerm);

/*Desconecta roteador cujo nome e passado dos terminais da lista passada
* inputs: nome do roteador e lista onde procurar
* output: nenhum
* pre-condicao: lista de terminais inicializada
* pos-condicao: roteador desconectado de todos os terminais da lista
*/
void DesconectaRoteador (char* nomeRot, CelTerm* listaTerm, LsRot* listaRot);

static Terminal* criaTerminal (char* nomeTerm, char* localizacao);

#endif /* TERMINAL_H_ */
