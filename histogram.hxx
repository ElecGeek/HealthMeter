#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <math.h>
using namespace std;

class histogram {
  const vector<unsigned short> histo_vals{ 90, 85, 80, 75 };
  const vector<unsigned short> extra_histo{ 95, 101 };
  deque< pair< unsigned short, unsigned long > >the_histo;
  unsigned long counter;
  unsigned long min_val;
  unsigned long max_val;
  unsigned long moy_val;
  unsigned char sample_rate;
  bool extra_thresholds;
public:
  histogram()=delete;
  histogram(const char&,const bool&extra_thresholds);
  histogram&operator<<=( unsigned short);
  friend ostream&operator<<(ostream&, const histogram&);
};

