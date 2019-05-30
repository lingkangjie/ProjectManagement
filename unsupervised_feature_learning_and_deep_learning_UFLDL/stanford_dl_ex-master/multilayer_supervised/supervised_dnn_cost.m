function [ cost, grad, pred_prob] = supervised_dnn_cost( theta, ei,...
    data, labels, pred_only)
%SPNETCOSTSLAVE Slave cost function for simple phone net
%   Does all the work of cost / gradient computation
%   Returns cost broken into cross-entropy, weight norm, and prox reg
%        components (ceCost, wCost, pCost)

%% default values
po = false;
if exist('pred_only','var')
  po = pred_only;
end;

%% reshape into network
stack = params2stack(theta, ei);
numHidden = numel(ei.layer_sizes) - 1;
hAct = cell(numHidden+1, 1);%hAct is <2*1 cell>
gradStack = cell(numHidden+1, 1);%gradStack is <2*1 cell>
%% forward prop
%%% YOUR CODE HERE %%%
m = size(data,2);%m=60000
m_inv = 1/m;
for l = 1:(numHidden+1)
    if l==1
        hAct{l} = data; % just store input data<784*60000>
    else
% when run to here, l=2.
% stack{l-1}.W <256*784>,hAct{l-1}=hAct{1} <784*60000>
% after multiplying,<256*60000>meaning demension from 
% 784 reduce to 256. stack{l-1}.b=stack{1} <256*1>
% repmat(stack{l-1}.b,[1,m]) is <256*60000>,meaning row direction
% repeats 1 times and column direction repeats 60000 times.
% because for a sample we have 256 dimensions in hidden layer,so
% we have 256 neurons,meaning has 256 bias for a sample.
% here hAct{l} is "L",l=2, not number 1.
        hAct{l} = stack{l-1}.W * hAct{l-1} + ...
            repmat(stack{l-1}.b,[1,m]);
        if strcmp(ei.activation_fun,'logistic')
            hAct{l} = sigmoid(hAct{l});
        elseif strcmp(ei.activation_fun,'tanh')
            hAct{l} = tanh(hAct{l});
        elseif strcmp(ei.activation_fun,'rlu')
            hAct{l}(hAct{l}<0) = 0;
        end
    end
end
% layer l=2,stack{l}.W <10,256>,hAct{l}< 256*60000>,
% stack{l}.b <10,1>
% repmat(stack{l}.b,[1,m])<10,60000>. From another perspective,
% the output layer mapping 256 demensions to 10 demensions.
% pred_prob <10*60000>
pred_prob = stack{l}.W * hAct{l} + repmat(stack{l}.b,[1,m]);
% perform element-by-element binary operation for two array.
% max(pred_prob,[],1) <1*60000>, here, "max" function 
% produces the maximum values along the first 
% dimension of "pred_prob".
% normalizes the output to range[0,1] to represent probability.
% bsxfun works like it,suppose 
% A = [1 4 6  B=[2 2 2],after bsxfun(@minus,A,B) = [-1 2 4
%     5 7 3]                                        3  5  2]
% after sigmoid function, each neuron output  
% a variable range[0,1],however
% for Softmax regression, we treat these outputs 
% as weighed inputs,and put these outputs into 
% exp power function and divided by its sum 
% respectively for each sample
pred_prob = bsxfun(@minus,pred_prob,max(pred_prob,[],1));
pred_prob = exp(pred_prob);
pred_prob = bsxfun(@rdivide, pred_prob, sum(pred_prob));

%% return here if only predictions desired.
if po
  cost = -1; ceCost = -1; wCost = -1; numCorrect = -1;
  grad = [];
  return;
end;

%% compute cost
%%% YOUR CODE HERE %%%
% groundTruth <10*60000>,meaning if groundTruth(j,i) not equal to 0, 
% that j is the hand number label for input sample i
groundTruth = full(sparse(labels, 1:m, 1));
% groundTruth <10*60000>,pred_prob <10*60000>,after sum, <1*60000>
cost = -mean(sum(groundTruth .* log(pred_prob)));
% to add L2 regularization term.
ceCost = -(groundTruth - pred_prob);
wCost = 0;
% for all weighs in the network, just sum all weighs^2 up.Here wCost
% called weigh cost
for l=1:(numHidden+1)
    wCost = wCost + sum(sum(stack{l}.W .^ 2));
end
wCost = 0.5 * ei.lambda * wCost;
cost = cost + wCost;

%% compute gradients using backpropagation
%%% YOUR CODE HERE %%%
delta = cell(numHidden+1,1);%delta<2*1 cell>
for l=numHidden+1:-1:1 %from back to front, numHidden = 1
    % layer l is 2,2==1+1,output layer backpro
    if(l==numHidden+1)
        delta{l} = ceCost;
        %the residual between groundTruth and computed results
        % delta{2}<10*60000>,hAct{2}<256*60000>,actually is hidden
        % layer (layer 2) output (activation output).
        % I AM CONFUSED AT HERE.
        gradStack{l}.W = delta{l} * hAct{l}' .* m_inv;
        gradStack{l}.b = mean(delta{l},2);
    else
        % stack{2}.W'<60000*10>,delta{2}<10*60000>
        delta{l} = stack{l+1}.W' * delta{l+1};
        if strcmp(ei.activation_fun,'logistic')
            delta{l} = delta{l} .* hAct{l+1} .* (1-hAct{l+1});
        elseif strcmp(ei.activation_fun,'tanh')
            delta{l} = delta{l} .* (1-hAct{l+1}.^2);
        elseif strcmp(ei.activation_fun,'rlu')
            delta{l}(hAct{l+1}<0) = 0;
        end
        % I AM doubt here,why not consider the W2 matrix?
        % l = 1
        gradStack{l}.W = delta{l} * hAct{l}' .* m_inv + ...
            ei.lambda .* stack{l}.W;
        gradStack{l}.b = mean(delta{l},2);
    end
end

%% compute weight penalty cost and gradient for non-bias terms
%%% YOUR CODE HERE %%%

%% reshape gradients into vector
[grad] = stack2params(gradStack);
end

function sigm = sigmoid(x)
    sigm = 1./(1+exp(-x));
end