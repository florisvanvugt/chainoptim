#include <iostream>  // need this header file to support the C++ I/O system
#include <boost/program_options.hpp>

using namespace std; // telling the compiler to use namespace "std",
		     // where the entire C++ library is declared.

namespace po = boost::program_options; // For command-line options



/* The version number */
const char *VERSION = "0.01";


/* The number of volumes (measurements) */
int ntp = -1;

/* The TR duration (acquisition time for a single volume in seconds) */
float TR = -1;

/* The number of trials */
int ntrials = -1;

/* The duration of a trial (in # of TRs) */
int trial_duration = -1;






void deal_with_cl(int argc, char* argv[])
/* This deals with parsing the command line arguments */
{
  
  int opt;

  /* First we define the allowed options */
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help",
     "produce help message")
    ("ntp",
     po::value<int>(&ntp)->default_value(-1), 
     "number of volume acquisitions")
    ("optimization",
     po::value<int>(&opt)->default_value(10), 
     "optimization level")
    ("include-path,I",
     po::value< vector<string> >(), 
     "include path")
    ("input-file",
     po::value< vector<string> >(),
     "input file")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    
  
  if (vm.count("help")) {
    cout << desc << "\n";
    exit(EXIT_SUCCESS);
  }
  
  if (vm.count("compression")) {
    cout << "Compression level was set to " 
	 << vm["compression"].as<int>() << ".\n";
  } else {
    cout << "Compression level was not set.\n";
  } 
}




int main(int argc, char* argv[])
{
  // Print out a sentence on the screen.
  // "<<" causes the expression on its right to 
  // be directed to the device on its left.
  // "cout" is the standard output device -- the screen.
  cout << "Hello World!" <<  endl;

  deal_with_cl(argc,argv);
  
  return 0; // returns 0,which indicate the successful	
  // termination of the "main" function 
  
}
