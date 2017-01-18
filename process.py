#!/usr/bin/python

from sys import argv
from datetime import datetime
from binascii import *

if len(argv[1:]) == 0:
    print('nothing to do...')
    exit(0)

m = (1/4.656613e-10)

for uid in argv[1:]:
    uniqid = uid[:13] # uniqid part is the first 13 hex chars
    more_entropy = uid[len(uniqid):]

    sec_part = uniqid[:8]
    ms_part = '0'+uniqid[8:]

    sec = int(sec_part, 16)
    us = int(ms_part, 16)
    dme = float(more_entropy)/10

    print('Uniqid:{0} (s:{1} us:{2}) | more_entropy: {3} - {4}'.format(
        uniqid, sec, us, more_entropy, int(dme*m)))
