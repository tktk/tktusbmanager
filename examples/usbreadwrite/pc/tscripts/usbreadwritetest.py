# -*- coding: utf-8 -*-

from taf import *
from taf.tools import cpp

module.BUILDER = cpp.program

module.TARGET = 'usbreadwritetest'

module.SOURCE = [
    'main.cpp',
]

module.LIB = [
    'usb-1.0',
]
