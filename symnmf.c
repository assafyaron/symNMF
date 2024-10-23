#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void matrix_free(double **p, int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        free(p[i]);
    }
    free(p); 
}

double** matrix_malloc(double** new_matrix, int n, int m)
{
    int i;
    new_matrix = malloc(n*sizeof(double*));
    if(new_matrix == NULL)
    {
        printf("An Error Has Occured");
        matrix_free(new_matrix, n);
        return NULL;
    }
    for(i=0;i<n;i++)
    {
        new_matrix[i] = malloc(m * sizeof(double));
        if (new_matrix[i] == NULL)
        {
            printf("An Error Has Occured");
            matrix_free(new_matrix, n);
            return NULL;
        }
    }
    return new_matrix;
}

double** matrix_multiplication(double** matrix1, double** matrix2, int n, int m, int p)
{
    int i,j,k;
    double sum;
    double** result_matrix;
    result_matrix = matrix_malloc(result_matrix, n, p);

    for(i=0;i<n;i++)
    {
        for(j=0;j<p;j++)
        {
            sum = 0;
            for(k=0;k<m;k++)
            {
                sum += matrix1[i][k] * matrix2[k][j];
            }
            result_matrix[i][j] = sum;
        }
    }
    return result_matrix;
}

double** matrix_transpose(double** matrix, int n, int m)
{
    int i,j;
    double** transposed_matrix;
    transposed_matrix = matrix_malloc(transposed_matrix, m, n);

    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            transposed_matrix[j][i] = matrix[i][j];
        }
    }
}

double matrix_convergence(double** matrix1, double** matrix2, int n, int m)
{
    int i,j;
    double sum = 0;
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            sum += pow((matrix1[i][j] - matrix2[i][j]), 2);
        }
    }
    return sqrt(sum);
}

double euclidean_distance(double* vec1, double* vec2, int vecdim, int is_squared)
{
    int i;
    double sum = 0;
    for (i=0;i<vecdim;i++)
    {
        sum += pow(vec1[i] - vec2[i], 2);
    }
    if(is_squared==0)
    {
        return sqrt(sum);
    }
    else
    {
        return sum;
    }
}

double** sym(double** vectors, int N, int vecdim)
{
    int i,j;
    double value;
    double** sym_matrix;

    /* malloc a matrix of doubles sized N on vecdim */
    sym_matrix = matrix_malloc(sym_matrix, N, N);

    /* calculate the symilarity matrix */
    for(i=0;i<N;i++)
    {
        for(j=i;j<N;j++)
        {
            if (i==j)
            {
                sym_matrix[i][j] = 0;
            }
            else
            {
                value = euclidean_distance(vectors[i], vectors[j], vecdim, 1);
                value = exp(-value/2);

                sym_matrix[i][j] = value;
                sym_matrix[j][i] = value;
            }
        }
    }

    return sym_matrix;
}

double** ddg(double** vectors, int N, int vecdim)
{
    int i,j;
    double sum;
    double** sym_matrix;
    double** ddg_matrix;

    /* malloc a matrix of doubles sized N on vecdim */
    sym_matrix = sym(vectors, N, vecdim);
    ddg_matrix = matrix_malloc(ddg_matrix, N, N);

    /* initialize the ddg matrix */
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            ddg_matrix[i][j] = 0;
        }
    }
    /* calculate the ddg matrix */
    for(i=0;i<N;i++)
    {
        sum = 0;
        for(j=0;j<N;j++)
        {
            sum += sym_matrix[i][j];
        }
        ddg_matrix[i][i] = sum;
    }

    matrix_free(sym_matrix, N);
    return ddg_matrix;
}

double** norm(double** vectors, int N, int vecdim)
{
    int i,j;
    double sum;
    double** sym_matrix;
    double** ddg_matrix;
    double** norm_matrix;

    /* malloc a matrix of doubles sized N on vecdim */
    sym_matrix = sym(vectors, N, vecdim);
    ddg_matrix = ddg(vectors, N, vecdim);
    norm_matrix = matrix_malloc(norm_matrix, N, N);

    /* calculate the norm matrix */
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            norm_matrix[i][j] = sym_matrix[i][j] / sqrt(ddg_matrix[i][i] * ddg_matrix[j][j]);
        }
    }

    matrix_free(sym_matrix, N);
    matrix_free(ddg_matrix, N);
    return norm_matrix;
}

double** symnmf(double** W, double** H, int N, int vecdim, int k, int iter, double eps)
{
    int i,j,l;
    double beta = 0.5;
    double** new_H;
    new_H = matrix_malloc(new_H, N, k);

    for(l=0;l<iter;l++)
    {
        /* calculate the numerator and denominator matrices */
        double** nom_matrix;
        double** denom_matrix;

        nom_matrix = matrix_multiplication(W, H, N, N, k);
        denom_matrix = matrix_multiplication(matrix_multiplication(H, matrix_transpose(H, N, k), N, k, N), H, N, N, k);

        /* update the new_H matrix */
        for(i=0;i<N;i++)
        {
            for(j=0;j<k;j++)
            {
                new_H[i][j] = H[i][j] * (1 - beta + beta*(nom_matrix[i][j] / denom_matrix[i][j]));
            }
        }

        matrix_free(nom_matrix, N);
        matrix_free(denom_matrix, N);

        if(matrix_convergence(new_H, H, N, k) < eps)
        {
            return new_H;
        }
        else
        {
            for(i=0;i<N;i++)
            {
                for(j=0;j<k;j++)
                {
                    H[i][j] = new_H[i][j];
                }
            }
        }
    }
    return new_H;
}

/* TODO:
        - Implement data recieval from input
        - Implement matrix output
*/
