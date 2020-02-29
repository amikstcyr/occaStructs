#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include <occa.h>
#include <struct.h>

// to run with 100 size array using CUDA:
// ./main 100 CUDA
// using HIP (typically AMD GPU)
// ./main 100 HIP
// using OpenCL (assumes device 0 and platform 0)
// ./main 100 OpenCL

int main(int argc, char **argv){

  // configure OCCA device
  char *occaConfig = (char*) calloc(BUFSIZ, sizeof(char));
  
  sprintf(occaConfig, "mode: '%s', device_id: 0, platform_id: 0", argv[2]);
  
  printf("Configuring OCCA with: %s\n", occaConfig);
  
  occaDevice device = occaCreateDeviceFromString(occaConfig);

  int N = atoi(argv[1]);
  size_t sz = N*sizeof(struct_t);

  struct_t *q = (struct_t*) calloc(N, sizeof(struct_t));
  for(int n=0;n<N;++n){
    q[n].t = 0;
    q[n].b = 0;
    q[n].val = 0;
  }
  
  occaMemory o_q  = occaDeviceMalloc(device, sz, q, occaDefault);
	
  occaProperties props = occaCreateProperties();
  occaPropertiesSet(props, "defines/p_B", occaInt(16));

  occaKernel structKernel = occaDeviceBuildKernel(device, DFUN "/okl/struct.okl", "structKernel", props);

  occaKernelRun(structKernel, occaInt(N), o_q);

  occaCopyMemToPtr(q, o_q, occaAllBytes, 0, occaDefault);

  for(int n=0;n<N;++n){
    printf("t=%d, b=%d, val=%f\n", q[n].t, q[n].b, q[n].val);
  }
  
  return 0;
}


