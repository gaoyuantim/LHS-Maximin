## Guide
In this files there are two programs realized by testing maximin distance in matlab environment.
Input in the Command window of Matlab :

    Bound = filename(n,D2)

Here, _n_ is number of points, _D2_ is the bound of square distance which we start to test but this D2 isn’t tested in the program. _filename_ depends on which method you choose.

## Example
### Example for LHS_Bound.c
After you have initialized the mex files, input in the Command window of Matlab:

    Bound = LHS_Bound(15, 17)
    
It means that we check the bound (square distance) which is started from 17 for 15 points in 2 dimensions.

### Example for LHS_Multi_Bound.c
Input in the Command window of Matlab:

    Bound = LHS_Multi_Bound(15, 17)
    
You will get a better bound which is also started from 17 for 15 points in 2 dimensions.

## About algorithm

This method focus on finding out the bound for the distribution in dimension 2. At last we will get a result of D2, a new bound for the distribution.

At first we use the method in the article: _Bounds for Maximin Latin Hypercube Designs_ by Edwin R.van Dam, Gijs Rennen, and Bart Husslage. This method can find out the best bound at present. We realize this method in the first program and the result accords with the data on [the site for LHS maximin](https://spacefillingdesigns.nl/).

After that, we realize that some parts of the method can be improved, so we add more constraint to find out a better bound. The second program is realize in this way. As a result, there isn’t much difference between the two methods, but the new method can really decrease the bound for some points. However, because of the new constraint, the new method takes longer time than the origin. 

At last, here is a part of the meaningful results we have got for present.

Points | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 42 | 43 | 44 | 45 | 46 | 47 | 48 | 49 | 50
---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
Old Bound| 13 | 13 | 13 | 13 | 17 | 17 | 18 | 20 | 20 | 25 | 25 | 25 | 26 | 29 | 29 | 29 | 29 | 32 | 34 | 34 | 34 | 34 | 37 | 40 | 41 | 41 | 41 | 45 | 45 | 45 | 45 | 45 | 50 | 52 | 52 | 52 | 53 | 58 | 58 | 58 | 61
New Bound|10 | 10 | 13 | 13 | 17 | 17 | 18 | 18 | 20 | 20 | 25 | 25 | 26 | 26 | 26 | 29 | 29 | 29 | 32 | 34 | 34 | 37 | 37 | 37 | 40 | 41 | 41 | 41 | 45 | 45 | 45 | 45 | 50 | 50 | 50 | 52 | 53 | 53 | 58 | 58 | 58 
D^2|10 | 10 | 13 | 13 | 17 | 17 | 17 | 18 | 18 | 18 | 18 | 20 | 25 | 26 | 26 | 26 | 26 | 26 | 29 | 29 | 29 | 32 | 32 | 34 | 37 | 37 | 37 | 37 | 41 | 41 | 41 | 41 | 41 | 41 | 50 | 50 | 50 | 50 | 50 | 50 | 52
