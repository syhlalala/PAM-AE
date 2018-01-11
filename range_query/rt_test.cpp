#include "pbbs-include/get_time.h"
#include "pbbs-include/parse_command_line.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <climits>
#include <cstdlib>
#include <iomanip>

#include <cilk/cilk_api.h>
#include "range_tree.h"


using namespace std;

using data_type = int;
using point_type = Point<data_type, data_type>;
using tuple_type = tuple<data_type, data_type, data_type, data_type>;

struct Query_data {
  data_type x1, x2, y1, y2;
} query_data;

int str_to_int(char* str) {
  return strtol(str, NULL, 10);
}

int win;

int random_hash(int seed, int a, int rangeL, int rangeR) {
  if (rangeL == rangeR) return rangeL;
  a = (a+seed) + (seed<<7);
  a = (a+0x7ed55d16) + (a<<12);
  a = (a^0xc761c23c) ^ (a>>19);
  a = (a+0x165667b1) + (a<<5);
  a = ((a+seed)>>5) ^ (a<<9);
  a = (a+0xfd7046c5) + (a<<3);
  a = (a^0xb55a4f09) ^ (a>>16);
  a = a % (rangeR-rangeL);
  if (a<0) a+= (rangeR-rangeL);
  a+=rangeL;
  return a;
}

vector<point_type> generate_points(size_t n, data_type a, data_type b, data_type offset = 0) {
  vector<point_type> ret(n);

  cilk_for (size_t i = 0; i < n; ++i) {
    ret[i].x = random_hash('x'+offset, i, a, b);
    ret[i].y = random_hash('y'+offset, i, a, b);
    ret[i].w = random_hash('w', i, a, b);
  }

  return ret;
}

vector<Query_data> generate_queries(size_t q, data_type a, data_type b) {
  vector<Query_data> ret(q);

  
  cilk_for (size_t i = 0; i < q; ++i) {
    data_type x1 = random_hash('q'*'x', i*2, a, b);
    data_type y1 = random_hash('q'*'y', i*2, a, b);
	data_type dx = random_hash('d'*'x', i, 0, win);
    data_type dy = random_hash('d'*'y', i, 0, win);
    data_type x2 = x1+dx;
    data_type y2 = y1+dy;
    if (x1 > x2) {
      data_type t = x1; x1 = x2; x2 = t;
    }
    if (y1 > y2) {
      data_type t = y1; y1 = y2; y2 = t;
    }
    ret[i].x1 = x1; ret[i].x2 = x2;
    ret[i].y1 = y1; ret[i].y2 = y2;
  }

  return ret;
}

void reset_timers() {
  reserve_tm.reset();
  init_tm.reset(); sort_tm.reset(); build_tm.reset(); total_tm.reset(); 	
}

pair<double,double> run_all(vector<point_type>& points, size_t iteration,
	     data_type min_val, data_type max_val, size_t query_num) {
  RangeQuery<data_type, data_type> *r = new RangeQuery<data_type, data_type>(points);

  vector<Query_data> queries = generate_queries(query_num, min_val, max_val);
	
  timer t_query_total;
  t_query_total.start();
  cilk_for (size_t i = 0; i < query_num; i++) {
    sequence<pair<int,int>> out = r->query_range(queries[i].x1, queries[i].y1,
					       queries[i].x2, queries[i].y2);
  }
  
  t_query_total.stop();

  //if (iteration == 0) r->print_status();
  delete r;
  return make_pair(total_tm.get_total(), t_query_total.get_total());
}


pair<double,double> run_sum(vector<point_type>& points, size_t iteration, data_type min_val, data_type max_val, size_t query_num) {
  RangeQuery<data_type, data_type> *r = new RangeQuery<data_type, data_type>(points);

  vector<Query_data> queries = generate_queries(query_num, min_val, max_val);
	
  timer t_query_total;
  t_query_total.start();
  cilk_for (size_t i = 0; i < query_num; i++) {
    r->query_sum(queries[i].x1, queries[i].y1,
			     queries[i].x2, queries[i].y2);
  }

  t_query_total.stop();
  
  //if (iteration == 0) r->print_status();
  delete r;
  return make_pair(total_tm.get_total(), t_query_total.get_total());
}

int main(int argc, char** argv) {

  commandLine P(argc, argv,
		"./rt_test [-n size] [-l rmin] [-h rmax] [-r rounds] [-q queries] [-w window] [-t query_type]");
  size_t n = P.getOptionLongValue("-n", 100000000);
  int min_val = P.getOptionIntValue("-l", 0);
  int max_val = P.getOptionIntValue("-h", 1000000000);
  size_t iterations = P.getOptionIntValue("-r", 3);
  win = P.getOptionIntValue("-w", 1000000);
  size_t query_num = P.getOptionLongValue("-q", 1000);
  int type = P.getOptionIntValue("-t", 0);
  srand(2017);

  double* build_tm = new double[iterations];
  double* query_tm = new double[iterations];
  for (size_t i = 0; i < iterations; ++i) {
    vector<point_type> points = generate_points(n, min_val, max_val);
    if (type == 0) {
		pair<double, double> res = run_all(points, i, min_val, max_val, query_num);
		if (i>0) {
			build_tm[i-1] = res.first;
			query_tm[i-1] = res.second;
		}
	}
    if (type == 1) {
		pair<double, double> res = run_sum(points, i, min_val, max_val, query_num);
        if (i>0) {
			build_tm[i-1] = res.first;
			query_tm[i-1] = res.second;
		}
	}		
	reset_timers();
  }
    auto less = [] (double a, double b) {return a < b;};
	pbbs::sample_sort(build_tm,iterations,less);
	pbbs::sample_sort(query_tm,iterations,less);
	
	string benchmark_name = "Query-All";
	if (type == 1) benchmark_name = "Query-Sum"; 
  
  cout << "RESULT" << fixed << setprecision(3)
       << "\talgo=" << "RageTree"
       << "\tname=" << benchmark_name
       << "\tn=" << n
       << "\tq=" << query_num
       << "\tp=" << __cilkrts_get_nworkers()
       << "\twin-mean=" << win
       << "\tbuild-time=" << build_tm[iterations/2]
       << "\tquery-time=" << query_tm[iterations/2]
       << endl;

  return 0;
}

