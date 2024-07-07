For the whose has an health or sport meter meter with the properties:
* can be connected with a (micro) usb that exposes a disk containing a unique file.
* the file is a concatenation of measurements, until the meter's memory is full (and has be be erase for the device).
* the measurement binary files:
  * contains some data
  * contains records separated by a series of $ff (odd or even)
    * contain a header
    * record number
    * some data
    * the date and time as plain ASCII characters
    * some data
    * the sampling rate as the time, in seconds, between two samples.
    * some data
      * contains a data zone that can read as a raw file 8bits, signed, 2 channels
      * the data is between 0 and 100
  * terminates with a set of $ff
This project is for him.


The project reads one or more binary files to generate:
* some statistics sent to the standard output.
  * the record number
  * some data in debug mode only
  * the date
  * the time
  * the record length in minutes
  * the min, max and average
  * many percent of the time the value is under a certain threshold
* a raw file name base on the first record time-stamp.
* a raw file intended to be open by most of the music editing software.
  * a record limit containing the time-stamp in the 2 channels. It has been design to not be confused with the record themselves
    * 100% sample
    * date
    * 0%
    * time
    * 100% sample
	The user has to zoom at the maximum to see the samples individually.
    the numbers (between 0 and 59) are coded as 2 samples. one IS for the left digit, one for the right. They are multiplied by 10%. e.g. 52 is 50% 20%.
  * the data converted from 0 to 100% into 0 to 127. By this way, the editing software shows 100% for a 100% measure.

An additional feature applies if a record follows the previous one exactly one hour later:
* the statistics display the time-stamp into brackets.
* the record limit, in the raw file, are omitted.

All thee interfaces with the machine are done via the C++11 features. The project should compile my a simple "make" invocation.
The Makefile supports the SOURCEDIR, BUILDDIR, DESTDIR, CXX, CXXFLAGS and LDFLAGS. 

By editing and recompiling, some modifications can be done to change, for instance, the thresholds.

TODO: make some debug activation more optional than hard-coded

TODO: write the workflows to compile for many plateforms

TODO: read the arguments to load multiples input files

TODO or never: handle properly the new and fix the bug that does not merge the records in the new year night
