## Guide
In this files there are two programs realized by testing maximin distance in Matlab environment.

  Input in the Command window of Matlab : 

    Table = LHS_D2(n, m, D2)

Here, _n_ is number of points, _D2_ is the square of distance tested, _m_ is the dimension.

##Example

  After initialize the mex file, input in the Command window of Matlab :
  
    table = LHS_D2(15, 2, 17)
    
  We get :
  
0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14
---|---|---|---|---|---|---|---|---|---|---|---|---|---|---
0 | 4 | 8 | 12 | 1 | 5 | 9 | 13 | 2 | 6 | 10 | 14 | 3 | 7 | 11


## About algorithme
This method focus on finding out a group of points with a maximin square distance D2 we choose, at last we will get a result of table or nothing as D2 maybe too big.

In this way, we check every possible position line by line. At each time when we try a position, there will be an area around the position becoming forbidden area, which means no other points could take these positions anymore.

However, it gets more and more complicated by the growth of dimensions, so we just realized this method in 2 and 3 dimensions. We can see that from the title that LHS_D2_2.c means 2 dimensions and LHS_D2_3.c means the space has 3 dimensions.

At last, all the results meaningful we get are stored in the folder file.
