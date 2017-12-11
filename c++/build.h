#pragma once
#include "pbbs-include/get_time.h"
#include "pbbs-include/sequence_ops.h"
#include "pbbs-include/sample_sort.h"

template <class Entry>
struct build {
  using K = typename Entry::key_t;
  using V = typename Entry::val_t;
  using ET = typename Entry::entry_t;

  static sequence<ET>
  sort_remove_duplicates(sequence<ET> A,
			 bool seq_inplace = false) {
    auto less = [&] (ET a, ET b) {
      return Entry::comp(Entry::get_key(a), Entry::get_key(b));};
    if (A.size() == 0) return sequence<ET>(0);
    if (seq_inplace) {
      pbbs::quicksort(A.as_array(), A.size(), less);

      // remove duplicates
      size_t  j = 1;
      for (size_t i=1; i<A.size(); i++)
	if (less(A[j-1], A[i])) A[j++] = A[i];
      return A.slice(0,j);
    } else {
      //t.start();
      auto B = pbbs::sample_sort(A, less);
      //std::cout << t.get_next() << std::endl;
      sequence<bool> Fl(B.size(), [&] (size_t i) {
	  return (i==0) || less(B[i-1], B[i]); });
      //std::cout << t.get_next() << std::endl;
      auto C = pbbs::pack(B, Fl);
      //std::cout << t.get_next() << std::endl;
      return C;
    }
  }
        
  template<class Vi, class Reduce>
  static sequence<ET>
  sort_reduce_duplicates(sequence<std::pair<K,Vi>> A,
			 const Reduce& reduce) {
    using E = std::pair<K,Vi>;
    size_t n = A.size();
    if (n == 0) return sequence<ET>(0);

    auto lessE = [&] (E a, E b) {
      return Entry::comp(a.first, b.first);};

    //t.start();
    sequence<E> B = pbbs::sample_sort(A, lessE);
    //std::cout << "time sort: " << t.stop() << std::endl;
    sequence<bool> Fl(n);
	  
    // determines the index of start of each block of equal keys
    // and copies values into vals
    sequence<Vi> Vals(n, [&] (size_t i) {
	Fl[i] = (i==0) || lessE(B[i-1], B[i]);
	return B[i].second;
      });

    sequence<node_size_t> I = pbbs::pack_index<node_size_t>(Fl);

    // combines over each block of equal keys using function reduce
    auto set = [&] (size_t i) {
      size_t start = I[i];
      size_t end = (i==I.size()-1) ? n : I[i+1];
      return ET(B[start].first, reduce(Vals.slice(start,end)));
    };

    auto a =  sequence<ET>(I.size(), set);
    
    // tabulate set over all entries of i
    return a;
  }

  template<class Bin_Op>
  static sequence<ET>
  sort_combine_duplicates(sequence<ET> A,
			  Bin_Op& f,
			  bool seq_inplace=false) {
    auto less = [&] (ET a, ET b) {return Entry::comp(a.first, b.first);};
    if (A.size() == 0) return sequence<ET>(0);
    if (seq_inplace) {
      pbbs::quicksort(A.as_array(), A.size(), less);
      size_t j = 0;
      for (size_t i=1; i < A.size(); i++) {
	if (less(A[j], A[i])) A[++j] = A[i];
	else A[j].second = f(A[j].second,A[i].second);
      }
      return A.slice(0,j+1);
    } else {
      auto reduce_op = [&] (sequence<V> S) { return pbbs::reduce(S, f);};
      return sort_reduce_duplicates(A, reduce_op);
    }
  }      
};
