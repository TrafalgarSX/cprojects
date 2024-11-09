from distutils.core import setup, Extension
import numpy

sm9_module = Extension('_SM9',
                        #    sources=['SM9_wrap.c',],
                           sources=['SM9_wrap.cxx',],
                           libraries=['gmsslSM9'],
                           )

setup (name = 'SM9',
       version = '0.1',
       author      = "SWIG Docs",
       description = """Simple swig example from docs""",
       ext_modules = [sm9_module],
       include_dirs = [numpy.get_include()],
       py_modules = ["SM9"],
       )