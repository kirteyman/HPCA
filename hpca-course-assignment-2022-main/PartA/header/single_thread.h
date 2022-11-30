#include <immintrin.h>
void singleThread(int N, int *matrix_1, int *matrix_2, int *rslt)
{
  int i, j, k;
        for (j = 0; j < N; j = j + 2){

            int coloumn_1[N];
            int coloumn_2[N];

            for (k = 0; k < N; k = k+1){
               
                coloumn_1[k] = matrix_2[(k*N) + j];
                coloumn_2[k] = matrix_2[(k*N) + j+1];       
            } 

            for(i = 0; i < N; i = i + 2){
                
                int sum = 0;
                for (k= 0 ; k < N; k = k + 8){
 

                        __m256i V_1 = _mm256_mullo_epi32(_mm256_loadu_si256((__m256i*) & matrix_1[i * N + k]), _mm256_loadu_si256((__m256i*)&coloumn_1[k]) ) ;
                
                        __m256i V_2 = _mm256_mullo_epi32(_mm256_loadu_si256((__m256i*) & matrix_1[i * N + k]), _mm256_loadu_si256((__m256i*)&coloumn_2[k]) ) ;
                        V_2 = _mm256_add_epi32(V_2, V_1);
                
                        __m256i V_3 = _mm256_mullo_epi32(_mm256_loadu_si256((__m256i*) & matrix_1[(i+1)*(N)+k]), _mm256_loadu_si256((__m256i*)&coloumn_1[k]) ) ;
                        V_3 = _mm256_add_epi32(V_3, V_2);
                
                        __m256i V_4 = _mm256_mullo_epi32(_mm256_loadu_si256((__m256i*) & matrix_1[(i+1)*(N)+k]), _mm256_loadu_si256((__m256i*)&coloumn_2[k]) ) ;
                        V_4 = _mm256_add_epi32(V_4, V_3);



                        int res[8];
                        _mm256_storeu_si256((__m256i*)&res, V_4);
                        sum += (res[0] + res[1] + res[2] + res[3] + res[4] + res[5] + res[6] + res[7]);
  
                }
            int row = i>>1;
            int col = j>>1;
            rslt[(row*(N>>1))+col] = sum; 
   
        }  
    }
}
