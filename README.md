PAM (Parallel Augmented Maps) is a parallel C++ library implementing the interface for augmented maps [1].  It is designed for maintaining an ordered map data structure while efficiently answering range-based and related queries.    In the experiments we
use the interface in four examples: augmented-sums, interval-queries, 2d range-queries, and an inverted index.    The released code includes the examples and scripts for running the specific experiments reported in the paper.   It is also designed so it is easy to try in many other scenarios (different sizes, different numbers of cores, and other operations described in the paper, but not reported in the experiments).

Usage:
To define an augmented map using PAM, user need to specify the parameters including type names and (static) functions in the entry structure ``entry''.

typename key_t: the key type (K),
function comp: K x K -> bool: the comparison function on K (<_K)
typename val_t: the value type (V),
typename aug_t: the augmented value type (A),
function from_entry: K x V -> A: the base function (g)
function combine: A x A -> A: the combine function (f)
function get_empty: empty -> A: the identity of f (I)

Then an augmented map is defined with C++ template as augmented_map<entry>.

Note that a plain ordered map is defined as an augmented map with no augmentation (i.e., it only has $K$, $<_K$ and $V$ in its entry) and a plain ordered set is similarly defined as an augmented map with no augmentation and no value type.

More details and examples can be found in our paper [1].


Experiment Workflow:
At the top level there is a makefile (make) and a script for compiling and running all timings (./run_all).  There are
four example applications provided in our repository:

* The range sum (in directory aug\_sum/).
* The interval tree (in directory interval/).
* The range tree (in directory range\_query/).
* The inverted indices (in directory index/).

In each of the directories there is a separated makefile and a script to run the timings for the corresponding application.

All tests include parallel and sequential running times.  The sequential versions are the algorithms running directly on one thread, and the parallel versions use all threads on the machine using ``numactl -i all''.