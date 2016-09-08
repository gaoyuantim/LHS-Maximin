/******************************************************************************
*                                                                            *
* Copyright Notice                                                           *
*                                                                            *
*    Copyright (C) 2016  Yuan Gao <gaoyuantim@gmail.com>                                      *
*                                                                            *
*    Author: Yuan Gao <gaoyuantim@gmail.com>                                  *
*                                                                            *
* Copying Permission Statement                                               *
*                                                                            *
*    This program is free software; you can redistribute it and/or modify it *
*    under the terms of the  GNU Lesser General Public License  as published *
*    by the Free Software Foundation;  either version 3.0 of the License, or *
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mex.h"

bool calculation_Dimension(int n, int m, int dimension, int num_Type, int **type_Ligne, double *num_Result, int **Distance2, int *table_Fix);
bool calculation_Colone(int num, int n, int m, int dimension, int num_Type, int *table, bool *table_Check, int **type_Ligne, int **type_Ligne_Backup, double *num_Result, int **Distance2, int *table_Fix);
bool calculation_Distance2_Update(int i, int num, int n, int dimension, int *table, int **type_Ligne, double *num_Result, int **Distance2);
bool type_Check(int n, int ligne, int dimension, int **type_Ligne, double *num_Result, int **Distance2);
int LHS_Start(int n, int dimension, int num_Type, double *num_Result, double *result);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    
    int n, k, num_Type;
    double *num_Result;
    
    n = (int)mxGetScalar(prhs[0]);
    k = (int)mxGetScalar(prhs[1]);
    num_Type = (int)mxGetScalar(prhs[2]);
    num_Result = mxGetPr(prhs[3]);
    
    plhs[0] = mxCreateDoubleMatrix(n,k,mxREAL);
    
    double *Table_fix;
    Table_fix = mxGetPr(plhs[0]);
    
    LHS_Start(n, k, num_Type, num_Result, Table_fix);

}

int ABS(int a){
	if (a < 0)
		return (-a);
	else
		return a;
}

int LHS_Start(int n, int dimension, int num_Type, double *num_Result, double *result){
	int i, j;

	/*For example: Distance2[i][j] = k
					i means distance, j means which combinaison of points, k means hoz many times.
	  In fact we have  n*(n-1)/2 combinaisons, but we construct n*(n-1)to make it easier for calculation*/
	int **Distance2;
	Distance2 = (int **)malloc(n * sizeof(int *));
	for (i = 0; i < n; i++){
		Distance2[i] = (int *)malloc(n * (n - 1) * sizeof(int));
		for (j = 0; j < n * (n - 1); j++){
			Distance2[i][j] = 0;
		}
	}

	/*We suppose we have several types of combinaisons and we use type_Ligne to note them.
	  For example: type_Ligne[i][0] = p
					i means which combinaison of points, the first value p notes how many kinds of possible types remained
				   type_Ligne[i][j] = q
					i means which combinaison of points, j is a pointer and q means which type of combinaison
	  For another example:
				   we suppose that type_Ligne[i] = {3 3 5 7 0 0 0 0 0} and type_Ligne[i][0] means there is still 3 possible types and the other not zero numbers mean which type*/
	int **type_Ligne;
	type_Ligne = (int **)malloc(n*(n - 1) * sizeof(int *));
	for (j = 0; j < n*(n - 1); j++){

		type_Ligne[j] = (int *)malloc((num_Type + 1) * sizeof(int));
		type_Ligne[j][0] = num_Type;
		for (i = 1; i <= num_Type; i++){
			type_Ligne[j][i] = i;
		}
	}

	/*Initialisation for 1st dimension*/
	int *table;
    int *table_Fix;
	table = (int *)malloc(n * sizeof(int));
    table_Fix = (int *)malloc(n * dimension * sizeof(int));
	for (i = 0; i < n; i++){
		table[i] = i;
		table_Fix[i] = i;
	}

	int difference, ligne;
	for (i = 0; i < n; i++){
		for (j = i + 1; j < n; j++){

			difference = ABS(table[i] - table[j]);
			ligne = i * n + j;

			Distance2[difference][ligne]++;

			type_Check(n, ligne, dimension, type_Ligne, num_Result, Distance2);

		}
	}


	/*Start from second dimension*/
	if (calculation_Dimension(n, 1, dimension, num_Type, type_Ligne, num_Result, Distance2, table_Fix + n)){
        
       for (i = 0; i < dimension; i++){
			for (j = 0; j < n; j++){
                result[i*n + j] = (double)table_Fix[i*n + j];
            }
       }
    }
    
	for (i = 0; i < n; i++){
        free(Distance2[i]);
        Distance2[i] = NULL;
    }
    free(Distance2);
    Distance2 = NULL;
	for (i = 0; i < n*(n - 1); i++){
		free(type_Ligne[i]);
        type_Ligne[i] = NULL;
   	}
	free(type_Ligne);
    type_Ligne = NULL;
	free(table);
    table = NULL;
    free(table_Fix);
    table_Fix = NULL;
    
    return 0;
}

/*Initialisation for a new dimension we are going to study*/
bool calculation_Dimension(int n, int m, int dimension, int num_Type, int **type_Ligne, double *num_Result, int **Distance2, int *table_Fix){

	bool result;
	
	if ( m == dimension )
		
		result = true;
	
	else{
		
		int i, j;

		int *table;
		bool *table_Check;
		int **type_Ligne_Backup;
		table = (int *)malloc(n * sizeof(int));
		table_Check = (bool *)malloc(n * sizeof(bool));

		for (i = 0; i < n; i++){
			table_Check[i] = true;
		}

		/*Create a backup of type_Ligne for each colone*/
		type_Ligne_Backup = (int **)malloc(n * (n - 1) * sizeof(int *));
		for (i = 0; i < n * (n - 1); i++){
			
			type_Ligne_Backup[i] = (int *)malloc((num_Type + 1) *sizeof(int));

			for (j = 0; j <= num_Type; j++){

				type_Ligne_Backup[i][j] = type_Ligne[i][j];
			}
		}

		if (calculation_Colone(0, n, m, dimension, num_Type, table, table_Check, type_Ligne, type_Ligne_Backup, num_Result, Distance2, table_Fix)){
			
			for (i = 0; i < n; i++){
				table_Fix[i] = table[i];
			}
			
			result = true;
		}
		else{
			result = false;

		}
		
		for (i = 0; i < n * (n - 1); i++){
			free(type_Ligne_Backup[i]);
            type_Ligne_Backup[i] = NULL;
		}
		free(type_Ligne_Backup);
        type_Ligne_Backup = NULL;
		free(table);
        table = NULL;
		free(table_Check);
        table_Check = NULL;
    }

	return result;
}

bool calculation_Colone(int num, int n, int m, int dimension, int num_Type, int *table, bool *table_Check, int **type_Ligne, int **type_Ligne_Backup, double *num_Result, int **Distance2, int *table_Fix){

	bool result;

	if (num == n)
		
		result = calculation_Dimension(n, m + 1, dimension, num_Type, type_Ligne, num_Result, Distance2, table_Fix + n);

	else{

		int i, j, k;
		int difference, ligne;
		int Possibility;

		for (i = 0; i < n; i++){

			if (table_Check[i]){
				table[num] = i;
				table_Check[i] = false;

				if (calculation_Distance2_Update(0, num, n, dimension, table, type_Ligne, num_Result, Distance2)){

					if (calculation_Colone(num + 1, n, m, dimension, num_Type, table, table_Check, type_Ligne, type_Ligne_Backup, num_Result, Distance2, table_Fix)){

						result = true;

						return result;
					}
					else{

						for (k = 0; k < num; k++){

							ligne = k * n + num;

							Possibility = type_Ligne_Backup[ligne][0];
							
							for (j = 0; j <= Possibility; j++){
								type_Ligne[ligne][j] = type_Ligne_Backup[ligne][j];
							}
						}

						for (j = 0; j < num; j++){

							difference = ABS(table[num] - table[j]);

							ligne = j * n + num;

							Distance2[difference][ligne]--;
						}

						table_Check[i] = true;
					}
				}

				else{

					for (k = 0; k < num; k++){

						ligne = k * n + num;

						Possibility = type_Ligne_Backup[ligne][0];
						
						for (j = 0; j <= Possibility; j++){
							type_Ligne[ligne][j] = type_Ligne_Backup[ligne][j];
						}
					}

					table_Check[i] = true;
				}/*else*/
			}
		}/*for*/
		result = false;
	}

	return result;
}

bool calculation_Distance2_Update(int i, int num, int n, int dimension, int *table, int **type_Ligne, double *num_Result, int **Distance2){

	bool result;

	if (i == num)
		result = true;

	else{
		
		int difference, ligne;
		
		difference = ABS(table[num] - table[i]);

		ligne = i * n + num;

		Distance2[difference][ligne]++;

		if (type_Check(n, ligne, dimension, type_Ligne, num_Result, Distance2)){

			if (calculation_Distance2_Update(i + 1, num, n, dimension, table, type_Ligne, num_Result, Distance2)){
				
				result = true;
			}
			else{
				
				Distance2[difference][ligne]--;
				result = false;
			}
		}

		else{
			Distance2[difference][ligne]--;
			result = false;
		}
	}
	
	return result;
}

/*Check possible types and update*/
bool type_Check(int n, int ligne, int dimension, int **type_Ligne, double *num_Result, int **Distance2){
	int i, j;

	int Possibility = type_Ligne[ligne][0];
	int new_Possibility = 0;
	int type;

	for (i = 1; i <= Possibility; i++){
		type = type_Ligne[ligne][i];

		for (j = 1; j < n; j++){
			if (Distance2[j][ligne] > num_Result[type * (n - 1) + j - 1]){
				break;
			}
		}
		if (j == n){
			
			new_Possibility++;
			type_Ligne[ligne][new_Possibility] = type;

		}
	}
	if (new_Possibility > 0){
		type_Ligne[ligne][0] = new_Possibility;
		return true;
	}
	else
		return false;
}
