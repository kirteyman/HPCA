#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>



int N, thread_count = 2;
int *matrix_1, *matrix_2, *matrix_3;

void *thread( void *arg )
{
  int threadId, batch_size, row_initialize, row_end;
  double tot;
  
  threadId = *(int *)(arg); 
  batch_size = N / thread_count;
  row_initialize = threadId * batch_size;
  row_end = (threadId+1) * batch_size;


    for (int j = row_initialize ; j < row_end; j= j + 2)
        {   
            int coloumn[N];
            int coloumn_2[N];
            for (int k=0; k < N ; k = k+1){
                coloumn[k] = matrix_2[(k*N) + j];
                coloumn_2[k] = matrix_2[(k*N) + j+1];       
            } 
            for(int i=0; i<N; i = i + 2){
                int tot = 0;
                for (int k=0; k<N; k = k+1){
                    tot = tot + matrix_1[(i*N)+k] * coloumn[k];
                    tot = tot +  matrix_1[(i*N)+k] * coloumn_2[k]; 
                    tot = tot +  matrix_1[(i+1)*(N)+k] * coloumn[k];
                    tot = tot +  matrix_1[(i+1)*(N)+k] * coloumn_2[k]; 
                }
            int row = i>>1;
            int col = j>>1;
            matrix_3[(row*(N>>1))+col] = tot; 
   
        }  
    }

}



// Fill in this function
void multiThread(int n, int *matrix_A, int *matrix_B, int *rslt)
{
  N = n;
  matrix_1 = &matrix_A[0];
  matrix_2 = &matrix_B[0];
  matrix_3 = &rslt[0];
  pthread_t * threads;

  threads = (pthread_t *) malloc( thread_count * sizeof(pthread_t) );

  for (int i = 0; i < thread_count; ++i ) {
    int *threadId;
    threadId = (int *) malloc( sizeof(int) );
    *threadId = i;
    pthread_create( &threads[i], NULL, thread, (void *)threadId );
  }

  for (int i = 0; i < thread_count; ++i ) {
    pthread_join( threads[i], NULL );
  }
    
}

