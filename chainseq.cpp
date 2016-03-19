#include <iostream>
#include <boost/program_options.hpp>
#include <cstdlib>

#include <design.hpp>

using namespace std; 

namespace po = boost::program_options; // For command-line options



/* The version number */
const char *VERSION = "0.01";


/* The number of volumes (measurements) */
int ntp;

/* The TR duration (acquisition time for a single volume in seconds) */
float TR;

/* The number of trials */
int ntrials;

/* The duration of a trial (in # of TRs) */
int trial_duration;

/* THe hypothesised HRF function */
string hrf = "gam";


/* initialize random seed: */
unsigned random_seed = time(NULL);




void deal_with_cl(int argc, char* argv[])
/* This deals with parsing the command line arguments */
{
  
  /* First we define the allowed options */
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help",
     "produce help message")
    ("ntp",
     po::value<int>(&ntp),
     "number of volume acquisitions")
    ("tr",
     po::value<float>(&TR),
     "TR duration (sec)")
    ("ntrials",
     po::value<int>(&ntrials),
     "number of trials")
    ("trial_duration",
     po::value<int>(&trial_duration)->default_value(-1),
     "duration of a trial (in # of TRs)")
    ("hrf",
     po::value<string>(&hrf)->default_value("gam"),
     "hypothesised HRF function")
    ("randomseed",
     po::value<unsigned>(&random_seed),
     "seed for the random number generator")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    
  
  if (vm.count("help")) {
    cout << desc << "\n";
    exit(EXIT_SUCCESS);
  }

  if (!vm.count("ntp")) {
    printf("You need to set the number of time points (--ntp)\n");
    exit(EXIT_FAILURE);
  }
  if (!vm.count("ntrials")) {
    printf("You need to set the number of trials (--ntrials))\n");
    exit(EXIT_FAILURE);
  }
  if (!vm.count("tr")) {
    printf("You need to set the TR duration (--tr)\n");
    exit(EXIT_FAILURE);
  }
  if (!vm.count("trial_duration")) {
    printf("You need to set the trial duration (--trial_duration)");
    exit(EXIT_FAILURE);
  }

}






void pre_report() 
/* Prints a report of what we are going to do */
{
    printf("----------------------------------------\n");
    printf("     PARAMETERS               \n");
    printf("----------------------------------------\n");
    printf("  # time points:              %d\n",ntp);
    printf("  TR (sec):                   %f\n",TR);
    printf("  # trials:                   %d\n",ntrials);
    printf("  Trial duration (# of TRs):  %d\n",trial_duration);
    printf("----------------------------------------\n");
}






int main(int argc, char* argv[])
{
  deal_with_cl(argc,argv);

  pre_report();

  /* Preliminaries */
  srand (random_seed);
    
  Design design(ntrials);

  int n_null_trs = ntp-(ntrials*trial_duration);
  design.randomise(n_null_trs);
  
  design.print();
  
  return 0;
  
}
