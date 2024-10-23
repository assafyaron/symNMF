import math
import sys
import pandas as pd
import numpy as np
import mysymnmfsp as SymNMF

np.random.seed(1234)

def initialize_H(w_mat, N, k):
    # Calculate the average of all entries in w_mat
    m = np.mean(w_mat)

    # Calculate the upper bound for the random values
    upper_bound = 2 * np.sqrt(m / k)
    
    # Initialize H with random values from the interval [0, upper_bound]
    H = np.random.uniform(low=0, high=upper_bound, size=(N, k))
    
    return H

def main():
    try:
        # Get data from console
        input_data = sys.argv
        k, goal, input_file = int(input_data[1]), input_data[2], input_data[3]
        iter, eps = 300, 0

        # Create Vectors dataframe from csv and save dimension and number of vectors
        vectors = pd.read_csv(input_file, header=None)
        N = int(len(vectors))
        vecdim = len(vectors.columns)
        # Convert vectors to python list of lists
        vectors = vectors.values.tolist()
        
        matrix_goal = None # The matrix to calculate and return

        # Choose which matrix to calculate and return
        match goal:
            case "sym":
                matrix_goal = SymNMF.sym(vectors, N, vecdim) # Calling sym function in C to calculate the matrix
            case "ddg":
                matrix_goal = SymNMF.ddg(vectors, N, vecdim) # Calling ddg function in C to calculate the matrix
            case "norm":
                matrix_goal = SymNMF.norm(vectors, N, vecdim) # Calling norm function in C to calculate the matrix
            case "symnmf":
                w_mat = SymNMF.norm(vectors, N, vecdim) # Calling norm function in C to calculate W matrix
                h_mat = initialize_H(w_mat, N, k) # Initialize H matrix
                matrix_goal = SymNMF.symnmf(w_mat, h_mat, N, vecdim, k, iter, eps) # Calling symnmf function in C to calculate the matrix
            case _:
                print("An Error Has Occurred")
                return

        # print matrix_goal until 4 decimal points
        for centroid in matrix_goal:
            print(','.join(format(x, ".4f") for x in centroid))

    except Exception:
        print("An Error Has Occurred")


if __name__ == "__main__":
    main()

# TODO:
# - Implement API for sym, ddg, norm, symnmf
