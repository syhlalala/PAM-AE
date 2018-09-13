# PAM

```diff
+ A more recent version of the PAM library is available at https://github.com/cmuparlay/PAM
```

Note: this is the version submitted to PPoPP Artifact Evaluation. A more recent version of the PAM library is available at [here](https://github.com/cmuparlay/PAM).

PAM (Parallel Augmented Maps) is a parallel C++ library implementing the interface for augmented maps [1].  It is designed for maintaining an ordered map data structure while efficiently answering range-based and related queries.    In the experiments we use the interface in four examples: augmented-sums, interval-queries, 2d range-queries, and an inverted index.    The released code includes the examples and scripts for running the specific experiments reported in the paper.   It is also designed so it is easy to try in many other scenarios (different sizes, different numbers of cores, and other operations described in the paper, but not reported in the experiments).

More details and examples can be found in our paper [1].

## Usage:

To define an augmented map using PAM, user need to specify the parameters including type names and (static) functions in the entry structure ``entry''.

* typename key_t: the key type (K),
* function comp: K x K -> bool: the comparison function on K (<_K)
* typename val_t: the value type (V),
* typename aug_t: the augmented value type (A),
* function from_entry: K x V -> A: the base function (g)
* function combine: A x A -> A: the combine function (f)
* function get_empty: empty -> A: the identity of f (I)

Then an augmented map is defined with C++ template as 

```
augmented_map<entry>.
```

Note that a plain ordered map is defined as an augmented map with no augmentation (i.e., it only has K, <_K and V in its entry) and a plain ordered set is similarly defined as an augmented map with no augmentation and no value type.

Here is an example of defining an augmented map "m" that has integer keys and values and is augmented with value sums (similar as the augmented sum example in our paper [1]):

```
struct entry {
  using key_t = int;
  using val_t = int;
  using aug_t = int;
  static bool comp(key_t a, key_t b) { 
    return a < b;}
  static aug_t get_empty() { return 0;}
  static aug_t from_entry(key_t k, val_t v) { 
    return v;}
  static aug_t combine(aug_t a, aug_t b) { 
    return a+b;}};
augmented_map<entry> m;
```

Another quick example can be found in [1], which shows how to implement an interval tree using the PAM interface.

## Hardware dependencies

Any modern (2010+) x86-based multicore machines.  Relies on 128-bit CMPXCHG (requires -mcx16 compiler flag) but does not need hardware transactional memory (TSX).  Most examples given in our scripts require 64GB memory, but range_query requires 256GB memory and aug_sum on the large input requires 1TB memory.  All the examples can take smaller input size by setting command line arguments.

## Software dependencies
PAM requires g++ 5.4.0 or later versions supporting the Cilk Plus extensions.    The scripts that we provide in the repository use "numactl" for better performance. All tests can also run directly without "numactl".

We use python to write a script to organize all results and compute speedup. It is not required to run tests.

## Datasets
We use the publicly available Wikipedia database (dumped on Oct. 1, 2016) for the inverted index experiment.  We release a sample (1% of total size) in the github repository (35MB compressed).  The full data (3.5TB compressed) is available on request.  All other applications use randomly generated data.


## Experiment Workflow:

At the top level there is a makefile (make) and a script for compiling and running all timings (./run_all).  The source code
of the library is provided in the directory c++/, and the other directories
each corresponds to some examples of applications. There are
four example applications provided in our repository:

* The range sum (in directory aug_sum/).
* The interval tree (in directory interval/).
* The range tree (in directory range_query/).
* The inverted indices (in directory index/).

In each of the directories there is a separated makefile and a script to run the timings for the corresponding application.

All tests include parallel and sequential running times.  The sequential versions are the algorithms running directly on one thread, and the parallel versions use all threads on the machine using "numactl -i all".

To run all tests, just type the following at the top level:

```
make
./run_all.sh
```

By default the script will not include the tests on very large input sizes (10 billion), which costs long. Users can use 

```
make
./run_all.sh -l
```

to include large tests.

To run separated tests on each application, users can also go to each sub-directory to run the scripts.

We recommend to use numactl -i all on all parallel tests.

### Augmented Sum (/aug_sum/)

Using

```
make
```

will give two executable files aug_sum (augmented version) and aug_sumNA (non-augmented version). This is done by setting flag -DNO_AUG in compiling time.

Using 

```
./run_aug_sum.sh -l
```

will run all experiments as shown in Table 3 in [1] on augmented sum. If you do not want to run on large input, remove "-l".

Using 

```
./runall [-r rounds] [-p threads]
```

will run all functions as shown in Table 3 in [1] with 'rounds' rounds and 'threads' threads. By default rounds=3 and threads=`nproc --all` (maximum number of threads on the machine).

Both scripts will output to both stdout and a file res.txt. The script run_aug_sum.sh will then call a python code to give all results (timings and speedups) in a file data.txt.

If user wants to directly run our executable file (aug_sum or aug_sumNA), the arguments are listed as follows:

```
./aug_sum [-n size1] [-m size2] [-r rounds] [-p] <testid>
```

### Interval Tree (/interval/)

Using 

```
make
```

will give the executable file (interval).

Using

```
run_interval
```
will give the same experiment of interval trees as shown in Table 5 in [1]. 

To directly run the executable file (interval), one can try:

```
./interval n q r
```

where n stands for the number of intervals, q is the number of querys, r is the number of rounds. By default n=100000000, q=n, r=5.

### Range Tree (/range_query/)

Using 

```
make
```

will give the executable file (rt_test).

Using 

```
./run_range 
```

will give the same experiment of range trees as shown in Table 5 in [1]. 

To directly run the executable file (rt_test), one can try:

```
./rt_test [-n size] [-l rmin] [-h rmax] [-r rounds] [-q queries] [-w window] [-t query_type]
```

where 'size' stands for the number of points, 'rmin' and 'rmax' are the upper and lower bound of coordinates, 'rounds' is the number of rounds, 'queries' is the number of queries, 'window' is the query window size (for one dimension), 'query_type' is 0 for query-all, and 1 for query-ssum. By default n=100000000, l=0, h=1000000000, r=3, q=1000, w=1000000, t=0.


### Inverted Index
Using 

```
make
```

will give the executable file (index).

Using 

```
./run_index
```

will give the same type of experiment of inverted index as shown in Table 6 in [1], but on a smaller input size.

To directly run the executable file (index), one can try:

```
./index [-o] [-v] [-n max_chars] [-q num_queries] [-f file]
```

where '-o' indicates an output file of query results, '-v' means to output verbose information, '-n' means the length to read from a file, '-q' is the number of queries, and '-f' is the input file. By default n=1000000000000 (just read the whole file), q=10000, f='wiki_small.txt'.

## Reference

[1] Yihan Sun, Daniel Ferizovic, and Guy E. Blelloch. PAM: Parallel Augmented Maps. PPoPP 2018. 
