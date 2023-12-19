/*************************************************************
* In this program. I use least square method to solve a problem.
* And employing 2 methods:
*   "Gaussian elimination" and "QR decomposition"
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vector_matrix.h"
#include "gaussian_solver.h"
#include "QR_solver.h"

#define M 15 // sample number
#define N 9 // (degree of the poly.) + 1

double **sample=NULL; // total M sample points. form:(xi,yi)
double *c_answer = NULL; //vector of N elements

double **A = NULL; // matrix of MxN
double *c = NULL; //vector of N elements
double *y = NULL; //vector of M elements

double **B_gaussian = NULL; // matrix of NxN
double **B_QR = NULL; // matrix of NxN
double *c_new_gaussian = NULL; //vector of N elements
double *c_new_QR = NULL; //vector of N elements
double *d_gaussian = NULL; //vector of N elements
double *d_QR = NULL; //vector of N elements

void free_all_memory(){
  free_mtx(sample, M);
  free_vec(c_answer);
  free_mtx(A, M);
  free_vec(c);
  free_vec(y);
  free_mtx(B_gaussian, N);
  free_mtx(B_QR, N);
  free_vec(c_new_gaussian);
  free_vec(c_new_QR);
  free_vec(d_gaussian);
  free_vec(d_QR);
}

double Px(double x){
  double sum = 1 + x;
  for(int i=1; i<N-1; i++){
    sum = 1 + x*sum ;
  }
  return sum;
}

void generate_sample_data(){
  sample = alloc_mtx(M,2);
  for(int i=0; i<M; i++){
    sample[i][0] = i*0.2;
    sample[i][1] = Px(i*0.2);
  }
  c_answer = alloc_vec(N);
  for(int i=0;i<N;i++){
    if(i<8) {
      c_answer[i]=1.0;
    }else{
      c_answer[i]=0.0;
    }
  }
}

void system_init(){
  A = alloc_mtx(M,N);
  for(int i=0; i<M; i++){
    for(int j=0; j<N; j++){
      A[i][j] = pow(sample[i][0],j);
    }
  }
  c = alloc_vec(N);
  for(int i=0; i<N; i++) c[i] = 0.0;
  y = alloc_vec(M);
  for(int i=0; i<M; i++) y[i] = sample[i][1];
}

void new_system(){
  B_gaussian = alloc_mtx(N,N);
  B_QR = alloc_mtx(N,N);
  double **AT = alloc_mtx(N,M);
  transpose_mtx(AT, A, M, N);
  mtx_mtx_mult(B_gaussian, AT, A, N, M, N);
  mtx_mtx_mult(B_QR, AT, A, N, M, N);
  c_new_gaussian = alloc_vec(N);
  for(int i=0; i<N; i++) c[i] = 0.0;
  c_new_QR = alloc_vec(N);
  for(int i=0; i<N; i++) c[i] = 0.0;
  d_gaussian = alloc_vec(N);
  d_QR = alloc_vec(N);
  mtx_vec_mult(d_gaussian, AT, N, M, y);
  mtx_vec_mult(d_QR, AT, N, M, y);
  free_mtx(AT, N);
}

void print_result(){
  printf("------TEST_DATA------\n");
  print_mtx(sample, M, 2);
  printf("------RESULT------\n");
  printf("\tGaussian \t\t new_QR\t\toriginal_QR \t\t\n");
  for(int i=0; i<N; i++){
    printf("%15.15f\t%15.15f\t%15.15f  \n", c_new_gaussian[i], c_new_QR[i], c[i]);
  }
  printf("------ABSOLUTE ERROR 2-norm------\n");
  printf("%15.15f\t%15.15f\t%15.15f  \n", distance_vec_2_norm(c_answer,c_new_gaussian,N)
      , distance_vec_2_norm(c_answer,c_new_QR,N)
      , distance_vec_2_norm(c_answer,c,N));
  printf("------ABSOLUTE ERROR infinite-norm------\n");
  printf("%15.15f\t%15.15f\t%15.15f  \n", distance_vec_infinite_norm(c_answer,c_new_gaussian,N)
      , distance_vec_infinite_norm(c_answer,c_new_QR,N)
      , distance_vec_infinite_norm(c_answer,c,N));
  
  double c_2_norm = vec_2_norm(c_answer, N);
  printf("------RELATIVE ERROR 2-norm------\n");
  printf("%15.15f\t%15.15f\t%15.15f  \n", distance_vec_2_norm(c_answer,c_new_gaussian,N)/c_2_norm
      , distance_vec_2_norm(c_answer,c_new_QR,N)/c_2_norm
      , distance_vec_2_norm(c_answer,c,N)/c_2_norm);
  double c_infinite_norm = vec_infinite_norm(c_answer, N);
  printf("------RELATIVE ERROR infinite-norm------\n");
  printf("%15.15f\t%15.15f\t%15.15f  \n", distance_vec_infinite_norm(c_answer,c_new_gaussian,N)/c_infinite_norm
      , distance_vec_infinite_norm(c_answer,c_new_QR,N)/c_infinite_norm
      , distance_vec_infinite_norm(c_answer,c,N)/c_infinite_norm);
}

/*----------------------------------------------------
 * The main procedure
 */
int main(int argc, char **argv)
{
  generate_sample_data(); // generate M sample data
  system_init();
  new_system();
  gaussian_solver(B_gaussian, c_new_gaussian, d_gaussian, N);
  QR_solver(B_QR, c_new_QR, d_QR, N, N);
  QR_solver(A, c, y, M, N);
  
  print_result();



  free_all_memory();
  //printf("%f",Px(2));
  //fprintf(stderr,Px(1));
  getchar();
  return 0;
}


