import numpy as np
import matplotlib.pyplot as plt
import matplotlib
from sys import argv, exit

if len(argv) < 5:
    print("Usage: ./script WHICH AXIS_X AXIS_Y FILE [FILE, FILE, ...]")
    print("WHICH = <column #>[,<column #>,...]")
    print("AXIS_X = <x axis label>")
    print("AXIS_Y = <y axis label>")
    exit(-1)

WHICH = [int(x) for x in argv[1].split(',')]
AXIS_X = argv[2]
AXIS_Y = argv[3]

data = {}

def running_mean(l, N):
    sum = 0
    result = list( 0 for x in l)

    for i in range( 0, N ):
        sum = sum + l[i]
        result[i] = sum / (i+1)

    for i in range( N, len(l) ):
        sum = sum - l[i-N] + l[i]
        result[i] = sum / N

    return result

for data_file in argv[4:]:
    with open(data_file, 'r') as f:
        data[data_file] = []

        for line in f:
            if len(line.strip()) == 0:
                continue

            xs = [float(x)/2.4E6 for x in line.split()]
            #xs = [float(x) for x in line.split()]

            data[data_file].append(xs)

handles = []

for data_file in sorted(data.keys()):
    for i in WHICH:
        if i < len(data[data_file][0]):
            #ys = [x[i] - data[data_file][0][i] for x in data[data_file]]
            # uncomment next line to prevent normalizing to X_0
            #ys = running_mean([x[i] for x in data[data_file]], 5000)
            ys = [x[i] for x in data[data_file]]
            counts, bin_edges = np.histogram(ys, bins = int(max(ys)* 2.4E6/500), weights = ys)
            cdf = np.cumsum(counts) / sum(ys) * 100.0
            #xs = [float(i) / 1E3 for i in range(len(ys))]
            line, = plt.plot(bin_edges[:-1], cdf, label = data_file)
            #line, = plt.plot(cdf, label = data_file)
            handles.append(line)

#plt.xscale("log", basex = 2)
#plt.yscale(SCALE)

#plt.xlim(0, 512+32)
#plt.ylim(0, 3)

plt.xlabel(AXIS_X)
plt.ylabel(AXIS_Y)
plt.grid(True)

plt.legend(handles = handles, loc='best')

font = {'family' : 'normal',
        'weight' : 'bold',
        'size'   : 22}

matplotlib.rc('font', **font)

plt.show()
