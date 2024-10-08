#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <string_view>
using namespace std;


class ASCII_date_time {
  unsigned char year;
  unsigned char month;
  unsigned char day;
  unsigned char hour;
  unsigned char minute;
  unsigned char second;
  // The result of Check_new_date_time is saved into this variable as well
  bool isNewDateTime;
  void Set_new_date_time( const basic_string_view<unsigned char>& );
  void send_digit( ostream&theFile, const unsigned char&digit )const;
public:
  explicit ASCII_date_time();
  // Checks if the data provided is one hour after the date and time
  // recorded in the class.
  // If so, return true
  // For all cases, the date provided overwrites the recorded date
  bool Check_new_date_time( const basic_string_view<unsigned char>& );
  void Send_to_raw_file(ostream&)const;
  constexpr string_view GetTitles()const
  {
	return string_view("  date     time   ");
  }
  friend ostream&operator<<(ostream&os,const ASCII_date_time&);
};
