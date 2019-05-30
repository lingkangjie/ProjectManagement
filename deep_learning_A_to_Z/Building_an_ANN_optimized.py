
# coding: utf-8

# In[1]:


import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os

scripts_dir = os.getcwd()
abs_file_path = os.path.join(scripts_dir, 'data\Churn_Modelling.csv')
print(abs_file_path)


# In[2]:


dataset = pd.read_csv('.\data\Churn_Modelling.csv')
X = dataset.iloc[:, 3:13].values
y = dataset.iloc[:, 13].values

from sklearn.preprocessing import LabelEncoder, OneHotEncoder
labelencoder_X_1 = LabelEncoder()
labelencoder_X_2 = LabelEncoder()
X[:, 1] = labelencoder_X_1.fit_transform(X[:, 1])
X[:, 2] = labelencoder_X_2.fit_transform(X[:, 2])
onehotencoder = OneHotEncoder(categorical_features=[1])
X = onehotencoder.fit_transform(X).toarray()
X = X[:, 1:]

from sklearn.model_selection import train_test_split
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size = 0.2, random_state = 42)

from sklearn.preprocessing import StandardScaler
sc = StandardScaler()
X_train = sc.fit_transform(X_train)
X_test = sc.transform(X_test)


# In[3]:


from sklearn.model_selection import cross_val_score
from tensorflow.contrib.keras.api.keras.wrappers.scikit_learn import KerasClassifier
from sklearn.model_selection import GridSearchCV
from tensorflow.contrib.keras.api.keras.models import Sequential
from tensorflow.contrib.keras.api.keras.layers import Dense, Dropout
from tensorflow.contrib.keras import backend


# In[4]:


# first we build a ANN model, and then wrapped it to Sklearn with different parameters being optimized
def build_classifier(optimizer) :#  we explicitly use optimizer parameter here, and other parameters are in fit method
    Classifier = Sequential()
    Classifier.add(Dense(units = 6, kernel_initializer = 'uniform', activation = 'relu', input_dim = 11))
    Classifier.add(Dropout(0.1))
    Classifier.add(Dense(units = 6, kernel_initializer = 'uniform', activation = 'relu'))
    Classifier.add(Dense(units = 1, kernel_initializer = 'uniform', activation = 'sigmoid'))
    Classifier.compile(optimizer = optimizer, loss = 'binary_crossentropy', metrics = ['accuracy'])
    return Classifier


# In[10]:


Classifier = KerasClassifier(build_fn=build_classifier)

parameters = {
    'batch_size': [500, 1000],
    'epochs': [5, 10],
    'optimizer': ['adam', 'rmsprop']
}

grid_search = GridSearchCV(estimator=Classifier,
                          param_grid = parameters,
                          scoring = 'accuracy', 
                          cv = 3,
                          n_jobs = 1)
grid_search_resluts = grid_search.fit(X_train, y_train)


# In[9]:


best_parameters = grid_search_resluts.best_params_
best_accuracy = grid_search_resluts.best_score_
print(best_parameters)
print(best_accuracy)

