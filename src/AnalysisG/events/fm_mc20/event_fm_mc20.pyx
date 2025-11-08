# distutils: language=c++
# cython: language_level=3

from AnalysisG.core.event_template cimport EventTemplate

cdef class FmMC20(EventTemplate):

    def __cinit__(self):
        self.fm = new fm_mc20()
        self.ptr = <event_template*>(self.fm)

    def __init__(self): pass
    def __dealloc__(self): del self.ptr
