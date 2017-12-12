# PAM

PAM (Parallel Augmented Maps) is a parallel C++ library implementing the interface for augmented maps [1].  It is designed for maintaining an ordered map data structure while efficiently answering range-based and related queries.    In the experiments we
use the interface in four examples: augmented-sums, interval-queries, 2d range-queries, and an inverted index.    The released code includes the examples and scripts for running the specific experiments reported in the paper.   It is also designed so it is easy to try in many other scenarios (different sizes, different numbers of cores, and other operations described in the paper, but not reported in the experiments).

## Usage:

To define an augmented map using PAM, user need to specify the parameters including type names and (static) functions in the entry structure ``entry''.

* typename key_t: the key type (K),
* function comp: K x K -> bool: the comparison function on K (<_K)
* typename val_t: the value type (V),
* typename aug_t: the augmented value type (A),
* function from_entry: K x V -> A: the base function (g)
* function combine: A x A -> A: the combine function (f)
* function get_empty: empty -> A: the identity of f (I)

Then an augmented map is defined with C++ template as augmented_map<entry>.

Note that a plain ordered map is defined as an augmented map with no augmentation (i.e., it only has $K$, $<_K$ and $V$ in its entry) and a plain ordered set is similarly defined as an augmented map with no augmentation and no value type.

More details and examples can be found in our paper [1].

## Hardware dependencies

Any modern (2010+) x86-based multicore machines.  Relies on 128-bit CMPXCHG (requires -mcx16 compiler flag) but does not need hardware transactional memory (TSX).  Most examples given in our scripts require 64GB memory, but range_query requires 256GB memory and aug_sum on the large input requires 1TB memory.  All the examples can take smaller input size by setting command line arguments.

## Software dependencies
PAM requires g++ 5.4.0 or later versions supporting the Cilk Plus extensions.    The scripts that we provide in the repository use \texttt{numactl} for better performance. All tests can also run directly without "numactl".

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

## Reference

[1] Sun, Yihan, Daniel Ferizovic, and Guy E. Blelloch. PAM: Parallel Augmented Maps. PPoPP 2018. To appear.