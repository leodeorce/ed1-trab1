#include <stdio.h>
#include "netmap.h"

int main (int argv, char* argc[])
{
	int netmap;		// Variavel auxiliar.
	netmap = CriaNetmap(argv, argc);		// CriaNetmap retorna valor de acordo com sucesso da execucao.
	
	if(netmap == 0)
		puts("NetMap criado com sucesso");
	else if(netmap == -1)
		puts("Erro: Nenhuma entrada");
	else if(netmap == -2)
		puts("Erro: Falha ao abrir arquivo");
	
	return netmap;
}