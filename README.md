# SymNMF_v1
Implementation of a clustering algorithm based on symmetric non-negative matrix factorization in Python and C.

## symnmf.py
This is a the Python interface for the symNMF algorithm.
It recieves 3 arguments from console: _k, goal, input_ where _k_ is the number of clusters, _goal_ determines the function to produce and _input_ is a .txt file in a CSV format representing vectors.

The Python code will process the vectors, calculate and output the required matrix goal using a Python-C API.

Example: > python symnmf.py 2 symnmf input_1.txt

## sumnmf.c
This is a the C implementation for the symNMF algorithm.
It recieves a goal from console and a .txt file into input.

The C code will process the vectors, calculate and output the required matrix goal.

Example: > ./symnmf 2 sym < input_1.txt

## symnmfmodule.c
Python-C API. Defines the C extension for the Python code.

## analysis.py
Compares SymNMF to [Kmeans](https://github.com/OzCabiri/K-means-clustering_v1).
Applies both methods to a given dataset and reports the silhouette score from sklearn.metrics.
The score is calculated as the mean of the silhouette coefficient of each data point seperatly.
A higher score indicates better defined clusters.

It recieves 2 arguments from console: _k_, _input_ where _k_ is the number of clusters and _input_ is a .txt file in a CSV format representing vectors.

Example: > python analysis.py 5 symnmf input_1.txt