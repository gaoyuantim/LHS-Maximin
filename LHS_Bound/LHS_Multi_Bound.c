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
#include <stdbool.h>
#include "mex.h"


void Coord_Occupation(int x, int y, int n, int length, int distance, bool *coord, char *coord_Change);
bool Calculation(int number, int n, int length, int distance, bool *coord_fix, char *coord_Change);
void D2_Square_Arrange(int *sum_Square);
bool Possibility_Count(int n, int length, int distance, bool *coord_fix, char *coord_Change);
int LHS_Start(int n, int D2, double *D2_final);

/*the distance between points will not bigger than 10*10 + 10*10*/
static int Bound_d = 10;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    
    int n, D2;
    
    if (nrhs != 2){
        mexErrMsgTxt("we need two caracters!");
    }
    
    n = (int)mxGetScalar(prhs[0]);
    D2 = (int)mxGetScalar(prhs[1]);
    plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
    
    double *D2_final;
    D2_final = mxGetPr(plhs[0]);
    
    LHS_Start(n, D2, D2_final);

}

int LHS_Start(int n, int D2, double *D2_final){
	
    int i, num_D2;

    /*calculate the sum of square numbers from 1 to 10*/
	int *sum_Square;
    sum_Square = (int *)malloc(Bound_d * Bound_d * sizeof(int));
	D2_Square_Arrange(sum_Square);
	for (num_D2 = 0; D2 > sum_Square[num_D2]; num_D2++);

	if (D2 != sum_Square[num_D2]){
		printf("D2 wrong");
		return 0;
	}

	/*try 2 * d colones*/
	int length;
	length = (int)ceil(sqrt((float)D2)) * 2;

	/*values to record the position has been taken or not*/
	bool *coord;
	coord = (bool *)malloc(length*n*sizeof(bool));

	/*the area will be occupied*/
	int Distance = (int)ceil(sqrt((float)D2));
	char *coord_Change;
	coord_Change = (char *)malloc((Distance + 3)*sizeof(char));
	for (i = 1; i < Distance; i++){
		coord_Change[i] = (char)ceil(sqrt((float)(D2 - i*i))) - 1;
	}

	do {
		num_D2++;
		D2 = sum_Square[num_D2];
		/*printf("\n%d\n", sum_Square[num_D2]);*/

		Distance = (int)ceil(sqrt((float)D2));

		if (Distance > length / 2){

			length = (int)ceil(sqrt((float)D2)) * 2;

			free(coord);
			coord = NULL;
			coord = (bool *)malloc(length*n*sizeof(bool));
		}

		for (i = 1; i < Distance; i++){
			coord_Change[i] = (char)ceil(sqrt((float)(D2 - i*i))) - 1;
		}
	} while (Possibility_Count(n, length, Distance, coord, coord_Change));

	*D2_final = (double)sum_Square[num_D2 - 1];


	/*if the distance changes a lot, we also need to update the check area*/
	

	free(coord);
	coord = NULL;
	free(coord_Change);
	coord_Change = NULL;
    free(sum_Square);
    sum_Square = NULL;


	return 0;
}

/*output : sum_Square*/
/*in this part we calculate the range of distances we are going to study*/
void D2_Square_Arrange(int *sum_Square){
	int i, j, k, m, sum;

	int Bound_D2 = Bound_d * Bound_d;
	for (i = 0; i < Bound_D2; i++){
		sum_Square[i] = 0;
	}
	for (i = Bound_d; i >= 1; i--){
		for (j = Bound_d; j >= 1; j--){
			sum = i*i + j*j;
			k = Bound_D2 - 1;
			while (sum < sum_Square[k]){
				k--;
			}
			for (m = 0; m < k; m++){
				sum_Square[m] = sum_Square[m + 1];
			}
			sum_Square[m] = sum;
		}
	}

	for (k = Bound_D2 - 1, i = 0; i < k;){
		if (sum_Square[i] == sum_Square[i + 1]){
			for (j = i + 1; j < k; j++){
				sum_Square[j] = sum_Square[j + 1];
			}
			sum_Square[k] = 0;
			k--;
		}
		else
			i++;
	}
}

/*Output : *coord*/
/*update the new area of points where can't be chosen again*/
void Coord_Occupation(int x, int y, int n, int length, int distance, bool *coord, char *coord_Change){
	int i, j;

	for (i = 1; i < length - x; i++){

		coord[i*n + y] = false;
	}

	for (i = 1; i < distance; i++){
		for (j = 0; (j <= coord_Change[i]) && ((x + i) < length); j++){

			if ((y + j) < n)
				coord[n * i + y + j] = false;
			if ((y - j) > 0)
				coord[n * i + y - j] = false;

		}
	}
}

/*Output : whether we can find the number of different situations we need with a decided distance*/
bool Possibility_Count(int n, int length, int distance, bool *coord, char *coord_Change){

	int i, j;
	int Possibility = 0;
	bool result = true;

	for (i = 0; i < n; i++){
		for (j = 0; j < n*length; j++){
			coord[j] = true;
		}

		Coord_Occupation(0, i, n, length, distance, coord, coord_Change);

		if (Calculation(1, n, length, distance, coord + n, coord_Change) == false){

			Possibility++;
			if (Possibility == length){
				result = false;
				return result;
			}
		}
	}

	return result;
}

/*Output : whether we can find a available position in the colone we decide*/
bool Calculation(int number, int n, int length, int distance, bool *coord_fix, char *coord_Change){

	int i, k;
	bool result = false;

	if (number == length)
		result = true;

	else
	{
		bool *coord;
		coord = (bool *)malloc((length - number)*n*sizeof(bool));
		for (k = 0; k < n*(length - number); k++){
			coord[k] = true;
		}

		for (i = 0; i < n; i++){

			if (coord_fix[i]){

				for (k = 0; k < n*(length - number); k++){
					coord[k] = coord_fix[k];
				}

				Coord_Occupation(number, i, n, length, distance, coord, coord_Change);
				if (Calculation(number + 1, n, length, distance, coord + n, coord_Change)){

					result = true;
					break;
				}
			}
		}
		free(coord);
		coord = NULL;
	}

	/*Fail find coordinate*/
	return result;
}

