#include <mpi.h>
#include <stdio.h>


#define NHIJOS 3
#define EXE_HIJO "./hijo"
#define COUNT_DATA 1

int main(int argc, char *argv[]) {
	//Proceso Padre

	/*
		Inicio de MPI
		Obtenemos el rango
	*/
	MPI_Init(&argc, &argv);

	printf("Inicio proceso Padre:\n");


	/*
		Creamos los procesos hijo
	*/
	printf("Creando %d hijos ...\n", NHIJOS);


	int mirango;
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
	MPI_Comm intercom;
	//MPI_Comm_spawn(char *command, char *argv[], int numprocs, MPI_Infoinfo, int root, MPI_Comm comm, MPI_Comm *intercom, int array_of_errcodes[])
	MPI_Comm_spawn(
		EXE_HIJO, MPI_ARGV_NULL, NHIJOS, MPI_INFO_NULL, 
		mirango, MPI_COMM_WORLD, &intercom,	MPI_ERRCODES_IGNORE
	);
	

	/*
		Creamos el comunicador global
	*/
	MPI_Comm intracom;
	MPI_Intercomm_merge(intercom, 0, &intracom);

	/*
		Realizamos el envio
	*/

	int DATA = 1234;

	printf("Proceso padre dice: Hijos, os envio el dato: %d\n", DATA);
	MPI_Bcast(&DATA, COUNT_DATA, MPI_INT, mirango, intracom);
	
	//Finalizamos mpi
	MPI_Finalize();

	printf("Fin del proceso padre\n");
	return 0;
}