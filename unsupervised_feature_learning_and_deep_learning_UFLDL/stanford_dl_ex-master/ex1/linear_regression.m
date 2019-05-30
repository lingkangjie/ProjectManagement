function [f,g] = linear_regression(theta, X,y)
  %
  % Arguments:
  %   theta - A vector containing the parameter values to optimize.
  %   X - The examples stored in a matrix.
  %       X(i,j) is the i'th coordinate of the j'th example.
  %   y - The target value for each example.  y(j) is the target for example j.
  %
  
  m=size(X,2);
  n=size(X,1);

  f=0;
  g=zeros(size(theta));
  % f: cost function sum , g:gradient sum
 for row = 1:m
      curr_x = X(:,row);
      curr_y = y(:,row);
      f = f + (1/2).*(theta'*curr_x-curr_y)^2;
      g = g + curr_x.*(theta'*curr_x-curr_y);
 end
% another version:vectorization
%   h = theta' * X - y;
%   f = (1/2) * h * h';
%   g = X*h';