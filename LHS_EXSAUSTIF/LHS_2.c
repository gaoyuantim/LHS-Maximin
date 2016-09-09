/******************************************************************************
*                                                                            *
* Copyright Notice                                                           *
*                                                                            *
*    Copyright (C) 2016 Yuan Gao <gaoyuantim@gmail.com>                      *
*                                                                            *
*    Author: Yuan Gao <gaoyuantim@gmail.com>                                 *
*                                                                            *
* Copying Permission Statement                                               *
*                                                                            *
*    This program is free software; you can redistribute it and/or modify it *
*    under the terms of the  GNU Lesser General Public License  as published *
*    by the Free Software Foundation;  either version 2.1 of the License, or *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program  is distributed  in the hope  that it will be useful,  but *
*    WITHOUT ANY WARRANTY;  without even  the implied warranty of MERCHANTA- *
*    BILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General *
*    Public License for more details.                                        *
*                                                                            *
*    You  should have  received  a copy of  the GNU  Lesser  General  Public *
*    License along with this program;  if not, see                           *
*    <http://www.gnu.org/licenses/>.                                         *
*                                                                            *
******************************************************************************/
#include "mex.h"
#include <stdio.h>
#include <stdlib.h>


int MIN(int a, int b){
	if (a>b) return b;
	else return a;
}

void Swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}
int Square(int a){
	return a*a;
}

void Copy(int *a, int *b, int length){
	int i;
	for (i = 0; i < length; i++){
		a[i] = b[i];
	}
}

void LHS_Start(int m, int n, double *D2_maximin, double *coord_fix);
int Colone_Change(int dimension, int position, int m, int n, int *coord, int *coord_maximin, int *Delta2_pairs, int *D2_pairs, double D2_maximin);
int Ligne_Change(int dimension, int m, int n, int *coord_fix, int *D2_pairs, double D2_maximin);
void Caculation(int p_1, int p_2, int m, int n, int *Delta2_pairs, int *D2_pairs);

/*Interation part of Matlab and C. The dimension m and points n are input from matlab and the calculation result will be returned back to Matlab*/
void mexFunction(int nlhs, mxArray *plhs[], int nrhs,
        const mxArray *prhs[]){
    
    int m, n;
    double *D2_maximin;

    if (nrhs != 2){
        mexErrMsgTxt("We need two caracters!");
    }
    n = (int)mxGetScalar(prhs[0]);
    m = (int)mxGetScalar(prhs[1]);
    plhs[0] = mxCreateDoubleMatrix(n,m,mxREAL);
    plhs[1] = mxCreateDoubleMatrix(1,1,mxREAL);
    
    double *Table_max;
    D2_maximin = mxGetPr(plhs[1]);
    Table_max = mxGetPr(plhs[0]);
    
    LHS_Start(m, n, D2_maximin, Table_max);
}

void LHS_Start(int m, int n, double *D2_result, double *Table_max){
	int i, j;
	int *D2_pairs, *coord_fix;
    
	/*Initialisaiton of coordinates and distance2*/
	coord_fix = (int *)malloc(m*n*sizeof(int));
    D2_pairs = (int *)malloc(n*n*sizeof(int));
	for (i = 0; i < n*n; i++){
		D2_pairs[i] = 0;
	}

	*D2_result = (double)Ligne_Change(1, m, n, coord_fix, D2_pairs, 0);
    
    for (i = 0; i < m*n; i++){
        Table_max[i] = (double)coord_fix[i];
    }
    
	/* Free pointer*/
    free(coord_fix);
    coord_fix = NULL;
	free(D2_pairs);
	D2_pairs = NULL;
}



void Caculation(int p_1, int p_2, int m, int n, int *Delta2_pairs, int *D2_pairs){
	int i;

	/*Change distance2 between the points in the same dimension*/
	int *t1, *t2;
	int p_1n = p_1 * n;
	int p_2n = p_2 * n;
	for (i = 0; i<n; i++){
		if ((i != p_1) && (i != p_2)){
			t1 = Delta2_pairs + p_1n + i;
			t2 = Delta2_pairs + p_2n + i;
			Swap(t1, t2);
			Delta2_pairs[i*n + p_1] = *t1;
			Delta2_pairs[i*n + p_2] = *t2;
		}
	}

	/*Calculation D2*/
	int in;
	for (i = 0; i < n; i++){
		in = i * n;
		t1 = Delta2_pairs + p_1n + i;
		t2 = Delta2_pairs + p_2n + i;
		if ((i != p_1) && (i != p_2)){
			D2_pairs[in + p_1] = D2_pairs[in + p_1] + *t1 - *t2;
			D2_pairs[in + p_2] = D2_pairs[in + p_2] + *t2 - *t1;
			D2_pairs[p_1n + i] = D2_pairs[in + p_1] ;
			D2_pairs[p_2n + i] = D2_pairs[in + p_2];
		}
	}
}

int Colone_Change(int dimension, int position, int m, int n, int *coord, int *coord_maximin, int *Delta2_pairs_fix, int *D2_pairs_fix, double D2_maximin){

	int p_1 = position - 1;
	int p_2, i, j, D2_min, *Delta2_pairs, *D2_pairs;
	int *t1, *t2;

	Delta2_pairs = (int *)malloc(n*n*sizeof(int));
	D2_pairs = (int *)malloc(n*n*sizeof(int));

	/*Iteration of every point in each dimension*/
	if (position != n){
		for (p_2 = p_1; p_2 < n; p_2++){

			/*Exchange the coordinates*/
			t1 = coord + p_1;
			t2 = coord + p_2;
			Swap(t1, t2);

			/*Copyback the Distance2, D2*/
			Copy(Delta2_pairs, Delta2_pairs_fix, n*n);
			Copy(D2_pairs, D2_pairs_fix, n*n);

			/*Update Distance2, D2*/
			if (p_1 != p_2){
				Caculation(p_1, p_2, m, n, Delta2_pairs, D2_pairs);
			}

			if (dimension < m){
				/*The maximin from the dimension behind*/
				D2_min = Ligne_Change(dimension + 1, m, n, coord + n, D2_pairs, D2_maximin);

				if (D2_min > D2_maximin){
					D2_maximin = D2_min;
					Copy(coord_maximin, coord, n*(m - dimension + 1));
				}
			}
			else {
				/*Caculation of D2_min in the last dimension*/
				D2_min = n*n*m;
				for (i = 0; i < n; i++){
					for (j = i + 1; j < n; j++){
						D2_min = MIN(D2_min, D2_pairs[i*n + j]);
					}
				}
				if (D2_min > D2_maximin){
					D2_maximin = D2_min;
					Copy(coord_maximin, coord, n);
				}
			}

			/*Update D2_maximin when D2_maximin from dimension behind or in the last dimension*/

			D2_maximin = Colone_Change(dimension, position + 1, m, n, coord, coord_maximin, Delta2_pairs, D2_pairs, D2_maximin);
			
			Swap(t1, t2);
		}
	}
	
	/*Clear Pointer*/
	free(D2_pairs);
	D2_pairs = NULL;
	free(Delta2_pairs);
	Delta2_pairs = NULL;

	return D2_maximin;
}

int Ligne_Change(int dimension, int m, int n, int *coord_fix, int *D2_pairs_fix, double D2_maximin){

	int i, j, *coord, *coord_maximin, *D2_pairs;

	D2_pairs = (int *)malloc(n*n*sizeof(int));
	Copy(D2_pairs, D2_pairs_fix, n*n);

	/*Initilisation of the coordinates in this dimension*/
	coord = (int *)malloc(n*(m - dimension + 1)*sizeof(int));
	coord_maximin = (int *)malloc(n*(m - dimension + 1)*sizeof(int));
	for (i = 0; i < n; i++){
		coord[i] = i;
		coord_maximin[i] = i;
	}

	/*Initialisation of Delta2_pairs in this dimension*/
	int *Delta2_pairs;
	Delta2_pairs  = (int *)malloc(n*n*sizeof(int));
	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){
			Delta2_pairs[i*n + j] = Square(coord[i] - coord[j]);
			D2_pairs[i*n + j] += Delta2_pairs[i*n + j];
		}
	}

	if (dimension == 1){
		for (i = 0; i < n; i++){
			coord_fix[i] = i;
		}
		/*Clear pointer*/
		free(Delta2_pairs);
		Delta2_pairs = NULL;

		return (Ligne_Change(dimension + 1, m, n, coord_fix + n, D2_pairs, D2_maximin));
	}

	/*Go to the colone part*/
	D2_maximin = Colone_Change(dimension, 1, m, n, coord, coord_maximin, Delta2_pairs, D2_pairs, D2_maximin);
	Copy(coord_fix, coord_maximin, n*(m - dimension + 1));

	/*Clear pointer*/
	free(coord);
	coord = NULL;
	free(D2_pairs);
	D2_pairs = NULL;
	free(Delta2_pairs);
	Delta2_pairs = NULL;
	free(coord_maximin);
	coord_maximin = NULL;

	return D2_maximin;
}
