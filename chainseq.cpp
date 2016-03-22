#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>

#include <boost/program_options.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <chain.hpp>
#include <aux.hpp>


namespace ublas = boost::numeric::ublas;

namespace po = boost::program_options; // For command-line options



/* The version number */
const char *VERSION = "0.5";





void deal_with_cl(int argc, char* argv[])
/* This deals with parsing the command line arguments */
{
  
  /* First we define the allowed options */
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help",
     "produce help message")
    ("verbose",
     "print verbose output to the screen")
    ("ntp",
     po::value<int>(&config::ntp),
     "number of volume acquisitions")
    ("tr",
     po::value<float>(&config::TR),
     "TR duration (sec)")
    ("ntrials",
     po::value<int>(&config::ntrials),
     "number of trials")
    ("trial_duration",
     po::value<float>(&config::trial_duration_sec),
     "duration of a trial (sec)")
    ("hrf",
     po::value<std::string>(&config::hrf)->default_value("gam"),
     "hypothesised HRF function")
    ("choose",
     po::value<std::string>(&config::move_choose)->default_value(config::move_choose),
     "how to choose from the possible movements at each iteration: 'max' or 'random'")
    ("1d",
     po::value<std::string>(&config::afniout)->default_value(""),
     "write the stimulus times to an AFNI-style output file")
    ("Xout",
     po::value<std::string>(&config::xout)->default_value(""),
     "write the design matrix to an output file")
    ("history",
     po::value<std::string>(&config::history)->default_value(""),
     "write a history file of the iterations")
    ("polort",
     po::value<int>(&config::npolort)->default_value(config::npolort),
     "number of orthogonal Legendre polynomials to use")
    ("maxiter",
     po::value<int>(&config::maxiter),
     "maximum number of iterations to perform")
    ("stack_move",
     po::value<bool>(&config::stack_move)->default_value(config::stack_move),
     "whether to allow moving a whole stack of null time at a time")
    ("far_move",
     po::value<bool>(&config::far_move)->default_value(config::far_move),
     "whether to allow moving null time far away (to the next non-null time block)")
    ("randomseed",
     po::value<unsigned>(&config::random_seed),
     "seed for the random number generator")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    
  
  if (vm.count("help")) {
    std::cout << desc << "\n";
    exit(EXIT_SUCCESS);
  }

  if (vm.count("verbose")) {
    config::verbose = true;
  }

  /* Convert movement-choose parameter and check its value. */
  std::transform(config::move_choose.begin(), config::move_choose.end(), config::move_choose.begin(), ::tolower);
  if (!(config::move_choose=="random" || config::move_choose=="max")) {
    std::cout<<"Unknown movement choice strategy '"<<config::move_choose<<"'\n";
    exit(EXIT_FAILURE);
  }

  
  /* Convert hrf to lower case and check its value */
  std::transform(config::hrf.begin(), config::hrf.end(), config::hrf.begin(), ::tolower);
  if (!(config::hrf=="gam")) {
    std::cout<<"Unknown HRF '"<<config::hrf<<"'\n";
    exit(EXIT_FAILURE);
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
  } else {

    // Check whether trial duration is divisible by TR
    float rem = fmod(config::trial_duration_sec,config::TR);
    if (abs(rem)>.001) {
      std::cout<<"The trial duration needs to be a multiple of the volume acquisition time. Remainder "<<rem<<" sec.\n";
      exit(EXIT_FAILURE);
    }

    // Compute trial duration as # of TRs.
    config::trial_duration = config::trial_duration_sec/config::TR;

    
  }

}








int main(int argc, char* argv[])
{
  std::cout<<"Chain fMRI event-related design optimiser v"<<VERSION<<"\n";
  deal_with_cl(argc,argv);

  config::pre_report();

  /* Preliminaries */
  srand (config::random_seed);


  clock_t tStart = clock();

  /* Initiate the chain */
  Chain chain;// this actually initialises it
  bool success = chain.run();

  clock_t tEnd = clock();

  if (success) {
    // Look at output
    std::cout<<"completed "<<chain.iteration<<" iterations.\n";;
    std::cout<<"Final efficiency: "<<chain.history.back().efficiency<<"\n";;
    /* Do your stuff here */
    std::cout<<"Time taken: " << (double)(tEnd - tStart)/CLOCKS_PER_SEC <<" sec.\n";

    //chain.result_design->print();
    
    if (config::afniout!="") {
      /* Write the stimulus times to an AFNI-style output file */
      chain.result_design->to_afni_file(config::afniout.c_str());
    }

    if (config::xout!="") {
      /* Write the final design matrix to an output file */
      ublas::matrix<double> X;
      X = chain.result_design->get_matrix();
      matrix_to_file(X,config::xout.c_str(),(char*)" ");
    }

    if (config::history!="") {
      /* Write the final design matrix to an output file */
      chain.history_to_file(config::history.c_str());
    }

    
  }
  
  return 0;
  
}
