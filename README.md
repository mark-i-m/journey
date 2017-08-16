# Journey

This is the repo for our CS736 project, including microbenchmarks, results,
figures, and paper.

To build all benchmarks, use `make` (I recommend also using `-j`).

Here is a quick list of the contents:

- Microbenchmarks: A bunch of C++ files. The naming convention is `bench_`,
  followed by what metric is being measured (`cpu`, `time`, `mem`,
  `ops_per_sec`), followed by what operation/subsystem is being benchmarked
  (`kswapd`, `mmap`), followed by what workload is being run (the easiest thing
  is to read the code to see what pattern is used).
    - `bench_cpu_kswapd_anon.cc`
    - `bench_cpu_kswapd_anon_rand.cc`
    - `bench_measure_cpu_util_cpu.cc`
    - `bench_mem_control.cc`
    - `bench_mem_mmap_16k_cont.cc`
    - `bench_mem_mmap_16k_cont_rwx_prot_4k_rdonly.cc`
    - `bench_mem_mmap_16k_cont_rwx_prot_4k_rdonly_touch.cc`
    - `bench_mem_mmap_16k_cont_touch.cc`
    - `bench_mem_mmap_4k_rand.cc`
    - `bench_mem_mmap_anon_rss.cc`
    - `bench_mem_mmap_anon_rss_rand.cc`
    - `bench_mem_mmap_frag.cc`
    - `bench_mem_mmap_stride.cc`
    - `bench_ops_per_sec_mmap_16k_cont_touch.cc`
    - `bench_ops_per_sec_mmap_4k_cont_touch.cc`
    - `bench_time_mmap_anon_rss.cc`
    - `bench_time_mmap_anon_rss_rand.cc`
    - `bench_time_per_op_mmap_16k_cont.cc`
    - `bench_time_per_op_mmap_16k_stride.cc`
    - `bench_time_per_op_mmap_4k_rand.cc`
    - `bench_time_per_op_mmap_frag.cc`
    - `bench_time_per_op_mmap_small_remap_large.cc`

- Supporting files
    - `pattern.cc`, `pattern.h` - Helpers for producing patterns of memory accesses
    - `pinning.h` - Helpers for pinning workloads to processor cores
    - `rdmemusg.cc`, `rdmemusg.h` - Helpers for reading/computing total system memory usage
    - `rdtsc.cc`, `rdtsc.h` - Helpers for measuring time elapsed
    - `makefile` - Builds all benchmarks
    - `offsets.txt` - A file containing a bunch of non-repeating random integers

- Scripts
    - `do_latency_measurements.py` - Runs a bunch of latency-related microbenchmarks
    - `run_tests.py` - Runs benchmarks specified in a global variable. Be
      careful when using this, as the python interpretter could affect results
      for benchmarks that measure global system memory usage.
    - `scaling_set_freq.sh` - Set the frequency of the processor and scaling governors

    - Scripts for analyzing results and generating graphs. Most of these use `matplotlib` or
          `scipy`. Read the script to see if it suits your purposes.
        - `gen_bar.py`
        - `line_graph.py`
        - `line_graph_with_control.py`
        - `time_cdf.py`
        - `medians.py`

- Reports
    - `paper.tex` - Our final report
    - `proposal.tex` - Our original idea proposal
    - `related_work.tex` - Our original related work section
    - `references.bib` - References for all reports
    - `results/` - Final results (raw numbers)
    - `figures/` - Images used in `paper.tex`
