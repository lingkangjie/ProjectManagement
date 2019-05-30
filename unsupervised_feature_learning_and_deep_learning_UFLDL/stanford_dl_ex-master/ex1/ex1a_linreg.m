% the "housing.data" coming from http://archive.ics.uci.edu/ml.

clc;
clear;
close all;
addpath ../common
addpath ../common/minFunc_2012/minFunc
addpath ../common/minFunc_2012/minFunc/compiled

% Load housing data from file.
data = load('housing.data');
data=data'; % put examples in columns

% Include a row of 1s as an additional intercept feature.
data = [ ones(1,size(data,2)); data ];

% Shuffle examples.
data = data(:, randperm(size(data,2)));

% Split into train and test sets
% The last row of 'data' is the median home price.
train.X = data(1:end-1,1:400);
train.y = data(end,1:400);

test.X = data(1:end-1,401:end);
test.y = data(end,401:end);

[features_n,samples_n] = size(train.X);

% Initialize the coefficient vector theta to random values.
theta = rand(features_n,1);

% Run the minFunc optimizer with linear_regression.m as the objective.
tic;
options = struct('MaxIter', 200);
theta = minFunc(@linear_regression, theta, options, train.X, train.y);
fprintf('Optimization took %f seconds.\n', toc);

% Plot predicted prices and actual prices from training set.
actual_prices = train.y;
predicted_prices = theta'*train.X;

% Print out root-mean-squared (RMS) training error.
train_rms=sqrt(mean((predicted_prices - actual_prices).^2));
fprintf('RMS training error: %f\n', train_rms);

% Print out test RMS error
actual_prices = test.y;
predicted_prices = theta'*test.X;
test_rms=sqrt(mean((predicted_prices - actual_prices).^2));
fprintf('RMS testing error: %f\n', test_rms);

% Plot predictions on test data.
plot_prices=true;
if (plot_prices)
  [actual_prices,I] = sort(actual_prices);
  predicted_prices=predicted_prices(I);
  plot(actual_prices, 'rx');
  hold on;
  plot(predicted_prices,'bx');
  legend('Actual Price', 'Predicted Price');
  xlabel('House #');
  ylabel('House price (1000 dollars)');
end