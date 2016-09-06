#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mex.h"

void Coord_Occupation(int x, int y, int z, int n, bool ***coord, char **coord_Change, int Distance);
bool Calculation(int z, int n, bool ***coord_fix, char **coord_Change, int Distance, char *Table);
int LHS_Start(int n, int D2, double *Table_Out);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    
    int n, D2;
    
    if (nrhs != 2){
        mexErrMsgTxt("we need two caracters!");
    }
    
    n = (int)mxGetScalar(prhs[0]);
    D2 = (int)mxGetScalar(prhs[1]);
    plhs[0] = mxCreateDoubleMatrix(n, 3, mxREAL);
    
    double *Table;
    Table = mxGetPr(plhs[0]);
    
    LHS_Start(n, D2, Table);

}

int LHS_Start(int n, int D2, double *Table_Out){
	
	int i, j, k;

	int Distance = (int)ceil(sqrt((float)D2));

	/*create a area of n*n to note whether the positions are taken or not*/
	/*coord[z][x][y]*/
	bool ***coord;
	coord = (bool ***)malloc(n*sizeof(bool**));
	for (i = 0; i < n; i++){
		coord[i] = (bool **)malloc(n*sizeof(bool*));
		for (j = 0; j < n; j++){
			coord[i][j] = (bool *)malloc(n*sizeof(bool));
			for (k = 0; k < n; k++){
				coord[i][j][k] = true;
			}
		}
	}

	/*If we take a position, the area can't be taken any more*/
	/*coord_Change[x][y] = z*/
	char **coord_Change;
	coord_Change = (char **)malloc(Distance*sizeof(char *));
	for (i = 0; i < Distance; i++){
		coord_Change[i] = (char *)malloc(Distance*sizeof(char));
		for (j = 0; j < Distance; j++){
			coord_Change[i][j] = (char)ceil(sqrt((float)(D2 - i*i - j*j))) - 1;
		}
	}

	/*Result*/
	/*Table[z] = x  y*/
	char *Table;
	Table = (char *)malloc(n*2*sizeof(char));
	for (i = 0; i < n; i++){
		for (j = 0; j < 2; j++){
			Table[i*2 + j] = i;
		}
	}

	if (Calculation(0, n, coord, coord_Change, Distance, Table)){
		for (i = 0; i < n; i++){
			Table_Out[i] = i;
            Table_Out[i + n] = Table[i*2];
            Table_Out[i + n*2] = Table[i*2 + 1];
		}
	}
	else printf("D2 too big");

	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){
			free(coord[i][j]);
			coord[i][j] = NULL;
		}
	}
	for (i = 0; i < n; i++){
		free(coord[i]);
		coord[i] = NULL;
	}
	free(coord);
	coord = NULL;
	
	for (i = 0; i < Distance; i++){
		free(coord_Change[i]);
		coord_Change[i] = NULL;
	}
	free(coord_Change);
	coord_Change = NULL;

	free(Table);
	Table = NULL;

	return 0;
}

/*if a position is taken, change state of the area concerned*/
void Coord_Occupation(int x, int y, int z, int n, bool ***coord, char **coord_Change, int Distance){
	int i, j, k;

	for (i = 1; i < n - z; i++){
		for (j = 0; j < n; j++){
			coord[i][j][y] = false;
			coord[i][x][j] = false;
		}
	}

	for (i = 1; i < Distance; i++){
		for (j = 1; j < Distance; j++){
			for (k = 1; (k <= coord_Change[i][j]) && ((k + z) < n); k++){

				if ((x + i) < n){
					if ((y + j) < n)
						coord[k][x + i][y + j] = false;
					if ((y - j) >= 0)
						coord[k][x + i][y - j] = false;
				}

				if ((x - i) >= 0){
					if ((y + j) < n)
						coord[k][x - i][y + j] = false;
					if ((y - j) >= 0)
						coord[k][x - i][y - j] = false;
				}
			}
		}
	}
}

/*try every position posible in each surface*/
bool Calculation(int z, int n, bool ***coord_fix, char **coord_Change, int Distance, char *Table){

	int i, j, k;
	int x, y;

	if (z < n){
		bool ***coord;
		coord = (bool ***)malloc((n - z)*sizeof(bool **));
		for (i = 0; i < n - z; i++){
			coord[i] = (bool **)malloc(n*sizeof(bool *));
			for (j = 0; j < n; j++){
				coord[i][j] = (bool *)malloc(n*sizeof(bool));
			}
		}

		for (x = 0; x < n; x++){
			for (y = 0; y < n; y++){
				if (coord_fix[0][x][y]){

					Table[z*2] = x;
					Table[z*2 + 1] = y;

					for (i = 0; i < n - z; i++){
						for (j = 0; j < n; j++){
							for (k = 0; k < n; k++){
								coord[i][j][k] = coord_fix[i][j][k];
							}
						}
					}

					Coord_Occupation(x, y, z, n, coord, coord_Change, Distance);
					if (Calculation(z + 1, n, coord + 1, coord_Change, Distance, Table)){

						for (i = 0; i < n - z; i++){
							for (j = 0; j < n; j++){
								free((void *)coord[i][j]);
								coord[i][j] = NULL;
							}
						}
						for (i = 0; i < n - z; i++){
							free((void *)coord[i]);
							coord[i] = NULL;
						}
						free((void *)coord);
						coord = NULL;

						return true;
					}
				}
			}
		}

		for (i = 0; i < n - z; i++){
			for (j = 0; j < n; j++){
				free(coord[i][j]);
				coord[i][j] = NULL;
			}
		}
		for (i = 0; i < n - z; i++){
			free(coord[i]);
			coord[i] = NULL;
		}
		free(coord);
		coord = NULL;

	}
	else
		return true;

	/*Fail find coordinate*/
	return false;
}