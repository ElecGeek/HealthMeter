#include "histogram.hxx"

histogram_info::histogram_info(const bool&extra_thresholds)
{
  if (extra_thresholds)
	{
	  concaten_histo.reserve(histo_vals.size()+extra_histo.size());
	  copy( histo_vals.begin(),histo_vals.end(),back_inserter(concaten_histo));
	  copy( extra_histo.begin(),extra_histo.end(),back_inserter(concaten_histo));
	}else{
	  concaten_histo.reserve(histo_vals.size());
	  copy( histo_vals.begin(),histo_vals.end(),back_inserter(concaten_histo));	
  }  
  titles.reserve( 30 + 7 * concaten_histo.size());
  titles += " dur.   moy    min    max ";
  stringstream strstm;
  strstm.precision(1);
  strstm.setf( ios_base::fixed, ios_base::floatfield );
  for( unsigned short val : concaten_histo )
	{
	  strstm << "  <" << ((float)val) << "%";

	  titles += strstm.str();
	  strstm.str("");
	}
}


histogram::histogram(const char&sample_time,
					 const histogram_info&histo_info,
					 const optional<const unsigned short>&delay_start):
  samples_counter(0),
  threshold_counter(0), threshold_high( true ),
  min_val( numeric_limits< decltype( min_val )>::max()),moy_val(0),max_val(0),
  sample_time(sample_time),
  histo_info(histo_info),
  delay_start(delay_start){
  for( unsigned short val : histo_info.concaten_histo )
	the_histo.push_back( make_pair( 10 * val, 0 ));
}
histogram&histogram::operator<<=( unsigned short val){
  if ( delay_start )
	{
	  //	  cout<<val<<" ";
	  if( *delay_start > val )
		delay_start = nullopt;
	  else
		return*this;
	} 
  // threshold counting
  if ( threshold_high && val < 90 )
	{
	  threshold_high = false;
	  threshold_counter += 1;
	}
  else if ( threshold_high == false && val > 90 )
	threshold_high = true;
  // Now the histogram
  for_each( the_histo.begin(), the_histo.end() , [&val]( auto & N){
	  if ( N.first > ( val * 10 ) ) N.second++;} );
  if ( ( val * 10 ) < min_val )
	min_val = val * 10;
  if ( ( val * 10 ) > max_val )
	max_val = val * 10;
  moy_val += val * 10;
  //  cout << val << "  ";
  samples_counter++;
  return*this;
};
ostream&operator<<(ostream&os, const histogram&histo){
  os << setfill(' ') << setw(3) << (histo.sample_time * histo.samples_counter ) / 60 << "'";
  os << setfill('0') << setw(2) << histo.sample_time * histo.samples_counter - 60 * (histo.sample_time * histo.samples_counter / 60 ) << "  ";
  os.precision(1);
  os.setf( ios_base::fixed, ios_base::floatfield );
  os << round((float)histo.moy_val/(float)histo.samples_counter) / 10.0 <<"%  ";
  os << ((float)histo.min_val)/10.0<<"%  ";
  os << ((float)histo.max_val)/10.0<<"%";
  float the_val;
  for_each( histo.the_histo.begin(), histo.the_histo.end(), [&](auto&iter)
	{
	  the_val = round( 1000.0 * (float)iter.second/(float)histo.samples_counter ) / 10.0;
	  os << "   ";
	  if ( the_val != 0.0 )
		os << setfill(' ') << setw(4) << the_val << "%";
	  else
		os << " /   ";
    } );
  os << "  " << setfill(' ') << setw(3) << histo.threshold_counter;
  return os;
}
