import os
import sys
from ctypes import *


class UniversalUI:
    def __init__(self, library_path):
        self.lib = cdll.LoadLibrary(library_path)

    def CreateWindow(self, title, width, height):
        self.lib.CreateWindow(title, width, height)
        
    

