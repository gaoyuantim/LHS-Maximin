/******************************************************************************
*                                                                            *
* Copyright Notice                                                           *
*                                                                            *
*    Copyright (C) 2016 CentraleSupelec                                      *
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
#include <stdbool.h>

void Coord_Occupation(int x, int y, int n, bool *coord, char *coord_Change, int Distance);
bool Calculation(int y, int n, bool *coord_fix, char *coord_Change, int Distance, char *Table);
int LHS_Start(int n, int D2, double *Table_Out);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    
    int n, D2;
    
    if (nrhs != 2){
        mexErrMsgTxt("we need two caracters!");
    }
    
    n = (int)mxGetScalar(prhs[0]);
    D2 = (int)mxGetScalar(prhs[1]);
    plhs[0] = mxCreateDoubleMatrix(n,2,mxREAL);
    
    double *Table;
    Table = mxGetPr(plhs[0]);
    
    LHS_Start(n, D2, Table);

}

int LHS_Start(int n, int D2, double *Table_Out){
	
    int i, j;

	int Distance = (int)ceil(sqrt((float)D2));

	/*create a area of n*n to note whether the positions are taken or not*/
	bool *coord;
	coord = (bool *)malloc(n*n*sizeof(bool));
	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){
			coord[i*n + j] = true;
		}
	}

	/*If we take a position, the area can't be taken any more*/
	char *coord_Change;
	coord_Change = (char *)malloc(Distance*sizeof(char));
	for (i = 0; i < Distance; i++){
		coord_Change[i] = (char)ceil(sqrt((float)(D2 - i*i))) - 1;
	}

	/*result*/
	char *Table;
	Table = (char *)malloc(n*sizeof(char));
	for (i = 0; i < n; i++){
		Table[i] = 0;
        Table_Out[i] = i;
        Table_Out[i+n] = 0;
	}

	if (Calculation(0, n, coord, coord_Change, Distance, Table)){
		for (i = 0; i < n; i++){
			Table_Out[i+n] = Table[i];
		}
	}
	else printf("D2 too big");

	free(coord);
	coord = NULL;
	free(coord_Change);
	coord_Change = NULL;
	free(Table);
	Table = NULL;

	return 0;
}

/*if a position is taken, change state of the area concerned*/
void Coord_Occupation(int x, int y, int n, bool *coord, char *coord_Change, int Distance){
	int i, j;
	for (i = 1; i < n - y; i++){
		coord[i*n + x] = false;
	}
	for (i = 1; i < Distance; i++){
		for (j = 1; (j <= coord_Change[i])&&((j + y) < n); j++){
			if ((x + i) < n)
				coord[j*n + x + i] = false;
			if ((x + i) >= 0)
				coord[j*n + x - i] = false;
		}
	}
}

/*try every position posible line by line*/
bool Calculation(int y, int n, bool *coord_fix, char *coord_Change, int Distance, char *Table){
	
	int i, j, k;
	
	if (y < n){
		/*create a new space and don't change the old one*/
		bool *coord;
		coord = (bool *)malloc(n*(n - y)*sizeof(bool));

		for (i = 0; i < n; i++){
			if (coord_fix[i]){
				
				Table[y] = i;

				for (j = 0; j < n - y; j++){
					for (k = 0; k < n; k++){
						coord[j*n + k] = coord_fix[j*n + k];
					}
				}

				Coord_Occupation(i, y, n, coord, coord_Change, Distance);
				if (Calculation(y + 1, n, coord + n, coord_Change, Distance, Table)){
					
					free(coord);
					coord = NULL;
					
					return true;
				}
			}
		}

		free(coord);
		coord = NULL;
	}
	else 
		return true;

	/*Fail find coordinate*/
	return false;
}
