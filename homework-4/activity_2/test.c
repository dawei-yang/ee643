#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <string.h>

// See for the (bad) default random number generator
#define RAND_SEED 842270

// Number of bytes to broadcast

#define NUM_BYTES 100000000

///////////////////////////////////////////////////////
//// program_abort() and print_usage() functions //////
///////////////////////////////////////////////////////

static void program_abort(char *exec_name, char *message);
static void print_usage();

// Abort, printing the usage information only if the
// first argument is non-NULL (and hopefully set to argv[0]), and
// printing the second argument regardless.
static void program_abort(char *exec_name, char *message) {
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  if (my_rank == 0) {
    if (message) {
      fprintf(stderr,"%s",message);
    }
    if (exec_name) {
      print_usage(exec_name);
    }
  }
  MPI_Abort(MPI_COMM_WORLD, 1);
  exit(1);
}

// Print the usage information
static void print_usage(char *exec_name) {
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

  if (my_rank == 0) {
    fprintf(stderr,"Usage: smpirun --cfg=smpi/bcast:mpich -np <num processes>\n");
    fprintf(stderr,"              -platform <XML platform file> -hostfile <host file>\n");
    fprintf(stderr,"              %s <bcast implementation name> [-c <chunk size>]\n",exec_name);
    fprintf(stderr,"MPIRUN arguments:\n");
    fprintf(stderr,"\t<num processes>: number of MPI processes\n");
    fprintf(stderr,"\t<XML platform file>: a Simgrid platform description file\n");
    fprintf(stderr,"\t<host file>: MPI host file with host names from the platform file\n");
    fprintf(stderr,"PROGRAM arguments:\n");
    fprintf(stderr,"\t<bcast implementation name>: the name of the broadcast implementaion (e.g., naive_bcast)\n");
    fprintf(stderr,"\t[-c <chunk size>]: chunk size in bytes for message splitting (optional)\n");
    fprintf(stderr,"\n");
  }
  return;
}

///////////////////////////
////// Main function //////
///////////////////////////

int main(int argc, char *argv[])
{
	int i,j;
	int chunk_size = NUM_BYTES;
	char *bcast_implementation_name;

	// Parse command-line arguments (not using getopt because not thread-safe
	// and annoying anyway). The code below ignores extraneous command-line
	// arguments, which is lame, but we're not in the business of developing
	// a cool thread-safe command-line argument parser.

	MPI_Init(&argc, &argv);

	// Bcast implementation name
	if (argc < 2) {
		program_abort(argv[0],"Missing <bcast implementation name> argument\n");
	} else {
		bcast_implementation_name = argv[1];
	}

	// Check that the implementation name is valid
	if (strcmp(bcast_implementation_name, "naive_bcast") &&
		strcmp(bcast_implementation_name, "default_bcast") &&
		strcmp(bcast_implementation_name, "ring_bcast") &&
		strcmp(bcast_implementation_name, "pipelined_ring_bcast") &&
		strcmp(bcast_implementation_name, "asynchronous_pipelined_ring_bcast") &&
		strcmp(bcast_implementation_name, "asynchronous_pipelined_bintree_bcast")) {
			char message[256];
			sprintf(message, "Unknown bcast implementation name '%s'\n",bcast_implementation_name);
			program_abort(NULL,message);
	}

	// Chunk size optional argument
	for (i=1; i < argc; i++) {
		if (!strcmp(argv[i],"-c")) {
			if ((i+1 >= argc) || (sscanf(argv[i+1],"%d",&chunk_size) != 1)) {
				program_abort(argv[0],"Invalid <chunk size> argument\n");
			}
		}
	}

	// Determine rank and number of processes
	int num_procs;
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	// Allocate buffer
	int checksum;
	char *buffer;

	if ((buffer = malloc(sizeof(char) * NUM_BYTES)) == NULL) {
		program_abort(argv[0],"Out of memory!");
	}


	// On rank 0 fill the buffer with random data 
	if (0 == rank) { 
		checksum = 0;
		int i = 65;
		for (j = 0; j < 40; j++) {
			buffer[j] = i;
			i++; 
			checksum += buffer[j];
		}
		printf("Rank 0 checksum = %d\n", checksum);
		printf("buffer is %s\n", buffer);
	}

	// Start the timer
	double start_time;
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {  
		start_time = MPI_Wtime();
	}

	/////////////////////////////////////////////////////////////////////////////
	//////////////////////////// TO IMPLEMENT: BEGIN ////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	// char *bcast_implementation_name:   the bcast implementation name (argument #1)
	// int chunk_size:                    the chunk size (optional argument #2)
	// int NUM_BYTES:                     the number of bytes to broadcast
	// char *buffer:                      the buffer to broadcast

	// Process rank 0 should be  the source of the broadcast

	// #include "bcast_solution.c"
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(strcmp(bcast_implementation_name, "default_bcast") == 0) {
		MPI_Bcast(&buffer, 2, MPI_INT, 0, MPI_COMM_WORLD);
	}
	if(strcmp(bcast_implementation_name, "naive_bcast") == 0) {
		char *tmp = malloc(sizeof(char) * 10); 
		if(rank == 0) {
			
/* 			for(int i = 0; i< 10; i++) {
				tmp[i] = buffer[i];	
			}
			printf("send first one: %s\n", tmp);
			for(int i = 1; i< size; i++) {
				MPI_Ssend(&(tmp), 2, MPI_INT, i, 1, MPI_COMM_WORLD);
			}

			for(int i = 0; i< 10; i++) {
				tmp[i] = buffer[i + 10];	
			}
			printf("send second one: %s\n", tmp);

			for(int i = 1; i< size; i++) {
				MPI_Ssend(&(tmp), 2, MPI_INT, i, 2, MPI_COMM_WORLD);
			} */

/* 			MPI_Send(&buffer[0], chunk_size, MPI_BYTE, rank+1, 3, MPI_COMM_WORLD);
			MPI_Send(&buffer[2], chunk_size, MPI_BYTE, rank+1, 3, MPI_COMM_WORLD);
			MPI_Send(&buffer[4], chunk_size, MPI_BYTE, rank+1, 3, MPI_COMM_WORLD);
			MPI_Send(&buffer[6], chunk_size, MPI_BYTE, rank+1, 3, MPI_COMM_WORLD);
			MPI_Send(&buffer[8], chunk_size, MPI_BYTE, rank+1, 3, MPI_COMM_WORLD); */


		}else {

			MPI_Recv(&buffer, 2, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			fprintf(stderr,"receive [%s]\n", buffer);
/* 				MPI_Recv(&buffer[0], chunk_size, MPI_BYTE, rank-1, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if(rank != (num_procs -1)) MPI_Send(&buffer[0], chunk_size, MPI_BYTE, rank+1, 3, MPI_COMM_WORLD);

				MPI_Recv(&buffer[2], chunk_size, MPI_BYTE, rank-1, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if(rank != (num_procs -1)) MPI_Send(&buffer[2], chunk_size, MPI_BYTE, rank+1, 3, MPI_COMM_WORLD);

				MPI_Recv(&buffer[4], chunk_size, MPI_BYTE, rank-1, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if(rank != (num_procs -1)) MPI_Send(&buffer[4], chunk_size, MPI_BYTE, rank+1, 3, MPI_COMM_WORLD);

				MPI_Recv(&buffer[6], chunk_size, MPI_BYTE, rank-1, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if(rank != (num_procs -1)) MPI_Send(&buffer[6], chunk_size, MPI_BYTE, rank+1, 3, MPI_COMM_WORLD);

				MPI_Recv(&buffer[8], chunk_size, MPI_BYTE, rank-1, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if(rank != (num_procs -1)) MPI_Send(&buffer[8], chunk_size, MPI_BYTE, rank+1, 3, MPI_COMM_WORLD); */
		}
	}
	if(strcmp(bcast_implementation_name, "ring_bcast") == 0) {
		if(rank == 0) {
			MPI_Send(&buffer, 2, MPI_INT, rank+1, 1, MPI_COMM_WORLD);
			// fprintf(stderr, "sent {%s} from [%d] to [%d]\n", buffer, rank, rank+1);
		}
		else {
			MPI_Recv(&buffer, 2, MPI_INT, rank-1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// fprintf(stderr, "recv {%s} from [%d]\n", buffer, rank-1);
			if(rank != size-1)  MPI_Send(&buffer, 2, MPI_INT, rank+1, 1, MPI_COMM_WORLD);
		}
	}


	/////////////////////////////////////////////////////////////////////////////
	///////////////////////////// TO IMPLEMENT: END /////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	// All processes send checksums back to the root, which checks for consistency
	char all_ok = 1;
	if (0 == rank) {
	for (j = 1; j < num_procs; j++) {
		int received_checksum;
		MPI_Recv(&received_checksum, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// Print a single message in case of a mismatch, but continue
		// receiving other checksums to ensure that all processes
		// reach the MPI_Finalize()
		if ((all_ok == 1) && (checksum != received_checksum)) {
		fprintf(stderr,"\t** Non-matching checksum! **\n");
		all_ok = 0;
		break;
		}
	}
	} else {
	int checksum=0;
	fprintf(stderr,"Rank [%d]; Buffer [%s]\n", rank, buffer);
	for (j = 0; j < 20; j++) {
		checksum += buffer[j];
	}
	MPI_Send(&checksum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	fprintf(stderr,"send checksum [%d]; from [%d] to [0]\n", checksum, rank);
	}

	// Print out bcast implementation name and wall-clock time, only if the bcast was successful
	MPI_Barrier(MPI_COMM_WORLD);
	if ((0 == rank) && (all_ok)) {
	fprintf(stdout,"implementation: %s | chunksize: %d |  time: %.3lf seconds\n",
			bcast_implementation_name, 
			chunk_size,
			MPI_Wtime() - start_time);
	}
	// Clean-up
	free(buffer);
	MPI_Finalize();

	return 0;
}