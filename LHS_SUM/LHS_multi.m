%
%                                                                         
% Copyright Notice                                                         
%                                                                         
%    Copyright (C) 2016 CentraleSupelec                                   
%                                                                         
%    Author: Yuan Gao <gaoyuantim@gmail.com>                              
%                                                                         
% Copying Permission Statement                                            
%                                                                          
%  This program is free software; you can redistribute it and/or modify it
%  under the terms of the  GNU Lesser General Public License  as published
%  by the Free Software Foundation;  either version 3.0 of the License, or
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
%                                                                         
%
% Input :   n = number of points
%           k = number of dimension
%           sum[] = the sum we are insterested in
% 
% Output :  A table if there is a conbinaison satisfying the command;
%           "FALSE" if the result doesn't exist
%
% SYNTAX :  Table = LHS_multi ( n , k , sum[])
%
% Exemple : Table = LHS_multi(5, 8, [40]);
%
% Result :  
%   
%     Table =
%
%       0     1     2     3     4
%       0     1     2     4     3
%       0     3     4     1     2
%       1     2     4     3     0
%       1     4     0     2     3
%       1     4     2     0     3
%       1     4     2     3     0
%       2     1     4     0     3
%

function [ table_Fix ] = LHS_Bound_multi( n , k , addition )

num_Addition = length(addition);

Table = [];

if num_Addition > 0
    
    for i = 1:num_Addition
        
        Table_Plus = sos_decompose( addition(i) , k , n - 1);
        
        while Table_Plus(1,1) >= n * n
            
            Table_Plus(1,:) = [];
        
        end
        
        Table = [ Table ; Table_Plus];
        
    end
    
    [Height, Length] = size(Table);
     
    num_Result = zeros(1,(Height + 1)*(n - 1));
     
    for i = 1:Height
        for j = 1:Length
            
            num = sqrt(Table(i,j));
             
            num_Result(1,(i * (n - 1) + num)) = num_Result(1 , (i * (n - 1) + num)) + 1;
             
        end
    end
    
    table_Fix = LHS_Bound_Sum(n, k, Height, num_Result)';
    
end


end