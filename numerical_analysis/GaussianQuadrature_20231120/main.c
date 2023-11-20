#include <stdio.h>
#include <math.h>

#include "gaussianQuadrature.h"
#include "error.h"


/*----------------------------------------------------------------------------------------------------------
 * The main procedure 
 */
void main(int argc, char **argv)
{
  char filename[]="gauss.txt";
  double exactValue, gaussValue, err;
  FILE *fp;


  //Open the output file
  fp = fopen(filename, "w");

  //Compute the exact sum.
  exactValue = 470.3272100636381;
  fprintf(fp,"Exact integration = %15.10f\n", exactValue);

  fprintf(fp,"------1-cell------------------------------------------\n");
  gaussValue = gaussianQuadrature2D(-2.0, -2.0, 8.0, 8.0, 1, 2);
  err = relativeError(gaussValue, exactValue);
  fprintf(fp,"2X2 sample points: %15.10f      %15.2f%%\n",gaussValue, err);
  gaussValue = gaussianQuadrature2D(-2.0, -2.0, 8.0, 8.0, 1, 3);
  err = relativeError(gaussValue, exactValue);
  fprintf(fp,"3X3 sample points: %15.10f      %15.2f%%\n",gaussValue, err);
  gaussValue = gaussianQuadrature2D(-2.0, -2.0, 8.0, 8.0, 1, 4);
  err = relativeError(gaussValue, exactValue);
  fprintf(fp,"4X4 sample points: %15.10f      %15.2f%%\n",gaussValue, err);
  fprintf(fp,"------4-cell------------------------------------------\n");
  gaussValue = gaussianQuadrature2D(-2.0, -2.0, 8.0, 8.0, 2, 2);
  err = relativeError(gaussValue, exactValue);
  fprintf(fp,"2X2 sample points: %15.10f      %15.2f%%\n",gaussValue, err);
  gaussValue = gaussianQuadrature2D(-2.0, -2.0, 8.0, 8.0, 2, 3);
  err = relativeError(gaussValue, exactValue);
  fprintf(fp,"3X3 sample points: %15.10f      %15.2f%%\n",gaussValue, err);
  gaussValue = gaussianQuadrature2D(-2.0, -2.0, 8.0, 8.0, 2, 4);
  err = relativeError(gaussValue, exactValue);
  fprintf(fp,"4X4 sample points: %15.10f      %15.2f%%\n",gaussValue, err);
  fprintf(fp,"------16-cell------------------------------------------\n");
  gaussValue = gaussianQuadrature2D(-2.0, -2.0, 8.0, 8.0, 4, 2);
  err = relativeError(gaussValue, exactValue);
  fprintf(fp,"2X2 sample points: %15.10f      %15.2f%%\n",gaussValue, err);
  gaussValue = gaussianQuadrature2D(-2.0, -2.0, 8.0, 8.0, 4, 3);
  err = relativeError(gaussValue, exactValue);
  fprintf(fp,"3X3 sample points: %15.10f      %15.2f%%\n",gaussValue, err);
  gaussValue = gaussianQuadrature2D(-2.0, -2.0, 8.0, 8.0, 4, 4);
  err = relativeError(gaussValue, exactValue);
  fprintf(fp,"4X4 sample points: %15.10f      %15.2f%%\n",gaussValue, err);
  fprintf(fp,"------64-cell------------------------------------------\n");
  gaussValue = gaussianQuadrature2D(-2.0, -2.0, 8.0, 8.0, 8, 2);
  err = relativeError(gaussValue, exactValue);
  fprintf(fp,"2X2 sample points: %15.10f      %15.2f%%\n",gaussValue, err);
  gaussValue = gaussianQuadrature2D(-2.0, -2.0, 8.0, 8.0, 8, 3);
  err = relativeError(gaussValue, exactValue);
  fprintf(fp,"3X3 sample points: %15.10f      %15.2f%%\n",gaussValue, err);
  gaussValue = gaussianQuadrature2D(-2.0, -2.0, 8.0, 8.0, 8, 4);
  err = relativeError(gaussValue, exactValue);
  fprintf(fp,"4X4 sample points: %15.10f      %15.2f%%\n",gaussValue, err);
  fprintf(fp,"------256-cell------------------------------------------\n");
  gaussValue = gaussianQuadrature2D(-2.0, -2.0, 8.0, 8.0, 16, 2);
  err = relativeError(gaussValue, exactValue);
  fprintf(fp,"2X2 sample points: %15.10f      %15.2f%%\n",gaussValue, err);
  gaussValue = gaussianQuadrature2D(-2.0, -2.0, 8.0, 8.0, 16, 3);
  err = relativeError(gaussValue, exactValue);
  fprintf(fp,"3X3 sample points: %15.10f      %15.2f%%\n",gaussValue, err);
  gaussValue = gaussianQuadrature2D(-2.0, -2.0, 8.0, 8.0, 16, 4);
  err = relativeError(gaussValue, exactValue);
  fprintf(fp,"4X4 sample points: %15.10f      %15.2f%%\n",gaussValue, err);

  fclose(fp);
}//end of main()
  






