## Guide
In this files there are a programs realized by calculating sum distance in Matlab environment.

  Input in the Command window of Matlab : 

    Table = LHS_multi ( n , k , sum)

Here, _n_ is number of points, _k_ means dimension. _sum_ means the group of distance calculated.

## Example
After you have initialized mex files, input this:

    Table LHS_multi(5, 8, [40])
    
Here, you can get the bound in 8 dimensions for 5 points and all the square distance between each two points is 40.

## About algorithm
This method focus on the special situation when the dimension k >= the number of points n. According to the article _Bounds for Maximin Latin Hypercube Designs_ by Edwin R.van Dam, Gijs Rennen, Bart Husslage, we get a conclusion:

    d^2 <= [ n * (n + 1) * k / 6]

however, according to the result of experiments, we discover that only when dimension k >= points n, the maximin distance would be close to the maximum theoretical value.

For example : 

Dimension k | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14
------------|---|---|---|---|---|---|---|---|---|---|---|---|---|---
d^2 max | 1 | 5 | 11 | 15 |24 | 27 | 32 | 40 | 43 | 50 | 54 | 60 | 64 | 70
Upper Bound | 5 | 10 | 15 | 20 | 25 | 30 | 35 | 40 | 45 | 50 | 55 | 60 | 65 | 70

So we try to solve the problem from this point of view.

For example, we would like to solve when _k = 8_ and _n = 5_, we can calculate that the _upper bound = 40_. Then we decompose the sum by square numbers:

16 | 9 | 4 | 4 | 4 | 1 | 1 | 1
---|---|---|---|---|---|---|---
 9 | 9 | 9 | 9 | 1 | 1 | 1 | 1


We find that there are only two types possible for 8 kinds of combinations, so we would like to construct the points by these two types. At last, we can get the result :

0 | 1 | 2 | 3 | 4
---|---|---|---|---
0 | 1 | 2 | 4 | 3
0 | 3 | 4 | 1 | 2
1 | 2 | 4 | 3 | 0
1 | 4 | 0 | 2 | 3
1 | 4 | 2 | 0 | 3
1 | 4 | 2 | 3 | 0
2 | 1 | 4 | 0 | 3

By this method, we can not only find out the combination, we can also get the bound. However, when the dimension is bigger than 10, it will take a little too long time to get the result.
