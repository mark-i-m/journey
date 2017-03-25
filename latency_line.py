import numpy as np
import matplotlib.pyplot as plt
import matplotlib
from sys import argv

DATA = argv[1]
SCALE = argv[2] if len(argv) == 3 else "log"

data = []

with open(DATA, 'r') as f:
    for line in f:
        if len(line.strip()) == 0:
            continue

        lat = float(line)

        data.append(lat)

line_pipe, = plt.plot(data)

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
