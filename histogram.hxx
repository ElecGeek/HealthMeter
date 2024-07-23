#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <math.h>
using namespace std;

class histogram_info {
  const vector<unsigned short> histo_vals{ 90, 87, 85, 80, 75 };
  const vector<unsigned short> extra_histo{ 95, 101 };
  string titles;
public:
  histogram_info()=delete;
  histogram_info(const bool&extra_thresholds);
  vector<unsigned short> concaten_histo;
  string_view GetTitles()const
  {
	return string_view(titles);
  }
  friend class histogram;
};

class histogram {
  deque< pair< unsigned short, unsigned long > >the_histo;
  unsigned long counter;
  unsigned long min_val;
  unsigned long max_val;
  unsigned long moy_val;
  unsigned char sample_rate;
  const histogram_info&histo_info;
public:
  histogram()=delete;
  histogram(const char&,const histogram_info&histo_info);
  histogram&operator<<=( unsigned short);
  friend ostream&operator<<(ostream&, const histogram&);
};

