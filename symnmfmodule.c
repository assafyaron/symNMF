# define PY_SSIZE_T_CLEAN
# include <Python.h>
# include <stdio.h>
# include <math.h>
# include "symnmf.h"

double** convert_matrix(PyObject* self, PyObject* args)
{
    int k, N, vecdim, iter;
    double eps;
    PyObject* vec_arr_obj;
    PyObject* rows_obj;
    double** vec_arr;
    double** rows;
    
    /* This parses the Python arguments into:
        1. int (i) variables named k,N,vecdim,iter
        2. double (d) variable named eps
        3. A pointer to a pointer to a double (O) variable named vec_arr
        4. A pointer to a pointer to a double (O) variable named rows */
    if(!PyArg_ParseTuple(args, "iiiidOO", &k, &N, &vecdim, &iter, &eps, &vec_arr_obj, &rows_obj))
    {
        return NULL; /* In the CPython API, a NULL value is never valid for a
                        PyObject* so it is used to signal that an error has occurred. */
    }

    // Allocate memory for C arrays and check if allocation failed
    vec_arr = malloc(N*sizeof(double*));
    rows = malloc(k*sizeof(double*));
    if (vec_arr == NULL || rows == NULL)
    {
        PyErr_SetString(PyExc_MemoryError, "Memory allocation failed");
        free(vec_arr);
        free(rows);
        return NULL;
    }
    
    
    int i,j;
    /* Allocate memory for each vector in vec_arr and rows */
    for (i=0;i<N;i++)
    {
        vec_arr[i] = malloc(vecdim*sizeof(double));
        if (vec_arr[i] == NULL)
        {
            PyErr_SetString(PyExc_MemoryError, "Memory allocation failed");
            for (j=0;j<i;j++)
            {
                free(vec_arr[j]);
            }
            free(vec_arr);
            free(rows);
            return NULL;
        }
    }
    for (i=0;i<k;i++)
    {
        rows[i] = malloc(vecdim*sizeof(double));
        if (rows[i] == NULL)
        {
            PyErr_SetString(PyExc_MemoryError, "Memory allocation failed");
            for (j=0;j<i;j++)
            {
                free(rows[j]);
            }
            free(rows);
            for (j=0;j<N;j++)
            {
                free(vec_arr[j]);
            }
            free(vec_arr);
            return NULL;
        }
    }

    /* Convert python lists into C arrays */
    PyObject* vec1;
    PyObject* vec2;
    for (i=0;i<N;i++)
    {
        vec1 = PyList_GetItem(vec_arr_obj, i);
        for (j=0;j<vecdim;j++)
        {
            vec_arr[i][j] = PyFloat_AsDouble(PyList_GetItem(vec1,j));
        }
    }
    for (i=0;i<k;i++)
    {
        vec2 = PyList_GetItem(rows_obj, i);
        for (j=0;j<vecdim;j++)
        {
            rows[i][j] = PyFloat_AsDouble(PyList_GetItem(vec2,j));
        }
    }
}

/* Converts N and vecdim to C integers. */
int* convert_N_vecdim(PyObject* self, PyObject* args)
{
    int N, vecdim;
    if(!PyArg_ParseTuple(args, "ii", &N, &vecdim))
    {
        return NULL; /* In the CPython API, a NULL value is never valid for a
                        PyObject* so it is used to signal that an error has occurred. */
    }
    int* result = malloc(2*sizeof(int));
    if (result == NULL)
    {
        PyErr_SetString(PyExc_MemoryError, "Memory allocation failed");
        return NULL;
    }
    result[0] = N;
    result[1] = vecdim;

    return result;
}

static PyObject* symmodule(PyObject* self, PyObject* args)
{
    int i,j;
    double** vectors_matrix = convert_matrix(self, args);
    int* N_vecdim = convert_N_vecdim(self, args);
    double** sym_matrix = sym(vectors_matrix, N_vecdim[0], N_vecdim[1]);

    /* Convert our C double** to a python list of lists */
    PyObject* final_sym = PyList_New(N_vecdim[0]);
    PyObject* final_row;
    for (i=0;i<N_vecdim[0];i++)
    {
        final_row = PyList_New(N_vecdim[0]);
        for (j=0;j<N_vecdim[0];j++)
        {
            PyList_SetItem(final_row, j, PyFloat_FromDouble(sym_matrix[i][j]));
        }
        PyList_SetItem(final_sym, i, final_row);
    }

    /* Free all allocated memory */
    for (i=0;i<N_vecdim[0];i++)
    {
        free(vectors_matrix[i]);
    }
    free(vectors_matrix); 

    free(N_vecdim);

    for (i=0;i<N_vecdim[0];i++)
    {
        free(sym_matrix[i]);
    }
    free(sym_matrix);

    return Py_BuildValue("O", final_sym);
}

static PyObject* ddgmodule(PyObject* self, PyObject* args)
{
    int i,j;
    double** vectors_matrix = convert_matrix(self, args);
    int* N_vecdim = convert_N_vecdim(self, args);
    double** ddg_matrix = ddg(vectors_matrix, N_vecdim[0], N_vecdim[1]);

    /* Convert our C double** to a python list of lists */
    PyObject* final_ddg = PyList_New(N_vecdim[0]);
    PyObject* final_row;
    for (i=0;i<N_vecdim[0];i++)
    {
        final_row = PyList_New(N_vecdim[0]);
        for (j=0;j<N_vecdim[0];j++)
        {
            PyList_SetItem(final_row, j, PyFloat_FromDouble(ddg_matrix[i][j]));
        }
        PyList_SetItem(final_ddg, i, final_row);
    }

    /* Free all allocated memory */
    for (i=0;i<N_vecdim[0];i++)
    {
        free(vectors_matrix[i]);
    }
    free(vectors_matrix); 

    free(N_vecdim);

    for (i=0;i<N_vecdim[0];i++)
    {
        free(ddg_matrix[i]);
    }
    free(ddg_matrix);

    return Py_BuildValue("O", final_ddg);
}

static PyObject* normmodule(PyObject* self, PyObject* args)
{
    int i,j;
    double** vectors_matrix = convert_matrix(self, args);
    int* N_vecdim = convert_N_vecdim(self, args);
    double** norm_matrix = norm(vectors_matrix, N_vecdim[0], N_vecdim[1]);

    /* Convert our C double** to a python list of lists */
    PyObject* final_norm = PyList_New(N_vecdim[0]);
    PyObject* final_row;
    for (i=0;i<N_vecdim[0];i++)
    {
        final_row = PyList_New(N_vecdim[0]);
        for (j=0;j<N_vecdim[0];j++)
        {
            PyList_SetItem(final_row, j, PyFloat_FromDouble(norm_matrix[i][j]));
        }
        PyList_SetItem(final_norm, i, final_row);
    }

    /* Free all allocated memory */
    for (i=0;i<N_vecdim[0];i++)
    {
        free(vectors_matrix[i]);
    }
    free(vectors_matrix); 

    free(N_vecdim);

    for (i=0;i<N_vecdim[0];i++)
    {
        free(norm_matrix[i]);
    }
    free(norm_matrix);

    return Py_BuildValue("O", final_norm);
}

static PyObject* symnmfmodule(PyObject* self, PyObject* args)
{
    int* final_nmf = NULL;
    return Py_BuildValue("O", final_nmf);;
}

static PyMethodDef symnmfMethods[] = {
    {"sym",                   /* the Python method name that will be used */
      (PyCFunction) symmodule, /* the C-function that implements the Python function and returns static PyObject*  */
      METH_VARARGS,           /* flags indicating parameters accepted for this function */
      PyDoc_STR("Calculates similarity matrix from given vectors")}, /*  The docstring for the function */

    {"ddg",
      (PyCFunction) ddgmodule,
      METH_VARARGS,
      PyDoc_STR("Calculates diagonal degree matrix from given vectors")},
    
    {"norm",
      (PyCFunction) normmodule,
      METH_VARARGS,
      PyDoc_STR("Calculates normalized similarity matrix from given vectors")},

    {"symnmf",
      (PyCFunction) symnmfmodule,
      METH_VARARGS,
      PyDoc_STR("Calculates and updates the association matrix (H) matrix from given vectors until convergence or max iterations")},

    {NULL, NULL, 0, NULL}     /* The last entry must be all NULL as shown to act as a
                                 sentinel. Python looks for this entry to know that all
                                 of the functions for the module have been defined. */
};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "mysymnmfsp", /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,  /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    symnmfMethods /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC PyInit_mysymnmfsp(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (!m) {
        return NULL;
    }
    return m;
}
