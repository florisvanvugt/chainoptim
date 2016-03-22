
#include <iostream>
#include "config.hpp"


namespace config { 
  int ntp;
  float TR;
  int ntrials;
  int trial_duration;
  float trial_duration_sec;

  /* 
     Initialise some variables (these are defined in config.hpp) 
  */
  std::string hrf = "gam";
  int npolort = 4;
  int maxiter = -1;
  unsigned random_seed = time(NULL);
  std::string move_choose = "random";
  bool verbose = false;
  std::string afniout="";
  std::string xout="";
  std::string history="";

}





void config::pre_report() 
/* Prints a report of what we are going to do */
{
  std::cout << "----------------------------------------\n";
  std::cout << "     PARAMETERS               \n";
  std::cout << "----------------------------------------\n";
  std::cout << "  # time points:              "<<ntp<<"\n";
  std::cout << "  TR (sec):                   "<<TR<<"\n";
  std::cout << "  # trials:                   "<<ntrials<<"\n";
  std::cout << "  Trial duration (sec):       "<<trial_duration_sec<<"\n";
  std::cout << "  Trial duration (# of TRs):  "<<trial_duration<<"\n";
  std::cout << "  Hypothesis HRF:             "<<hrf<<"\n";
  std::cout << "  # of orthogonal polys:      "<<npolort<<"\n";
  std::cout << "  Max # of iterations:        "<<maxiter<<"\n";
  std::cout << "  Movement choice:            "<<move_choose<<"\n";
  std::cout << "----------------------------------------\n";
}



