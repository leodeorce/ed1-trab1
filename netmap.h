#ifndef NETMAP_H_
#define NETMAP_H_

/*Le o arquivo de entrada e executa seus comandos criando e manipulando um netmap
* inputs: argv e argc da main
* output: 0 caso sucesso, -1 caso nao houve entrada, -2 caso arquivo inexistente
* pre-condicao: arquivo de entrada com formatacao correta
* pos-condicao: funcoes executadas e arquivos de saida criados de acordo
*/
int CriaNetmap (int argv, char** argc);

#endif /* NETMAP_H_ */
