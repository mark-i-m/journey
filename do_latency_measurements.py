from subprocess import Popen, PIPE
import os

def run_mmap_cont():
    print "cont run 1"
    os.system("./bench_time_per_op_mmap_16k_cont 20 0 > time_per_op_mmap_cont_1") 
    print "cont run 2"
    os.system("./bench_time_per_op_mmap_16k_cont 20 0 > time_per_op_mmap_cont_2") 
    print "cont run 3"
    os.system("./bench_time_per_op_mmap_16k_cont 20 0 > time_per_op_mmap_cont_3") 
    print "cont run 4"
    os.system("./bench_time_per_op_mmap_16k_cont 20 0 > time_per_op_mmap_cont_4") 
    print "cont run 5"
    os.system("./bench_time_per_op_mmap_16k_cont 20 0 > time_per_op_mmap_cont_5") 

    files = ["time_per_op_mmap_cont_1", "time_per_op_mmap_cont_2","time_per_op_mmap_cont_3",
            "time_per_op_mmap_cont_4","time_per_op_mmap_cont_5"]
    out_file_name = "time_per_op_mmap_cont.res"
    #merge_output(files, out_file_name)

def merge_output(files, file_name):
    dic = {}
    i = 0
    for f in files:
        with open(f)  as cur_file:
            for line in cur_file.readlines():
                if i not in dic:
                    dic[i] = []
                dic[i].append(line)
                i = i + 1
        i = 0

    f = open(file_name, "w")
    for k, v in dic.iteritems():
        for val in v:
            f.write(val.rstrip() + " ")
        f.write("\n")
    f.close()


def run_mmap_strided():
    print "strided run 1"
    os.system("./bench_time_per_op_mmap_16k_stride 20 0 > time_per_op_mmap_stride_1") 
    print "strided run 2"
    os.system("./bench_time_per_op_mmap_16k_stride 20 0 > time_per_op_mmap_stride_2") 
    print "strided run 3"
    os.system("./bench_time_per_op_mmap_16k_stride 20 0 > time_per_op_mmap_stride_3") 
    print "strided run 4"
    os.system("./bench_time_per_op_mmap_16k_stride 20 0 > time_per_op_mmap_stride_4") 
    print "strided run 5"
    os.system("./bench_time_per_op_mmap_16k_stride 20 0 > time_per_op_mmap_stride_5") 

    files = ["time_per_op_mmap_stride_1","time_per_op_mmap_stride_2", "time_per_op_mmap_stride_3", 
             "time_per_op_mmap_stride_4", "time_per_op_mmap_stride_5"]
    out_file_name = "time_per_op_mmap_stride.res"

    #merge_output(files, out_file_name)


def main():
    #run_mmap_cont()
    run_mmap_strided()

    return  ## don't wanna pollute the environment by compiling during the actual run!

    process = Popen(["make", "clean"], stdout=PIPE)
    (output, err) = process.communicate()
    exit_code = process.wait()
    
    print output

    print "Compiling Source Files..."

    process = Popen(["make"], stdout=PIPE)
    (output, err) = process.communicate()
    exit_code = process.wait()

    if exit_code != 0:
        print "Compile Failed.."
        return
    print "Successfully Compiled..."

    run_mmap_cont()
    run_mmap_strided()

    
    



if __name__ == "__main__":
    main()
