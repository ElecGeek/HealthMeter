CXX?=g++
CXXFLAGS?=-O2
LDFLAGS?=
DEFAULTINPUTFILENAME?='"HEALTHMETER.DAT"'
SRCDIR?=
BUILDDIR?=
EXECDIR?=




$(EXECDIR)healthmeter	:	$(BUILDDIR)healthmeter.o $(BUILDDIR)histogram.o $(BUILDDIR)date_time.o
	g++ -o $(EXECDIR)healthmeter $(BUILDDIR)healthmeter.o $(BUILDDIR)histogram.o $(BUILDDIR)date_time.o

$(BUILDDIR)healthmeter.o	: $(SRCDIR)healthmeter.cpp $(SRCDIR)date_time.hxx $(SRCDIR)histogram.hxx
	g++ -c -o $(BUILDDIR)healthmeter.o -std=c++17 -D DEFAULTINPUTFILE=$(DEFAULTINPUTFILENAME) $(SRCDIR)healthmeter.cpp

$(BUILDDIR)date_time.o	: $(SRCDIR)date_time.cpp $(SRCDIR)date_time.hxx
	g++ -c -o $(BUILDDIR)date_time.o -std=c++17 $(SRCDIR)date_time.cpp

$(BUILDDIR)histogram.o	: $(SRCDIR)histogram.cpp $(SRCDIR)histogram.hxx
	g++ -c -o $(BUILDDIR)histogram.o -std=c++17 $(SRCDIR)histogram.cpp


clean	:
	rm -f $(EXECDIR)healthmeter
	rm -f $(BUILDDIR)healthmeter.o $(BUILDDIR)histogram.o $(BUILDDIR)date_time.o
