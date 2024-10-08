#include "date_time.hxx"
#include "histogram.hxx"


ASCII_date_time::ASCII_date_time():
  month( 0 ), day( 0 ),
  hour( 0 ), minute( 0 ), second( 0 ),
  isNewDateTime( true )
{}

void ASCII_date_time::Set_new_date_time( const basic_string_view<unsigned char>&a )
{
  year = a[ 0 ] + 17;
  month = a[ 1 ];
  day = a[ 2 ];
  hour = a[ 3 ];
  minute = a[ 4 ];
  second = a[ 5 ];
}
bool ASCII_date_time::Check_new_date_time( const basic_string_view<unsigned char>&a )
{
  if ( a[ 5 ] != second || a[ 4 ] != minute )
	{
	  isNewDateTime = true;
	}else{
	if ( a[ 3 ] != 0 )
	  {
		// Still the same day, check for YYMMDD
		if ( a[ 3 ] == ( hour + 1 ) && a[ 2 ] == day  && a[ 1 ] == month && ( a[ 0 ] + 17 ) == year )
		  {
			isNewDateTime = false;
		  }else{
		  isNewDateTime = true;
		}
	  }else{
	  // Not the same day, check against the dates
	  if( hour == 23 && (
						 // Still the same month
						 ( a[ 2 ] == ( day + 1 ) && a[ 1 ] == month ) ||
						 // Not the same month, check for day=1 and next month
						 ( a[ 2 ] == 1 && a[ 1 ] == ( month + 1 ))
						 // Assume, we don't make any measurement the new year night 
						 ))
		{
		  isNewDateTime = false;
		}else{
		isNewDateTime = true;
	  }
	}
  }
  Set_new_date_time( a );
  return isNewDateTime;
}
void ASCII_date_time::Send_to_raw_file(ostream&theFile)const
{
  send_digit( theFile, 0xff );
  send_digit( theFile, month );
  send_digit( theFile, day );
  send_digit( theFile, 0x0 );
  send_digit( theFile, hour );
  send_digit( theFile, minute );
  send_digit( theFile, second );
  send_digit( theFile, 0xff );
}
ostream&operator<<(ostream&os,const ASCII_date_time&a)
{
  if ( a.isNewDateTime )
	{ 
	  os << (unsigned short)a.year<< '/';
	  os << setfill('0') << setw(2) << (unsigned short)a.month << '/';
	  os << setfill('0') << setw(2) << (unsigned short)a.day << ' ';
	}else
	os << "         ";

  os << setfill(' ') << setw(2) << (unsigned short)a.hour << ':';
  os << setfill('0') << setw(2) << (unsigned short)a.minute << ':';
  os << setfill('0') << setw(2) << (unsigned short)a.second;
  return os;
}



void ASCII_date_time::send_digit( ostream&theFile, const unsigned char&digit )const
{
  // It displays a number, using 1 bar of amplitude 10% per unit
  // 
  // This function is supposed to be relevant for time-stamp only
  // It assumes, the numbers are never greater than 99, even 59
  // $7f is treated to get the maximum for both bars
  array<char,12>buffer;
  fill( buffer.begin(), buffer.end(), 0x0 );
  // Convert the char into BCD
  unsigned short theValL = digit/10;
  unsigned short theValR = digit - theValL * 10;
  if ( digit != 0xff ) {
	theValL *= 127;
	theValL /= 10;
	if ( theValL > 127 )
	  theValL = 127;
	theValR *= 127;
	theValR /= 10;
	if ( theValR > 127 )
	  theValR = 127;
  }else{
	theValR = 127;
	theValL = 127;
  }
  // The assume is the destination is a stereo, 8bits, signed file
  // Then the spacers and the data is sent twice
  buffer[ 4 ] = theValL;
  buffer[ 5 ] = theValL;
  buffer[ 8 ] = theValR;
  buffer[ 9 ] = theValR;
  theFile.write( buffer.data(), buffer.size() );
}
