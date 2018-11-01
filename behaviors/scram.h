#define MMDR_DYNA 1
#define MMDR_N5 2
#define MMDR_N4 3
#define MMD_MSD2 4

// options for simulation

#define REPETITIONS 1
//#define ALGORITHM MMDR_DYNA
//#define ALGORITHM MMDR_N5
#define ALGORITHM MMDR_N4
//#define ALGORITHM MMD_MSD2
// #define N 10
#define MAXN 20


#if ALGORITHM == MMDR_DYNA
#define SOLVER mmdr_dyna
#elif ALGORITHM == MMDR_N5
#define SOLVER mmdr_n5
#elif ALGORITHM == MMDR_N4
#define SOLVER mmdr_n4
#elif ALGORITHM == MMD_MSD2
#define SOLVER mmd_msd2
#endif // ALGORITHM


#include<iostream>
#include<fstream>
#include<cstdio>
#include<cmath>
#include<string>
#include<algorithm>
#include<vector>
#include<climits>
#include<float.h>
#include<string.h>

#include <boost/cstdint.hpp>

typedef uint64_t uint64;

// remove an element from a vector by value.
#define VECREMOVE(vec, v) (vec).erase(  \
              std::remove((vec).begin(), (vec).end(), (v)), (vec).end())


typedef std::pair<double, double> Point;

// Edge looks like (dist, (left node, right node))
typedef std::pair<double, std::pair<int, int> > Edge;

struct Test{
  std::vector<Point> starts;
  std::vector<Point> targets;
};

inline double getdist(const Point &a, const Point &b){
  return hypot(a.first-b.first, a.second-b.second);
}

inline bool edgeVectorLessThan(const std::vector<Edge> &a, const std::vector<Edge> &b) {
  const double ERROR_THRESH = .00005;
  for (int i = 0; i < a.size(); i++) {
    if (std::abs(a[i].first-b[i].first) < ERROR_THRESH) {
      continue;
    }
    return a[i] < b[i];
  }
  return false;
}

////////////  Begin implementation of getMinimalMaxEdgeInPerfectMatching  ////////////////

#if ALGORITHM == MMDR_N4 || ALGORITHM == MMD_MSD2

int flood(int x, int y, int prev);
int getMinimalMaxEdgeInPerfectMatching(std::vector<Edge> edges, int n, int k);

#endif // ALGORITHM == MMDR_N4 || ALGORITHM == MMD_MSD2

////////////  End implementation of getMinimalMaxEdgeInPerfectMatching  ////////////////

////////////  Begin implementation of mmdr_dyna  ////////////////

#if ALGORITHM == MMDR_DYNA

std::vector<Edge> mmdr_dyna(Test t);

#endif // ALGORITHM == MMDR_DYNA

////////////  End implementation of mmdr_dyna  //////////////

////////////  Begin implementation of mmdr_n5  ////////////////

#if ALGORITHM == MMDR_N5

#define DATA_64WORDS_SIZE (MAXN*MAXN-1)/64+2

// -- Start functions for opeations on 64 bit values --
void subtract_value(uint64 (&valueA)[DATA_64WORDS_SIZE], const uint64 (&valueB)[DATA_64WORDS_SIZE]);
// -- End functions for operations on 64 bit values --

void init_labels_large();
void update_labels_large();
void add_to_tree_large(int x, int prevx);
bool augment_large();
void hungarian_large();
std::vector<Edge> mmdr_n5(Test t);

#endif // ALGORITHM == MMDR_N5

////////////  End implementation of mmdr_n5  ////////////////

////////////  Begin implementation of mmdr_n4  ////////////////

#if ALGORITHM == MMDR_N4

#define INF 1e16    //just infinity

void init_labels_int();
void update_labels_int();
void add_to_tree_int(int x, int prevx);
bool augment_int();
int hungarian_int();

std::vector<Edge> mmdr_n4(Test t);

#endif // ALGORTHM == MMDR_N4

////////////  End implementation of mmdr_n4  ////////////////

////////////  Begin implementation of mmd_msd2 solution  ////////

#if ALGORITHM == MMD_MSD2

#define INF 1e16    //just infinity

void init_labels();
void update_labels();
void add_to_tree(int x, int prevx);
bool augment();
void hungarian();
std::vector<Edge> mmd_msd2(Test t);

#endif // ALGORITHM == MMD_MSD2

////////////  End implementation of mmd_msd2  ////////////////
