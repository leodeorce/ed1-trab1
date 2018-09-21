#ifndef ARQUIVO_H_
#define ARQUIVO_H_

/*Abre o arquivo
* inputs: nome do arquivo
* output: sucesso retorna ponteiro para o arquivo, fracasso retorna NULL
* pre-condicao: arquivo no mesmo diretorio que executavel
* pos-condicao: arquivo aberto
*/
FILE* AbreArquivo (char* arquivo);

/*Fecha o arquivo
* inputs: ponteiro para o arquivo
* output: nenhum
* pre-condicao: arquivo aberto
* pos-condicao: arquivo fechado
*/
void FechaArquivo (FILE* fp);

/*Escreve erro em log.txt
* inputs: nome do item (rot#/term#) inexistente no netmap
* output: nenhum
* pre-condicao: nenhuma
* pos-condicao: linha em log.txt preenchida com erro
*/
void EscreveLog (char* nomeItem);

/*Escreve saida do comando em saida.txt
* inputs: nome do comando, itens relevantes e valor retornado pelo comando
* output: nenhum
* pre-condicao: nome2 igual a nome1 caso saida pedir somente um item
* pos-condicao: linha em saida.txt preenchida com saida do comando
*/
void EscreveSaida (char* nomeComando, char* nome1, char* nome2, int valor);

#endif /* ARQUIVO_H_ */
