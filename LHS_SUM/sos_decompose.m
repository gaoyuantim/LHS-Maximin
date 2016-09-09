% sos_decompose computes decompositions into sum of squares
%
% SYNTAX:  sos = sos_decompose (S, k, n - 1)
%
%   computes all possible decompositions of integer S into a sum of k squares
%
% EXAMPLE:  S = 56, k = 6
%
%    >> sos = sos_decompose (56, 6, 6)
%
%    sos =
%
%       36    16     1     1     1     1
%       36     4     4     4     4     4
%       25    16     9     4     1     1
%       16     9     9     9     9     4

% Copyright Notice
%
%    Copyright (C) 2016 CentraleSupelec
%
%    Author: Julien Bect <julien.bect@centralesupelec.fr>
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

function [sos, sos_all] = sos_decompose (S, k, dmax)

if k == 1

    sos = cell (S, 1);
    dmax = min (dmax, floor (sqrt (S)));
    for t = 1:dmax
        sos{t^2, 1} = t^2;
    end

else

    [~, sos] = sos_decompose (S, k - 1, dmax);
    sos = [sos cell(S, 1)];

    for SS = k:S,
        dmax0 = min (dmax, floor (sqrt (SS - (k - 1))));
        for t = dmax0:-1:1
            B = sos{SS - t^2, k - 1};
            if ~ isempty (B)
                A = t^2 * (ones (size (B, 1), 1));
                sos{SS, k} = [sos{SS, k}; [A B]];
            end
        end
        sos{SS, k} = unique (sort (sos{SS, k}, 2, 'descend'), 'rows', 'stable');
    end

end % if

sos_all = sos;
sos = sos_all{end, end};

end
