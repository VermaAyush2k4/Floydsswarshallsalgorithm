#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include<mpi.h>
#define VERTICES 1250
#define ROOT 0
static int vertices;
void floydWarshall(int* matrix,int myrank,int size) {
	int k,i,j,dist;
	int* kthRow=(int*)malloc(vertices *sizeof(int));
	int division=vertices/size;
	for(k=0;k<vertices;k++){
		int x;
		int positionK=k%division;
		int ownerK=(k/division)%size;
		if(myrank==ownerK){
			for (x=0;x<vertices;x++){
				kthRow[x]=matrix[positionK*vertices+x];
			}
		}
		MPI_Bcast(kthRow,vertices,MPI_INT,ownerK,MPI_COMM_WORLD);
		for(i=0;i<division;i++){
			for(j=0;j<vertices;j++) {
				dist=matrix[i*vertices+k]+kthRow[j];
				if(dist<matrix[i*vertices+j]){
					matrix[i*vertices+j]=dist;
				}
			}
		}
	}
	free(kthRow);
}
int main(int argc,char** argv) {
	struct timeval TimeValue_Start;
    struct timezone TimeZone_Start;
    struct timeval TimeValue_Final;
    struct timezone TimeZone_Final;
    long time_start,time_end;
    double time_overhead;
    vertices=VERTICES;
	int size,myrank;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Bcast(&vertices,1,MPI_INT,0,MPI_COMM_WORLD);
    int* adj_matrix=(int*)malloc(vertices*vertices*sizeof(int*));
    int* result_matrix=(int*)malloc(vertices*vertices*sizeof(int*));
    int* matrix=(int*)malloc(vertices*(vertices/size)*sizeof(int*));
	if(myrank==ROOT) {
	    for(int i=0;i<vertices;i++){
	        for(int j=0;j<vertices;j++){
	            if(i==j)
	            	adj_matrix[i*vertices+j]=0;
	            else
	            	adj_matrix[i*vertices+j]=rand()%10000;
	        }
	    }
	}
	int division=vertices*(vertices/size);
	MPI_Scatter(adj_matrix,division,MPI_INT,matrix,division,MPI_INT,0,MPI_COMM_WORLD);
	if (myrank==ROOT) {
    	gettimeofday(&TimeValue_Start,&TimeZone_Start);
	}
	floydWarshall(matrix,myrank,size);
	MPI_Gather(matrix,division,MPI_INT,result_matrix,division,MPI_INT,0,MPI_COMM_WORLD);
	free(matrix);
	if (myrank == ROOT) {
	    gettimeofday(&TimeValue_Final,&TimeZone_Final);
	    time_start=TimeValue_Start.tv_sec*1000000+TimeValue_Start.tv_usec;
	    time_end=TimeValue_Final.tv_sec*1000000+TimeValue_Final.tv_usec;
	    time_overhead=(time_end-time_start)/1000000.0;
	    printf("\n\n\t\tTime in Seconds (T) :%lf\n",time_overhead);
	}
	MPI_Finalize();
	return 0;
}  
