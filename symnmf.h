/* C header file */
#ifndef SYMNMF_H
#define SYMNMF_H

double** sym(double** vectors, int N, int vecdim);
double** ddg(double** vectors, int N, int vecdim);
double** norm(double** vectors, int N, int vecdim);
double** symnmf(double** W, double** H, int N, int vecdim, int k, int iter, double eps);

#endif