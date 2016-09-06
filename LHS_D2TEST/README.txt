


GUIDE:

	In this files there are two programs using the exaustif methodes in matlab environment. 

	Before running in Matlab, we should run in the Command Window of Matlab : mex filename.c

	After that, we need to input in the Command window of Matlab : Table = filename(n,D2)

									where n is number of points, D2 is the square of distance studied.


Information : 

	This methode focus on find out a group of points with a maximin square distance D2 we studied, at last we will get a result of table or nothing as D2 maybe too big.

	In this way, we check every possible position line by line. At each time when we try a position, there will be an area around the position becoming forbidden area, which means no other points could take there positions anymore.

	However, it gets more and more complicated by the growth of dimensions, so we just realized this methode in 2 and 3 dimensions. We can see that from the title that <<LHS_D2_2>> means 2 dimensions and <<LHS_D2_3>> means the space has 3 dimenions.

	At last, the two programs are run in a computer equiped with Intel i7 3.4GHZ and RAM 16g. All the results meaningful we get are stored in the folder file.