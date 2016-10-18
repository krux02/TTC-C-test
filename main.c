#include <ttc_c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <assert.h>

int main() {
  printf("hello world\n");

  // Create handler
  ttc_handler_s *ttc_handle = ttc_init();
  int err;

  // Set TTC options
  int maxImplemenations = 10;
  err = ttc_set_opt( ttc_handle, TTC_OPT_MAX_IMPL,    (void*)&maxImplemenations, 1 );
  assert(err == 0);
  int numThreads = 24;
  err = ttc_set_opt( ttc_handle, TTC_OPT_NUM_THREADS, (void*)&numThreads, 13 );
  assert(err == 0);
  char affinity[] = "compact,1";
  err = ttc_set_opt( ttc_handle, TTC_OPT_AFFINITY,    (void*)affinity, strlen(affinity) );
  assert(err == 0);

  // Create transpose parameter
  uint32_t    perm[] = { 1, 2, 0 };
  uint32_t    size[] = { 2, 3, 4 };
  ttc_param_s param = { .alpha.d = 1.0, .beta.d = 0.0, .lda = NULL, .ldb = NULL, .datatype = TTC_TYPE_D, .perm = perm, .size = size, .loop_perm = NULL, .dim = 3 };

  
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

  err = ttc_transpose( ttc_handle, &param, A, B);

  // Execute transpose
  if (0 != err) {
    fprintf(stderr, "could not do transpose, err=%d\n", err);
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < N; ++i) {
    if(B[i] != C[i]) {
      fprintf(stderr, "got %f expected %f at index %d\n", B[i], C[i], i);
    }
  }

  printf("end world\n");
}

