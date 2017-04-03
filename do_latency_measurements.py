from subprocess import Popen, PIPE
import os

def run_mmap_cont():
    print "cont run 1"
    os.system("./bench_time_per_op_mmap_cont 20 0 >> time_per_op_mmap_cont_1") 
    print "cont run 2"
    os.system("./bench_time_per_op_mmap_cont 20 0 >> time_per_op_mmap_cont_2") 
    print "cont run 3"
    os.system("./bench_time_per_op_mmap_cont 20 0 >> time_per_op_mmap_cont_3") 
    print "cont run 4"
    os.system("./bench_time_per_op_mmap_cont 20 0 >> time_per_op_mmap_cont_4") 
    print "cont run 5"
    os.system("./bench_time_per_op_mmap_cont 20 0 >> time_per_op_mmap_cont_5") 

def run_mmap_strided():
    print "strided run 1"
    os.system("./bench_time_per_op_mmap_stride 20 0 >> time_per_op_mmap_stride_1") 
    print "strided run 2"
    os.system("./bench_time_per_op_mmap_stride 20 0 >> time_per_op_mmap_stride_2") 
    print "strided run 3"
    os.system("./bench_time_per_op_mmap_stride 20 0 >> time_per_op_mmap_stride_3") 
    print "strided run 4"
    os.system("./bench_time_per_op_mmap_stride 20 0 >> time_per_op_mmap_stride_4") 
    print "strided run 5"
    os.system("./bench_time_per_op_mmap_stride 20 0 >> time_per_op_mmap_stride_5") 



def main():
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

    
    



if __name__ == "__main__":
    main()
