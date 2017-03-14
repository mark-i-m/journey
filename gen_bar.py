import numpy as np
import matplotlib.pyplot as plt
import matplotlib

DATA = 'output/latency_large_mmap.csv'

data = {i: [] for i in range(10,30)}

with open(DATA, 'r') as f:
    for line in f:
        if len(line.strip()) == 0:
            continue

        size = int(line.split(",")[0])
        lat = float(line.split(",")[1])

        data[size].append(float(lat))

def median(vals):
    return sorted(vals)[len(vals) / 2]

labels = data.keys()
xs = [median(data[l]) for l in labels]

print xs

ind = np.arange(len(labels))  # the x locations for the groups
width = 0.7       # the width of the bars

fig, ax = plt.subplots()
rects1 = ax.bar(ind + ((1 - width) / 2.0), xs, width, color='k')

# add some text for labels, title and axes ticks
ax.set_ylabel('Latency (cyc)')
ax.set_xticks(ind + width / 2 + (1 - width) / 2)
ax.set_xticklabels(labels)

font = {'family' : 'normal',
        'weight' : 'bold',
        'size'   : 22}

matplotlib.rc('font', **font)

plt.grid(True)

plt.show()
