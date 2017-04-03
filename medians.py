import numpy as np
from sys import argv, exit

if len(argv) < 4:
    print("Usage: ./script WHICH OUTPUT_FILE FILE [FILE, FILE, ...]")
    print("WHICH = <column #>[,<column #>,...]")
    exit(-1)

WHICH = [int(x) for x in argv[1].split(',')]
OUTPUT_FILE = argv[2]

data = {}

medians = []

for data_file in argv[3:]:
    with open(data_file, 'r') as f:
        data[data_file] = []

        for line in f:
            if len(line.strip()) == 0:
                continue

            xs = [float(x) for x in line.split()]

            data[data_file].append(xs)

for row in range(len(data.values()[0])):
    medians_row = []
    for i in WHICH:
        vals = [data[data_file][row][i] - data[data_file][0][i] for data_file in data]
        medians_row.append(np.median(vals))
    medians.append(medians_row)

with open(OUTPUT_FILE, 'w') as f:
    for median_row in medians:
        f.write("%s\n" % " ".join([str(x) for x in median_row]))

    f.flush()

