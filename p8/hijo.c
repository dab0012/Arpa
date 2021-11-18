#include <mpi.h>
// #include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{	
	
	//Proceso Hijo
	//-------------

	//Inicializar mpi
	MPI_Init(&argc, &argv);

	/*
		Obtenemos algunos datos de interes
	*/

	//Rango del proceso actual
	int mirango;
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);

	/*
		Obtenemos un comunicador entre padres e hijos
	*/
	MPI_Comm intercom, intracom;
	//int MPI_Comm_get_parent(MPI_Comm *intercom) 
	MPI_Comm_get_parent(&intercom);	
	//Fusionamos los comunicadores en uno global
	//int MPI_CHARercomm_merge(MPI_Comm intercom, int orden, MPI_Comm* intracom) 
	MPI_Intercomm_merge(intercom, 1, &intracom);
	//Obtenemos el rango del proceso actual en el nuevo comunicador
	int rangoGlobal;
	MPI_Comm_rank(intracom, &rangoGlobal);

	printf("Proceso hijo %d dice: he sido creado\n", rangoGlobal);

	

	/*
		Recepcion del mensaje del padre
	*/

	int rangoPadre = 0;
	int count_padre = 1;
	int datosPadre;
	MPI_Bcast(&datosPadre, count_padre, MPI_INT, rangoPadre, intracom);
	printf("Proceso hijo %d dice: mi padre me ha dicho %d\n", rangoGlobal, datosPadre);

	/*
		Envio del mensaje del hijo de menor rango
	*/
	int rangoHijoMenor = 1;
	int count_hijoMenor = 1;

	int datosHijo;
	if (rangoGlobal == rangoHijoMenor) {
		datosHijo = 6789;
		printf("Proceso hijo %d dice: Soy el hijo de menor rango y os envio el dato: %d\n", rangoGlobal, datosHijo);
		MPI_Bcast(&datosHijo, count_hijoMenor, MPI_INT, rangoHijoMenor, intracom);
	}

	/*
		Recepcion del mensaje del hijo de menor rango
	*/	
	else {
		MPI_Bcast(&datosHijo, count_hijoMenor, MPI_INT, rangoHijoMenor, intracom);
		printf("Proceso hijo %d dice: mi hermano me ha dicho %d\n", rangoGlobal, datosHijo);
	}

	// printf("Proceso hijo %d ha finalizado\n", rangoGlobal);
	MPI_Finalize();
	printf("Fin del proceso hijo %d\n", rangoGlobal);
	return 0;
}