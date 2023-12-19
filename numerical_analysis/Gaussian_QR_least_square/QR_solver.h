#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vector_matrix.h"

/*----------------------------------------------------------------
 * Procedure to solve a lower triangular system
 *    L*x = b.
 */
void QR_back_substitute(double **U, double *x, double *b, int n)
{
	int   i,  j;

	for(i=n-1;i>=0;i--){
		x[i] = b[i]/U[i][i];
		for(j=i-1;j>=0;j--)
			b[j] = b[j] - U[j][i]*x[i];
	}
}	   

/*------------------------------------------------------
 * Create a Householder vector for a matrix. This vector
 * is used to create a Householder matrix H such that 
 * H*A makes the entries below A[i][j] of A[][j] be 0.
 * A is a matrix of (m by n)
 */
void QR_create_H_vec(double **A, int i, int j, double *v, int m)
{
	int      k;
    double   norm2;

	for(k=0;k<i;k++) v[k] = 0.0;
	norm2 = 0.0;
	for(k=i;k<m;k++) norm2 += A[k][j]*A[k][j];
	norm2 = sqrt(norm2);

	if(A[i][j]>=0.0) v[i] = A[i][j] + norm2;
	else v[i] = A[i][j] - norm2;

	for(k=i+1;k<m;k++) v[k] = A[k][j]; 
	//fprintf(stderr," Householder vector=\n");
	//for(k=0;k<m;k++) fprintf(stderr," %lf", v[k]);
	fprintf(stderr,"\n");
}


/*------------------------------------------------------
 * Procedure to reflect matrix A into an upper triangular
 * matrix by using a sequence of Householder matrices.
 *    H(n-2)H(n-3)...H(0)Ax = H(n-2)H(n-3)...H(0)b.
 * ---> Rx = b'.
 * A is a matrix of (m by n)
 */
void QR_reflect(double **A, double *b, int m, int n)
{
	int     i, j, k;
	double  *v, vv, vx;

	v = alloc_vec(m);
	//Eliminate each column to make A[][] into U[][].
	for(j=0;j<n-1;j++){
		// Create vector v[] = A.j + ||A.j||*e1.
		QR_create_H_vec(A, j, j, v, m);
        vv = inner_product(v, v, m);
		// Update each column A.k, j<=k<=n-1.
		for(k=j;k<n;k++){
            // Compute vx=<v, A.k>
            vx = 0.0;
			for(i=j;i<m;i++) vx += A[i][k]*v[i];
			// A.k = A.k - 2(<v, A.k>/<v,v>)v ;
            for(i=j;i<m;i++) 
				A[i][k] = A[i][k] - 2.0*(vx/vv)*v[i];
		}
		// Update b. b = b - 2(<v,b>/<v,v>)v ;
		vx = inner_product(v, b, m);
		for(i=j;i<m;i++) 
			b[i] = b[i] - 2.0*(vx/vv)*v[i];
		//fprintf(stderr," A[]=\n");
		//print_mtx(A, n, n);
		//fprintf(stderr," b[]=\n");
		//print_vec(b, n);
	}
	free(v);
}



/*---------------------------------------------------
 * Procedure to solve a linear system by using QR-
 * decompsotion.
 * Algm:
 *    1. convert Ax=b into an upper triangular system.
 *    2. Solve the upper trianglular system by using 
 *       backward substitution.
 */
void QR_solver(double **A, double *x, double *b, int m, int n)
{
	// Reflect A into an upper triangular matrix.
	QR_reflect(A, b, m, n);
    // Solve the upper triangular system by using backward
	// substitution.
    QR_back_substitute(A, x, b, n);
}







