#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <math.h>
#include <optional>
using namespace std;


// Common class containing the thresholds for the analyse of all the input files
class histogram_info {
  const vector<unsigned short> histo_vals{ 91, 90, 89, 80 };
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

// Histogram of one record of one input file
// It produces the length, the minimum, the maximum, the average
//   and the percent of the time the values are under
//  each threshold defined in the info class.
class histogram {
  deque< pair< unsigned short, unsigned long > >the_histo;
  unsigned long counter;
  unsigned long min_val;
  unsigned long max_val;
  unsigned long moy_val;
  // Seconds for one sample
  unsigned char sample_time;
  const histogram_info&histo_info;
  optional< const unsigned short>delay_start;
public:
  histogram()=delete;
  histogram(const char&sample_time,
			const histogram_info&histo_info,
			const optional< const unsigned short>&delay_start=nullopt);
  histogram&operator<<=( unsigned short);
  friend ostream&operator<<(ostream&, const histogram&);
};

