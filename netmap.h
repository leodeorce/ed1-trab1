#ifndef NETMAP_H_
#define NETMAP_H_

/*Le o arquivo de entrada e executa seus comandos criando e manipulando um netmap
* inputs: argv e argc da main
* output: 0 caso sucesso, -1 caso nao houve entrada, -2 caso arquivo inexistente
* pre-condicao: arquivo de entrada com formatacao correta
* pos-condicao: funcoes executadas e arquivos de saida criados de acordo
*/
void CriaNetmap (int argv, char** argc);

/*Cria arquivo de extensao DOT contendo o netmap em formato legivel pelo pacote de ferramentas GraphViz
* inputs: lista de terminais e lista de roteadores
* output: nenhum
* pre-condicao: listas inicializadas
* pos-condicao: arquivo saida.dot existe e está preenchido com o netmap recebido
*/
void ImprimeNetMap (void* listaTerm, void* listaRot);

#endif /* NETMAP_H_ */
