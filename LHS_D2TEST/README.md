## Guide
In this files there are two programs realized by testing maximin distance in matlab environment. Before running in Matlab, we should initialize the mex files in the Matlab.  You can run directly the file _init.m_ in your Matlab, or you have two steps:

  Run in the Command Window of Matlab :

    mex LHS_D2_2.c
    mex LHS_D2_3.c

  Then you can see these lines:

    Building with 'Microsoft Visual C++ 2013 Professional (C)'.
    MEX completed successfully.
    Building with 'Microsoft Visual C++ 2013 Professional (C)'.
    MEX completed successfully.

  Input in the Command window of Matlab : 

    Table = filename(n,D2)

Here, _n_ is number of points, _D2_ is the square of distance tested. _filename_ depends on which algorithme you choose.

## About algorithme
This method focus on find out a group of points with a maximin square distance D2 we studied, at last we will get a result of table or nothing as D2 maybe too big.

In this way, we check every possible position line by line. At each time when we try a position, there will be an area around the position becoming forbidden area, which means no other points could take these positions anymore.

However, it gets more and more complicated by the growth of dimensions, so we just realized this method in 2 and 3 dimensions. We can see that from the title that the first means 2 dimensions and the second means the space has 3 dimensions.

At last, all the results meaningful we get are stored in the folder file.
