#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include <occa.h>

// to run with 100 size array using CUDA:
// ./main 100 CUDA
// using HIP (typically AMD GPU)
// ./main 100 HIP
// using OpenCL (assumes device 0 and platform 0)
// ./main 100 OpenCL

int main(int argc, char **argv){

  // configure OCCA device
  char *occaConfig = (char*) calloc(BUFSIZ, sizeof(char));
  
  sprintf(occaConfig, "mode: 'OpenMP'");
  
  printf("Configuring OCCA with: %s\n", occaConfig);
  
  occaDevice device = occaCreateDeviceFromString(occaConfig);

  occaProperties props = occaCreateProperties();
  occaPropertiesSet(props, "defines/p_B", occaInt(16));

  int N = atoi(argv[1]);
  size_t sz = N*sizeof(double);

  double *q = (double*) calloc(N,sizeof(double));
  double *Aq = (double*) calloc(N,sizeof(double));

  for(int n=0;n<N;++n)
    q[n] = 1.2;
  
  occaMemory o_q = occaDeviceMalloc(device, sz, q, occaDefault);
  occaMemory o_Aq = occaDeviceMalloc(device, sz, Aq, occaDefault);
  
  occaKernel ompKernel = occaDeviceBuildKernel(device, DFUN "/okl/omp.okl", "ompKernel", props);
  
  occaKernelRun(ompKernel, occaInt(N), o_q, o_Aq);

  occaCopyMemToPtr(Aq, o_Aq, occaAllBytes, 0, occaDefault);
  occaCopyMemToPtr(q, o_q, occaAllBytes, 0, occaDefault);

  for(int n=0;n<N;++n){
    printf("Aq[%d]=%e\n",n, Aq[n]);
  }
  
  return 0;
}


