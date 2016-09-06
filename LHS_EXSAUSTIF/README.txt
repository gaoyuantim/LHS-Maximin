
GUIDE:

	In this files there are three programs using the exaustif methodes in matlab environment. 

	Before running in Matlab, we should run in the Command Window of Matlab : mex filename.c

	After that, we need to input in the Command window of Matlab : [D2, Table] = filename(m,n)

									where m is dimension, n is number of points.


Information :

	Totally the methode realizing the ergodicity in the programs is same but we use two different structrues to realize the iteration. 

	In the program LHS.c and LHS_Optimal, we calculate every possible construction of points to find out the maximin. According to the article : <<An effient algorithm for constructing optimal design of computer experiments>> by Ruichen Jin, Wei Chen and Agus Sudjianto, just a part of the values need to be changed each time.

	In the program LHS.C, at last we get a normal group of points and a maximin value. But by the program LHS_Optimal.c, we can get a optimal result according to the critere in standard in the article: <<Exploratory designs for computational experiments>> by Max D.Morris and Toby J.Mitchell.

	In the program LHS_2.c, we change another methode of comparation. For example, we would like to find maximin of N points in a space of M dimensions. Now we decide a construction of n points in m dimensions and m < M, then we calculate all the possibility to find out the maximin for the rest dimensions with the result of the prior dimensions. After that we return the value of maximin to the previous dimension, and in the layer of previous dimension, we will gather all the maximins with different distribution in this dimension and return the max to its previous.At last, we will get the maximin for the space.

	For the program LHS_2.c, it will take more time than LHS.C, which is also the reason we contruct the program LHS_Optimal.c by first methode.

	The three programs are run in a computer equiped with Intel i7 3.4GHZ and RAM 16g. All the results meaningful we get are stored in the folder file.