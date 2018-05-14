from random import random
from math import sqrt
prev = []
for _ in range(20000):
    id = str(int(random()*39999999))
    while id in prev:
        id = str(int(random()*39999999))
    r = 'add ' + id + ' "" ' + str(int(random()*1000))
    if random() > 0.5 and prev != []:
        m = int(random()*len(prev)/sqrt(len(prev)*10))
        for i in range(m):
            r += ' ' + str(prev[int(random()*len(prev))])
    print(r)
    prev.append(id)


print('duration')
print('path')
print('depend ' + str(prev[int(random()*len(prev))]))
print('exit')