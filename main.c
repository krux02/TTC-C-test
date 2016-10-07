#include <ttc_c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

int main() {
  printf("hello world\n");

  // Create handler
  ttc_handler_s *ttc_handle = ttc_init();
  int res;

  // Create transpose parameter
  uint32_t    perm[] = { 1, 2, 0 };
  uint32_t    size[] = { 2, 3, 4 };
  ttc_param_s param = { .alpha.s = 1.0f, .beta.s = 1.0f, .lda = NULL, .ldb = NULL, .perm = perm, .size = size, .loop_perm = NULL, .dim = 3};

  // Set TTC options (THIS IS OPTIONAL)
  int maxImplemenations = 10;
  ttc_set_opt( ttc_handle, TTC_OPT_MAX_IMPL,    (void*)&maxImplemenations, 1 );
  int numThreads = 24;
  ttc_set_opt( ttc_handle, TTC_OPT_NUM_THREADS, (void*)&numThreads, 1 );
  char affinity[] = "compact,1";
  ttc_set_opt( ttc_handle, TTC_OPT_AFFINITY,    (void*)affinity, strlen(affinity) );

  // Allocating memory for tensors
  int i;
  int N = 2*3*4;
  double *A = (double*)malloc(sizeof(double) * N);
  double *B = (double*)malloc(sizeof(double) * N);
  
  double C[] = {
    0.0,2.0,4.0,
    6.0,8.0,10.0,
    12.0,14.0,16.0,
    18.0,20.0,22.0,

    1.0,3.0,5.0,
    7.0,9.0,11.0,
    13.0,15.0,17.0,
    19.0,21.0,23.0
  };

  
  for (i = 0; i < N; ++i) {
    A[i] = (double)i;
  }

  res = ttc_transpose( ttc_handle, &param, A, B);

  // Execute transpose
  if (0 != res) {
    fprintf(stderr, "could not do transpose\n");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < N; ++i) {
    if(B[i] != C[i]) {
      fprintf(stderr, "got %f expected %f at index %d\n", B[i], C[i], i);
    }
  }

  printf("end world\n");
}
