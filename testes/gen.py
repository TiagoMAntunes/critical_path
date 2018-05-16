#Author: Tiago Antunes


from random import random
from math import sqrt
prev = []
for _ in range(10000):
    id = str(int(random()*2147483647)+1)
    while id in prev:
        id = str(int(random()*2147483647)+1)
    r = 'add ' + id + ' "" ' + str(int(random()*1000)+1)
    if random() > 0.5 and prev != []:
        m = int(random()*len(prev)/sqrt(len(prev)*10))
        for i in range(m):
            r += ' ' + str(prev[int(random()*len(prev))])
    print(r)
    prev.append(id)

size = len(prev)-1

print('path')
print('depend ' + str(prev[int(random()*len(prev))]))
for _ in range(100):
    print("duration " + str(int(random()*1000)+1))
print('exit')
