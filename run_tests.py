#! /usr/bin/python

import csv
import os
import subprocess
import shlex

# Assume that the test executables already exist
tests = [
        "large_mmap"
]

mmap_sizes = [i for i in range(10, 30)]

output_dir = os.path.join(os.getcwd(), "output")

def average(results):
    return sum(results) * 1./ (len(results))

def run_cmd(cmd):
    return subprocess.check_output(shlex.split(cmd))

def run_test(test_name, mmap_size, iterations=100):
    test_script = os.path.join(os.getcwd(), test_name)
    cmd = "%s %d" % (test_script, mmap_size)

    results = []
    for _ in xrange(iterations):
        output = run_cmd(cmd)
        results += [float(output)]
    return results

def dump_results(test_name, results):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    filename = '%s.csv' % os.path.join(output_dir, test_name)
    with open(filename, 'wb') as csvfile:
        writer = csv.writer(csvfile)
        for row in results:
            writer.writerow(row)

def run_latency_tests():
    print "LATENCY RESULTS"
    for test in tests:
        total_results = []
        for mmap_size in mmap_sizes:
            results = run_test(test, mmap_size)

            print "%20s %7d: %s" % (test, mmap_size, average(results))

            for result in results:
                total_results += [[mmap_size, result]]

        dump_results("latency_" + test, total_results)
        print
    print

if __name__ == "__main__":
    run_latency_tests()
