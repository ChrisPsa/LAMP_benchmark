#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <timer.h>
#include "mkl.h"

int main(int argc, char* argv[])
{
  int n, rhs;
  int* ipiv;
  double *A, *B;
  double dtime, dtime_save = DBL_MAX, cs_time = DBL_MAX;

  if (argc < 3) {
    printf("pass me 2 arguments: n, rhs, [name]\n");
    return (-1);
  } else {
    n = atof(argv[1]);
    rhs = atof(argv[2]);
  }

  srand48((unsigned)time((time_t*)NULL));

  A = (double*)mkl_malloc(n * n * sizeof(double), 64);
  B = (double*)mkl_malloc(n * rhs * sizeof(double), 64);
  ipiv = (int*)mkl_malloc(1 * n * sizeof(double), 64);

  for (int i = 0; i < n * rhs; i++) B[i] = drand48();

  for (int it = 0; it < LAMP_REPS; it++) {

    for (int i = 0; i < n * n; i++) A[i] = drand48();
    for (int i = 0; i < n; i++) ipiv[i] = 0;

    /*printf("A = [\n");*/
    /*for (int i = 0; i < n; i++) {*/
    /*for (int j = 0; j < n; j++)*/
    /*printf(" %lf\t", A[i + j * n]);*/
    /*printf(";\n");*/
    /*}*/
    /*printf("];\n");*/

    /*printf("B = [\n");*/
    /*for (int i = 0; i < n; i++) {*/
    /*for (int j = 0; j < rhs; j++)*/
    /*printf(" %lf\t", B[i + j * n]);*/
    /*printf(";\n");*/
    /*}*/
    /*printf("];\n");*/

    /*printf("C = A\\B;\n");*/

    int info = 1;

    cs_time = cache_scrub();

    dtime = cclock();
    dgesv_(&n, &rhs, A, &n, ipiv, B, &n, &info);
    dtime_save = clock_min_diff(dtime_save, dtime);

    /*printf("C2 = [\n");*/
    /*for (int i = 0; i < n; i++) {*/
    /*for (int j = 0; j < rhs; j++)*/
    /*printf(" %lf\t", B[i + j * n]);*/
    /*printf(";\n");*/
    /*}*/
    /*printf("];\n");*/
    /*printf("isapprox(C, C2, atol=1e-4)\n");*/
  };
  mkl_free(A);
  mkl_free(B);
  mkl_free(ipiv);

  if (argv[3]) {
    printf("solve_%s_rec;%d;%d;%d;%e;%e\n", argv[3], 0, rhs, n, dtime_save, cs_time);
  } else {
    printf("solve_rec;%d;%d;%d;%e;%e\n", 0, rhs, n, dtime_save, cs_time);
  }

  return (0);
}
