#include "mex.h"
#include <stdio.h>
#include <stdlib.h>

int MIN(int a, int b){
	if (a>b) return b;
	else return a;
}

int FABS(int a){
	if (a>0) return a;
	else return -a;
}

void Swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

/*Change the distance between every point*/
void Caculation (int dimension, int p_1, int p_2, int m, int n, int *coord,
                 int **delta2_pairs, int *D2_pairs, int *D2_points,
                 double *D2_maximin, double *Table_max)
{
  int i, j, k;

	/*Change distance2 between the points in one dimension*/
	int *t1, *t2;
	for (i = 0; i<n; i++){
		if ((i != p_1) && (i != p_2)){
			t1 = &(delta2_pairs[dimension*n + p_1][i]);
			t2 = &(delta2_pairs[dimension*n + p_2][i]);
			Swap(t1, t2);
			delta2_pairs[dimension*n + i][p_1] = *t1;
			delta2_pairs[dimension*n + i][p_2] = *t2;
		}
	}

	/*Calculation D2*/
	int in, D2_min;
	int p_1n = p_1 * n;
	int p_2n = p_2 * n;
	for (i = 0; i < n; i++){
		in = i * n;
		t1 = &(delta2_pairs[dimension*n + p_1][i]);
		t2 = &(delta2_pairs[dimension*n + p_2][i]);
		if ((i != p_1) && (i != p_2)){
			D2_pairs[in + p_1] = D2_pairs[in + p_1] + *t1 - *t2;
			D2_pairs[in + p_2] = D2_pairs[in + p_2]  + *t2 - *t1;
			D2_pairs[p_1n + i] = D2_pairs[in + p_1];
			D2_pairs[p_2n + i] = D2_pairs[in + p_2];
		}
	}
	D2_min = m*n*n;
	for (i = 0; i < n; i++){
		in = i * n;
		D2_points[i] = n * n * m;
		for (j = 0; j < n; j++){
			if (i != j)
				D2_points[i] = MIN(D2_pairs[in + j], D2_points[i]);
		}
		D2_min = MIN(D2_min, D2_points[i]);
	}

	/*Calculate maximin D2*/
	if (*D2_maximin < D2_min){
		*D2_maximin = D2_min;
		for (j = 0; j<m; j++){
			for (k = 0; k<n; k++){
				Table_max[j*n + k] = coord[j*n + k];
			}
		}
	}
}

void Position_Change(int dimension, int position, int m, int n, int *coord,
        int **delta2_pairs, int *D2_pairs, int *D2_points,
        double *D2_maximin, double *Table_max){

	int i = dimension - 1;
	int j = position - 1;
	int k, *t1, *t2;

	/*Iteration of every point in each dimension*/
	if (position != n){
		for (k = j; k<n; k++){
			t1 = coord + i*n + j;
			t2 = coord + i*n + k;
            Swap(t1, t2);
			Caculation(i, j, k, m, n, coord, delta2_pairs, D2_pairs,
                    D2_points, D2_maximin, Table_max);

			if (dimension < m){
				Position_Change(dimension + 1, 1, m, n, coord,
                        delta2_pairs, D2_pairs, D2_points,
                        D2_maximin, Table_max);
			}
			Position_Change(dimension, position + 1, m, n, coord,
                    delta2_pairs, D2_pairs, D2_points, D2_maximin,
                    Table_max);

			Swap(t1, t2);
			Caculation(i, j, k, m, n, coord, delta2_pairs, D2_pairs,
                    D2_points, D2_maximin, Table_max);
		}
	}
}

void LHS_Start(int m, int n, double *D2_maximin, double *Table_max){
	int i, j, k;
	int *square;
	int *D2_pairs, 
        *D2_points, 
        *coord, 
        **delta2_pairs;

	/*Initialisation of square numbers*/
	square = (int *)malloc(n*sizeof(int));
	for (i = 0; i<n; i++){
		square[i] = i*i;
	}

	/*Initialisaiton of coordinates and distance2*/
	coord = (int *)malloc(m*n*sizeof(int));
	delta2_pairs = (int **)malloc(m*n*sizeof(int *));
	D2_pairs = (int *)malloc(n*n*sizeof(int));
	D2_points = (int *)malloc(n*sizeof(int));
	
	for (i = 0; i < m; i++){
		/*Initialisation of coordinates*/
		for (j = 0; j < n; j++){
			coord[i*n + j] = j;
			Table_max[i*n + j] = j;
		}
		
		/*Initialisation of distance2*/
		for (j = 0; j < n; j++){
			delta2_pairs[i*n + j] = (int *)malloc(n*sizeof(int));
			for (k = 0; k < n; k++){
				delta2_pairs[i*n + j][k] = 
                        square[FABS(coord[i*n + j] - coord[i*n + k])];
			}
		}
	}

	/*Initialisation of D2_pairs, D2_points*/
	for (i = 0; i<n; i++){
		for (j = 0; j<n; j++){
			D2_pairs[i*n + j] = 0;
			for (k = 0; k<m; k++){
				D2_pairs[i*n + j] += delta2_pairs[k*n + i][j];
			}
		}
		D2_points[i] = m*square[n - 1];
		for (j = 1; j<n; j++){
			if (i != j){
				D2_points[i] = MIN(D2_pairs[i*n + j], D2_points[i]);
			}
		}
	}

	*D2_maximin = 0;
    /*First dimension needn't change*/
	Position_Change(2, 1, m, n, coord, delta2_pairs, D2_pairs, D2_points,
            D2_maximin, Table_max);

	/* Free pointer*/
	free(square);
	square = NULL;
	free(D2_pairs);
	D2_pairs = NULL;
	free(D2_points);
	D2_points = NULL;
	free(coord);
	coord= NULL;
	free(delta2_pairs);
	delta2_pairs = NULL;
}


void mexFunction(int nlhs, mxArray *plhs[], int nrhs,
        const mxArray *prhs[]){
    
    int m, n;
    double *D2_maximin;

    if (nrhs != 2){
        mexErrMsgTxt("We need two caracters!");
    }
    m = (int)mxGetScalar(prhs[0]);
    n = (int)mxGetScalar(prhs[1]);
    plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
    plhs[1] = mxCreateDoubleMatrix(n,m,mxREAL);
    
    double *Table_max;
    D2_maximin = mxGetPr(plhs[0]);
    Table_max = mxGetPr(plhs[1]);
    
    LHS_Start(m, n, D2_maximin, Table_max);

}
