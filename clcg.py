#!/usr/bin/python

from sys import argv

a1 = 40014
a2 = 40692
m1 = 2147483563
m2 = 2147483399


if len(argv) < 3:
    print('Wrong number of arguments')
    exit(1)

s1 = int(argv[1])
s2 = int(argv[2])
n = int(argv[3]) if len(argv) >= 4 else 10

for _ in range(n):
    z = (s1 - s2) % (m1 - 1)
    r = z/m1 if z > 0 else (m1 - 1)/m1
    print("{:0.8F}".format(r*10))
    # print(z)
    s1 = (a1 * s1) % m1
    s2 = (a2 * s2) % m2
