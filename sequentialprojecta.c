#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define vertices 1250
void floydWarshall(int adj_matrix[vertices][vertices]){
    static int distances[vertices][vertices];
    for(int i=0;i<vertices;i++){
        for(int j=0;j<vertices;j++){
            distances[i][j]=adj_matrix[i][j];
        }
    }
    for(int k=0;k<vertices;k++){
        for(int i=0;i<vertices;i++){
            for(int j=0;j<vertices;j++){
                if(distances[i][j]>distances[i][k]+distances[k][j]){
                    distances[i][j]=distances[i][k]+distances[k][j];
                }
             }
        }
    }
}
int main(int argc,char** argv){
    struct timeval TimeValue_Start;
    struct timezone TimeZone_Start;
    struct timeval TimeValue_Final;
    struct timezone TimeZone_Final;
    long time_start, time_end;
    double time_overhead;
    static int adj_matrix[vertices][vertices];
    for(int i=0;i<vertices;i++){
        for(int j=0;j<vertices;j++){
            if(i==j)
                adj_matrix[i][j]=0;
            else
                adj_matrix[i][j]=rand()%10000;
        }
    }
    gettimeofday(&TimeValue_Start,&TimeZone_Start);
    floydWarshall(adj_matrix);
    gettimeofday(&TimeValue_Final,&TimeZone_Final);
    time_start=TimeValue_Start.tv_sec*1000000+TimeValue_Start.tv_usec;
    time_end=TimeValue_Final.tv_sec*1000000+TimeValue_Final.tv_usec;
    time_overhead=(time_end -time_start)/1000000.0;
    printf("\n\n\t\t Time in Seconds (T):%lf\n",time_overhead);
    FILE *fptr;
    int v=vertices;
    fptr=fopen("sequential.txt","a");
    fprintf(fptr,"%d %lf\n",v,time_overhead);
    fclose(fptr);
}
