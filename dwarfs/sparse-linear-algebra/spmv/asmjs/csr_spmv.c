#ifndef SERIAL
#define SERIAL
#endif 

#include "../common/sparse_formats.h" 
#include "../common/common.h"
#include <getopt.h> 
#include <stdlib.h> 
/**
 * Sparse Matrix-Vector Multiply
 *
 * Multiplies csr matrix by vector x, adds vector y, and stores output in vector out
 */
void spmv_csr_cpu(const csr_matrix* csr,const float* x,const float* y,float* out)
{
	unsigned int row,row_start,row_end,jj;
	float sum = 0;
	for(row=0; row < csr->num_rows; row++)
	{
		sum = y[row];
		row_start = csr->Ap[row];
		row_end   = csr->Ap[row+1];

		for (jj = row_start; jj < row_end; jj++){
			sum += csr->Ax[jj] * x[csr->Aj[jj]];
		}
		out[row] = sum;
	}
}

static struct option long_options[] = { 
  /* name, has_arg, flag, val */ 
  {"stddev", 1, NULL, 's'}, 
  {"density", 1, NULL, 'd'}, 
  {"size", 1, NULL, 'n'},
  {0,0,0,0} 
}; 

int main(int argc, char *argv[]){
  int opt, option_index=0;
  unsigned int dim=1024, density=5000; 
  double normal_stdev=0.01; 
  unsigned long seed = 10000; 
  float *v; 
  stopwatch sw; 

  while ((opt = getopt_long(argc, argv, "s:d:n:", long_options, &option_index)) != -1){
      switch(opt){
        case 's':
          normal_stdev = atof(optarg); 
          break; 
        case 'd': 
          density =  atoi(optarg); 
          break; 
        case 'n':
          dim  = atoi(optarg); 
          break ; 
        default: 
          printf("Usage: %s [-s stddev] [-d density] [-n dimension]", argv[0]);
          break;
      } 
  }

  float *sum = malloc(dim*sizeof(*sum)); 
  float *result = malloc(dim*sizeof(*result)); 
  memset(sum, 0.0, sizeof(sum)); 
  memset(result, 0.0, sizeof(result)); 

  csr_matrix sm =  rand_csr(dim, density, normal_stdev, &seed, stdout); 
  create_vector_from_random(&v, dim); 
  
  stopwatch_start(&sw); 
  spmv_csr_cpu(&sm,v,sum, result);  
  stopwatch_stop(&sw); 

  printf("The spmv benchmark took a total time of %lf seconds!\n", get_interval_by_sec(&sw)); 
}

   






  





