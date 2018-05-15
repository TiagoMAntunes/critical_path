#!/usr/bin/python
# File:  gen_rnd.py
# Author:  mikolas
# Created on:  Mon Apr 30 21:18:55 WEST 2018
# Copyright (C) 2018, Mikolas Janota
#!/usr/bin/python
import sys,random
from random import randint, sample, random, choice

def print_usage():
  print 'USAGE:'
  print ' ', sys.argv[0], '<number of commands>'

if len(sys.argv) != 2:
  print_usage()
  sys.exit(100)

ncmds = int(sys.argv[1])

ts = []

def a():
    global ts
    nid = randint(1,4294967295)
    deps = 0 if not ts else randint(1,len(ts))
    print 'add', nid, '"task '+str(nid)+'"', randint(1,100),
    if deps == 0:
        print
    else:
        s = sample(ts, deps)
        print ' '.join(map(str, s))
    ts.append(nid)

def d():
    if random() < 0.7: print 'duration'
    else: print 'duration', randint(1,100)

def p(): print 'path'

def r():
    if not ts:
        print 'remove 1000'
    else:
        rid = choice(ts)
        print 'remove', rid
        ts.remove(rid)

cmds = [d, p, r]

for i in range(ncmds):
    if random() < .4: a()
    else: (choice(cmds))()

print 'exit'