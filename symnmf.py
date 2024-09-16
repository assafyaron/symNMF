import math
import sys
import pandas as pd
import numpy as np
# import mysymnmfsp as SymNMF

np.random.seed(1234)

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
                return
            case "ddg":
                return
            case "norm":
                return
            case "symnmf":
                return

        # Print the chosen vectors
        print(','.join(str(x) for x in matrix_goal))
        # print final centroids until 4 decimal points
        for centroid in matrix_goal:
            print(','.join(format(x, ".4f") for x in centroid))

    except Exception:
        print("An Error Has Occurred")


if __name__ == "__main__":
    main()

# TODO:
# - Implement API for sym, ddg, norm, symnmf
# - Implement sym, ddg, norm functions
# - Implement symnmf using norm and calculate initial H
# - Don't forget to import the module