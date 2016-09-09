% Input: n = number of points
%        m = number of dimension
%        D2 = the square distance tested
% 
% Output: A table of points constructed by the maximin D2 if D2is suitable;
%         "D2 too big" when D2 is too big
%         "Dimension is to big" if m is bigger than 3
%         
% SYNTAX: table = LHS_D2(n , m , D2)
% 
% Exemple:    table = LHS(15, 2, 17)
% 
%     table = 
%                  0     0
%                  1     4
%                  2     8
%                  3    12
%                  4     1
%                  5     5
%                  6     9
%                  7    13
%                  8     2
%                  9     6
%                 10    10
%                 11    14
%                 12     3
%                 13     7
%                 14    11

% Copyright Notice
%
%    Copyright (C) 2016 Yuan Gao <gaoyuantim@gmail.com>
%
%    Author: Yuan Gao <gaoyuantim@gmail.com>
%
% Copying Permission Statement
%
%  This program is free software; you can redistribute it and/or modify it
%  under the terms of the  GNU Lesser General Public License  as published
%  by the Free Software Foundation;  either version 2.1 of the License, or
%  (at your option) any later version.
%
%  This program  is distributed  in the hope  that it will be useful,  but
%  WITHOUT ANY WARRANTY;  without even  the implied warranty of MERCHANTA-
%  BILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
%  Public License for more details.
%
%  You  should have  received  a copy of  the GNU  Lesser  General  Public
%  License along with this program;  if not, see
%  <http://www.gnu.org/licenses/>.

function [ table ] = LHS_D2( n , m , D2 )

if m == 2
    
    table = LHS_D2_2( n , D2 );

else
    if m == 3
        
        table = LHS_D2_3( n , D2 );
    
    else
        
        disp('Dimension is to big');
    end
end