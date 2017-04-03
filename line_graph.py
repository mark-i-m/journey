import numpy as np
import matplotlib.pyplot as plt
import matplotlib
from sys import argv

DATA = argv[1]
SCALE = argv[2] if len(argv) >= 3 else "linear"
WHICH = [int(w) for w in argv[3].split(",")] if len(argv) >= 4 else None

data = []

with open(DATA, 'r') as f:
    for line in f:
        if len(line.strip()) == 0:
            continue

        xs = [float(x) for x in line.split()]

        data.append(xs)

for i in range(len(data[0])) if WHICH is None else WHICH:
    ys = [x[i] - data[0][i] for x in data]
    line, = plt.plot(ys)

#plt.xscale("log", basex = 2)
plt.yscale(SCALE)

#plt.xlim(0, 512+32)
#plt.ylim(0, 600)

#plt.xlabel("Message Size (bytes)")
#plt.ylabel("Latency ($\mu$secs)")
plt.grid(True)

font = {'family' : 'normal',
        'weight' : 'bold',
        'size'   : 22}

matplotlib.rc('font', **font)

plt.show()
