#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



/*----------------------------------------------------------------
 * Procedure to solve a lower triangular system
 *    L*x = b.
 */
void gaussian_back_substitute(double **U, double *x, double *b, int n)
{
	int   i,  j;

	for(i=n-1;i>=0;i--){
		x[i] = b[i]/U[i][i];
		for(j=i-1;j>=0;j--)
			b[j] = b[j] - U[j][i]*x[i];
	}
}	   


/**************************************************************
 * Perform forward elimination for a linear system Ax=b.
 * Partial pivoting is adopted.
 */
void gaussian_elimination_NxN(double **A, double *b, int n)
{
	int     p, i, j, k;
	double  maxEntry, t, r;

	for(i=0;i<n-1;i++){
		// Partial pivoting
		maxEntry = fabs(A[i][i]);
		p = i;
		for(k=i;k<n;k++)
			if(fabs(A[k][i])>maxEntry){
				p = k;
				maxEntry = fabs(A[k][i]);
			}
		if(p!=i){
			for(j=i;j<n;j++){
				t = A[p][j];
				A[p][j] = A[i][j];
				A[i][j] = t;
			}
			t = b[p];
			b[p] = b[i];
			b[i] = t;
		}
		//Forward elimination.
        for(k=i+1;k<n;k++){
			if(A[k][i]==0.0) continue;
			
			r = A[k][i]/A[i][i];
			for(j=i;j<n;j++)
				A[k][j] = A[k][j] - r*A[i][j];
			b[k] = b[k] - r*b[i];
		}
	}
}

			

/**************************************************************
 * Perform forward elimination for a linear system Ax=b.
 * Partial pivoting is adopted.
 * A is a m*n matrix, b is a vector of m*1 
 * m > n ,which means it is a over-constraint system
 */
void gaussian_elimination_MxN(double **A, double *b, int m, int n)
{
	int     p, i, j, k;
	double  maxEntry, t, r;

	for(i=0;i<n-1;i++){
		// Partial pivoting
		maxEntry = fabs(A[i][i]);
		p = i;
		for(k=i;k<m;k++)
			if(fabs(A[k][i])>maxEntry){
				p = k;
				maxEntry = fabs(A[k][i]);
			}
		if(p!=i){
			for(j=i;j<n;j++){
				t = A[p][j];
				A[p][j] = A[i][j];
				A[i][j] = t;
			}
			t = b[p];
			b[p] = b[i];
			b[i] = t;
		}
		//Forward elimination.
        for(k=i+1;k<m;k++){
			if(A[k][i]==0.0) continue;
			
			r = A[k][i]/A[i][i];
			for(j=i;j<n;j++) A[k][j] = A[k][j] - r*A[i][j];
			b[k] = b[k] - r*b[i];
		}
	}
}



/*---------------------------------------------------
 * Procedure to solve a linear system by using QR-
 * decompsotion.
 * Algm:
 *    1. convert Ax=b into an upper triangular system.
 *    2. Solve the upper trianglular system by using 
 *       backward substitution.
 */
void gaussian_solver(double **A, double *x, double *b, int n)
{
	gaussian_elimination_NxN(A, b, n);
    // Solve the upper triangular system by using backward
	// substitution.
    gaussian_back_substitute(A, x, b, n);
}






