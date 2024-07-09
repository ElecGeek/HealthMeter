#include "histogram.hxx"


histogram::histogram(const char&sample_rate, const bool&extra_thresholds):
  counter(0),
  min_val( numeric_limits< decltype( min_val )>::max()),moy_val(0),max_val(0),
  sample_rate(sample_rate),
  extra_thresholds(extra_thresholds) {
  for( unsigned short val : histo_vals )
	the_histo.push_back( make_pair( 10 * val, 0 ));
  if ( extra_thresholds)
	for( unsigned short val : extra_histo )
	  the_histo.push_back( make_pair( 10 * val, 0 ));
}
histogram&histogram::operator<<=( unsigned short val){
  for_each( the_histo.begin(), the_histo.end() , [&val]( auto & N){
	  if ( N.first > ( val * 10 ) ) N.second++;} );
  if ( ( val * 10 ) < min_val )
	min_val = val * 10;
  if ( ( val * 10 ) > max_val )
	max_val = val * 10;
  moy_val += val * 10;
  //  cout << val << "  ";
  counter++;
  return*this;
};
ostream&operator<<(ostream&os, const histogram&histo){
  // assume a sample rate of 1S
  os << setfill(' ') << setw(2) << (histo.sample_rate * histo.counter ) / 60 << "min  ";
  os.precision(1);
  os.setf( ios_base::fixed, ios_base::floatfield );
  os << "moy:" << round((float)histo.moy_val/(float)histo.counter) / 10.0 <<"%  ";
  os << "min:" << ((float)histo.min_val)/10.0<<"%  ";
  os << "max:" << ((float)histo.max_val)/10.0<<"%  ";
  float the_val;
  for_each( histo.the_histo.begin(), histo.the_histo.end(), [&](auto&iter)
	{
	  the_val = round( 1000.0 * (float)iter.second/(float)histo.counter ) / 10.0;
	  os << "\t<" << ((float)iter.first)/10.0 << "%:";
	  if ( the_val != 0.0 )
		os << setfill(' ') << setw(4) << the_val << "%";
	  else
		os << " /   ";
    } );
  return os;
}
