% Copyright Notice
%
%    Copyright (C) 2016 CentraleSupelec
%
%    Author: Julien Bect <julien.bect@centralesupelec.fr>
%            Yuan GAO <gaoyuantim@gmail.com>
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

MAXIMINLHS_ROOT_DIR = fileparts (mfilename ('fullpath'));
CURRENT_DIR = pwd ();

try
    MAXIMINLHS_METHOD_DIR = fullfile (MAXIMINLHS_ROOT_DIR, 'LHS_D2TEST');
    cd (MAXIMINLHS_METHOD_DIR);
    mex LHS_D2_2.c
    mex LHS_D2_3.c
    addpath (MAXIMINLHS_METHOD_DIR);
    
    MAXIMINLHS_METHOD_DIR = fullfile (MAXIMINLHS_ROOT_DIR, 'LHS_EXSAUSTIF');
    cd (MAXIMINLHS_METHOD_DIR);
    mex LHS_2.c
    mex LHS_Optimal.c
    addpath (MAXIMINLHS_METHOD_DIR);
    
    MAXIMINLHS_METHOD_DIR = fullfile (MAXIMINLHS_ROOT_DIR, 'LHS_SUM');
    cd (MAXIMINLHS_METHOD_DIR);
    mex LHS_Sum.c
    addpath (MAXIMINLHS_METHOD_DIR);
    
    MAXIMINLHS_METHOD_DIR = fullfile (MAXIMINLHS_ROOT_DIR, 'LHS_Bound');
    cd (MAXIMINLHS_METHOD_DIR);
    mex LHS_Bound.c
    mex LHS_Multi_Bound.c
    addpath (MAXIMINLHS_METHOD_DIR);
    
catch
    cd (CURRENT_DIR);
    rethrow (lasterror ()); %#ok<LERR>
end

cd (CURRENT_DIR);
clear CURRENT_DIR MAXIMINLHS_ROOT_DIR MAXIMINLHS_METHOD_DIR
