from setuptools import Extension, setup

module = Extension('mysymnmfsp', sources=['symnmfmodule.c'])

setup(
    name='symnmf',
    version='1.0',
    description='SymNMF module',
    ext_modules=[module]
)
