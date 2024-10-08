#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <string_view>
#include <vector>
#include <deque>
#include <map>
#include <algorithm>
#include <functional>
#include <limits>
#include <math.h>
using namespace std;

#include "date_time.hxx"
#include "histogram.hxx"

// Processes one record
// The data is normalised for audio editing software
// The histogram is processed and sent to the standard output
void send_bloc( ostream&theFile, basic_string_view<unsigned char> & bloc_sv, ASCII_date_time &the_date_time, const histogram_info&histo_info )
{
  //  cout << hex << (unsigned short)bloc_sv[0] << "\t" << hex << bloc_sv.length() << endl;
  /*  if ( 1==2 )
	{
	  if ( bloc_sv.length() > 50 )
		{
		  basic_string_view<unsigned char> body = bloc_sv.substr( 28, 20 );
		  for( auto&& s : body )
			cout << (unsigned short)s << ' ';
		}
		}else{*/
	  if ( bloc_sv.length() > 28 )
		{
		  basic_string_view<unsigned char> header = bloc_sv.substr( 0, 28 );
		 
		  // There is no different treatment between the two channels
		  basic_string_view<unsigned char> body = bloc_sv.substr( 28, basic_string_view<unsigned char>::npos );
		  bool odd_even = false;
		  unsigned short cnv_val_s;
		  char cnv_val_8;
		  histogram*histo;
		  // Pass the sampling rate to convert the samples number to minutes
		  histo = new histogram(header[14], histo_info);
		  for( auto&& s : body )
			{
			  cnv_val_s = (unsigned short)s;
			  cnv_val_s *= 127;
			  cnv_val_s /= 100;
			  // Saturation
			  if ( cnv_val_s > 127 )
				cnv_val_s = 127;
			  // char and unsigned char are equivalent, as the value is 0 <= val < 128
			  cnv_val_8 = cnv_val_s;
			  theFile.write( &cnv_val_8, 1 );
			  if( odd_even == false )
				(*histo)<<= s;
			  odd_even = !odd_even;
			}
		  cout << *histo;
		  delete histo;
		  if ( odd_even )
			{
			  cout << "This, normally should not happened, one padding 0 is added" << endl;
			}
		}
	  /*}*/
  cout << endl;
}

void Process_input_file(const string_view&inputFileName,
						const bool&debug_extra_data,const bool&debug_extra_thresholds)
{
  ofstream outputFile;
  // Open the input file and place the entire content into a string
  // It is assumed the size is never greater than a mega
  ifstream inputFile;
  cout << "Input file: " << inputFileName;
  inputFile.open(string(inputFileName),ios::binary|ios::in);
  ASCII_date_time the_date_time;
  // in the meantime of C++20
  basic_string<unsigned char> str_buff;
  decltype(str_buff)::size_type length = 524288;
  str_buff.reserve( length );
  str_buff.assign(( istreambuf_iterator<char>(inputFile)),istreambuf_iterator<char>());
  basic_string_view<unsigned char> sv_buff( str_buff );
  // Declare the "pointers"
  // in the meantime of C++20
  const basic_string<unsigned char> separ_str=(unsigned char*)"\xff\xff"; 
  const basic_string<unsigned char> separ_str_1=separ_str.substr( 0, 1 );
  decltype( str_buff )::size_type begin_pos,end_pos= str_buff.find( separ_str );
  histogram_info histo_info(debug_extra_thresholds);
  for(;;)
	{
	  // Search for a bloc between 2 separators ($ff)
	  // The number of bytes should be even,
	  //   the number of $ff is odd or even
	  //
	  // Run to the end of the separator
	  // We resume from the last end position
	  begin_pos = end_pos;
	  while( begin_pos != decltype( str_buff )::npos )
		{
		  if ( str_buff.compare( begin_pos, 1, separ_str_1 ) != 0 )
			// We found something not ff
			// To go out of the separator, we need only one unsigned char not equal to $ff
			break;
		  if ( (begin_pos + 1) < str_buff.length() )
			begin_pos += 1;
		  else
			begin_pos = decltype( str_buff )::npos;
		}
	  // If we left because of the eof, quit
	  if( begin_pos == decltype( str_buff )::npos )
		break;
	  // search for the next separator
	  // To go in the (next) separator, we need some unsigned chars equal to $ff
	  end_pos = str_buff.find( separ_str, begin_pos + separ_str.length());
	  // If the end of the string/file is reached, break the for(;;) loop
	  if( end_pos == decltype( str_buff )::npos )
		break;

	  // in the meantime of C++20
	  basic_string_view<unsigned char> slice_sv = sv_buff.substr( begin_pos, end_pos - begin_pos );

	  basic_string_view<unsigned char> header = slice_sv.substr( 0, 28 );

	  // The file-name takes the time-stamp of the first record
	  // then the output file can be opened only here
	  // and not before the main records loop
	  if( ! outputFile.is_open() )
		{
		  // Create the name
		  string filename;
		  filename.reserve( 30 );
		  filename = "Healthmeter_";
		  filename += to_string( header[ 8 ]);
		  filename += "_";
		  filename += to_string( header[ 9 ]);
		  filename += "__";
		  filename += to_string( header[ 10 ]);
		  filename += "_";
		  filename += to_string( header[ 11 ]);
		  filename += ".RAW";
		  // Open the file
		  outputFile.open(filename.c_str(),ios::binary|ios::out);
		  cout << "\tOutput file: " << filename << endl;
		  // Assume it is opened, TODO handle the errors
		  // Now send the titles of the columns
		  cout << " N    ";
		  cout << the_date_time.GetTitles();
		  cout << histo_info.GetTitles();
		  cout << endl;
		}
	  if ( debug_extra_data )
		cout << begin_pos << "-" << end_pos << "=" << end_pos - begin_pos  << "\t";

	  /*	  if ( 1 == 2 )
		{
		  unsigned short pos_count = 0;
		  for( auto&& s : header )
			{
			  cout << (unsigned short)s;
			  switch ( pos_count )
				{
				case 8:
				  cout << '/';
				  break;
				case 10:
				case 11:
				  cout << ':';
				  break;
				case 13:
				  cout << " S";
				  break;
				default:
				  cout<< ' ';
				  break;
				}
			  pos_count ++;
			}
		}
		else*/
		cout << setfill(' ') << setw(2) << (unsigned short)header[ 0 ] << ":  ";
	  
	  if ( the_date_time.Check_new_date_time( header.substr( 7, 6 )) )
		the_date_time.Send_to_raw_file( outputFile );
	  
	  cout << the_date_time << "  ";
	  
	  if( outputFile.is_open() )
		send_bloc( outputFile, slice_sv, the_date_time, histo_info );
	  else
		cout<< "***** Some data is lost, as the output file is not opened *****" << endl;
	}
  cout << endl;
  inputFile.close();
  if ( outputFile.is_open() )
	{
	  outputFile.flush();
	  outputFile.close();
	}
}


int main(int argc,char*argv[])
{

  bool debug_extra_data = false;
  bool debug_extra_thresholds = false;
  bool display_help = false;

  const map< string_view, function< void()> > options_list = {
	{ "-d",[&](){ debug_extra_data = true; } },
	{ "-t",[&](){ debug_extra_thresholds = true; } },
	{ "-h",[&](){ display_help = true; } },
	{ "--help",[&](){ display_help = true; } }
  };

  const vector<string_view>args(argv+1,argv+argc);

  deque< string_view > files_list;
  for( string_view param : args )
	{
	  // It is assumed, there are no argument to any option,
	  //   or there is no space between the option and the argument
	  if ( param.substr( 0, 1 ) == string( "-" ))
		{
		  decltype(options_list)::const_iterator option_found = options_list.find( param );
		  if ( option_found != options_list.end() )
			option_found->second();
		  else
			cout << " Unknown option: " << param << ", use -h or --help to get the documentation" << endl;
		} else
		  files_list.push_back( param );
	}

  if ( files_list.empty() )
	  files_list.push_back(string_view(DEFAULTINPUTFILE) );


  if ( display_help )
	{
	  cout << "HELP, there is no help. It is going to come later" << endl;
	}else
	for( string_view theFile : files_list )
	  Process_input_file( theFile, debug_extra_data, debug_extra_thresholds );
}
