/******************************************************************************
*                                                                            *
* Copyright Notice                                                           *
*                                                                            *
*    Copyright (C) 2016  Yuan Gao <gaoyuantim@gmail.com>                     *
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

int FABS(int a){
	if (a>0) return a;
	else return -a;
}

void Swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

void LHS_Start(int m, int n, double *D2_maximin, double *Table_max);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs,
	const mxArray *prhs[]){

	int m, n;
	double *D2_maximin;

	if (nrhs != 2){
		mexErrMsgTxt("We need two caracters!");
	}
	m = (int)mxGetScalar(prhs[0]);
	n = (int)mxGetScalar(prhs[1]);
	plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
	plhs[1] = mxCreateDoubleMatrix(n, m, mxREAL);

	double *Table_max;
	D2_maximin = mxGetPr(plhs[0]);
	Table_max = mxGetPr(plhs[1]);

	LHS_Start(m, n, D2_maximin, Table_max);
}

/*Compare each distance between every group of two points*/
void Liste_Check(int m, int n, int *coord, int *D2_pairs, 
        double D2_maximin, double *Table_max, int *D2_distribution){
    
    int i, j, k;
    const int Total = n * (n - 1) / 2;
    int addition = 0;
    
    int *Table_comparaison;
    Table_comparaison = (int *)malloc((m*n*n+1)*sizeof(int));
    for (i = 0; i <= n*n*m; i++){
        Table_comparaison[i] = 0;
    }
    
    for (i = 0; i < n; i++){
        for (j = i + 1; j < n; j++){
            Table_comparaison[(D2_pairs[i*n+j])]++;
        }
    }
    
    for (i = D2_maximin; addition < Total; i++){
        if (Table_comparaison[i] < D2_distribution[i]){
            for (j = 0; j < m; j++){
                for (k = 0; k < n; k++){
                    Table_max[j*n + k] = coord[j*n + k];
                }
            }
            for (j = 0; j <= m*n*n; j++){
                D2_distribution[j] = Table_comparaison[j];
            }
            break;
        }
        else if (Table_comparaison[i] > D2_distribution[i]){
            break;
        }
        addition += D2_distribution[i];
    }
    
    free(Table_comparaison);
    Table_comparaison = NULL;
}

/*Change the distance between every point*/
void Caculation (int dimension, int p_1, int p_2, int m, int n, int *coord,
        int **delta2_pairs, int *D2_pairs, int *D2_points, 
        double *D2_maximin, double *Table_max, int *D2_distribution)
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
        
        /*Reinitialisation de D2_distribution*/
        for (i = 0; i < m*n*n; i++){
            D2_distribution[i] = 0;
        }
        
        /*Change D2_distribution*/
        for (i = 0; i < n; i++){
            for (j = i + 1; j < n; j++){
                D2_distribution[(D2_pairs[i*n+j])]++;
            }
        }
    }
    else if (*D2_maximin == D2_min){
        Liste_Check(m, n, coord, D2_pairs, *D2_maximin, Table_max, 
                D2_distribution);
    }
}

void Position_Change(int dimension, int position, int m, int n, int *coord,
        int **delta2_pairs, int *D2_pairs, int *D2_points, 
        double *D2_maximin, double *Table_max,  int *D2_distribution){

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
                    D2_points, D2_maximin, Table_max, D2_distribution);

			if (dimension < m){
				Position_Change(dimension + 1, 1, m, n, coord,
                        delta2_pairs, D2_pairs, D2_points,
                        D2_maximin, Table_max, D2_distribution);
			}
			Position_Change(dimension, position + 1, m, n, coord,
                    delta2_pairs, D2_pairs, D2_points, D2_maximin,
                    Table_max, D2_distribution);

			Swap(t1, t2);
			Caculation(i, j, k, m, n, coord, delta2_pairs, D2_pairs,
                    D2_points, D2_maximin, Table_max, D2_distribution);
		}
	}
}

void LHS_Start(int m, int n, double *D2_maximin, double *Table_max){
	int i, j, k;
	int *square;
	int *D2_pairs, *D2_points, *coord, **delta2_pairs, *D2_distribution;

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
    D2_distribution = (int *)malloc((1 + m*n*n)*sizeof(int));
	
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
    
    /*Initialisation D2_distribution*/
    for (i = 0; i < m*n*n; i++){
        D2_distribution[i] = 0;
    }
    
	*D2_maximin = 0;
    /*First dimension needn't change*/
	Position_Change(2, 1, m, n, coord, delta2_pairs, D2_pairs, D2_points,
            D2_maximin, Table_max, D2_distribution);

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
