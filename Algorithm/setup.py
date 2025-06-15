from setuptools import setup, Extension
import pybind11

module = Extension(
    'graph',
    sources=['graph.cpp','cinterface.cpp'],
    include_dirs=[pybind11.get_include()],
    language='c++',
extra_compile_args=['/std:c++17']#MSVC
)

setup(
    name='graph',
    version='1.0',
    description='Pybind11 example',
    ext_modules=[module]
)