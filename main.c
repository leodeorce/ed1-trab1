#include <stdio.h>
#include <stdlib.h>
#include "terminal.h"

int main ()
{
	char* terms[2] 	= {"Ter1", "Ter2"};
	char* rots[2] 	= {"Rot1", "Rot2"};
	char* locais[2]	= {"VilaVelha", "Vitoria"};
	char* ops[2] 	= {"GVT", "NET"};
	int i;
	
	LsRot*	listaRot = InicializaListaRot();
	CelTerm* listaTerm = InicializaListaTerm();
	
	for (i = 0; i <= 1; i++)
		CadastraRoteador (rots[i], ops[i], listaRot);
	
	for (i = 0; i <= 1; i++)
		listaTerm = CadastraTerminal (terms[i], locais[i], listaTerm);
	
	for (i = 0; i <= 1; i++)
		ConectaTerminal (terms[i], rots[i], listaTerm, listaRot);
	
	for (i = 0; i <= 1; i++)
		RemoveRoteador (rots[i], listaRot);
	
	// for (i = 0; i <= 0; i++)
		// DesconectaRoteador (rots[i], listaTerm, listaRot);
	
	for (i = 0; i <= 1; i++)
		ConectaTerminal (terms[i], rots[i], listaTerm, listaRot);
	
	LiberaListaTerm (listaTerm);
	
	free (listaRot);
	
	return 0;
}