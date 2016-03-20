#include <iostream>
#include <fstream>
#include <cstdlib>

#include <boost/program_options.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <chain.hpp>
#include <aux.hpp>


namespace ublas = boost::numeric::ublas;

namespace po = boost::program_options; // For command-line options



/* The version number */
const char *VERSION = "0.05";


/* The number of volumes (measurements) */
int ntp;

/* The TR duration (acquisition time for a single volume in seconds) */
float TR;

/* The number of trials */
int ntrials;

/* The duration of a trial (in # of TRs) */
int trial_duration;

/* The hypothesised HRF function */
std::string hrf = "gam";

/* The number of orthogonal polynomials to stick into the design matrix */
int npolort = 3;

/* The maximum number of iterations we will try, after which we terminate the chain. (-1 means there is no maximum) */
int maxiter = -1;

/* initialize random seed: */
unsigned random_seed = time(NULL);

/* Verbose output */
bool verbose = false;

/* AFNI-style 1D output file with stimulus times. */
std::string afniout="";

/* Write the final design matrix to an output file. */
std::string xout="";



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
     po::value<int>(&ntp),
     "number of volume acquisitions")
    ("tr",
     po::value<float>(&TR),
     "TR duration (sec)")
    ("ntrials",
     po::value<int>(&ntrials),
     "number of trials")
    ("trial_duration",
     po::value<int>(&trial_duration),
     "duration of a trial (in # of TRs)")
    ("hrf",
     po::value<std::string>(&hrf)->default_value("gam"),
     "hypothesised HRF function")
    ("1d",
     po::value<std::string>(&afniout)->default_value(""),
     "write the stimulus times to an AFNI-style output file")
    ("Xout",
     po::value<std::string>(&xout)->default_value(""),
     "write the design matrix to an output file")
    ("polort",
     po::value<int>(&npolort)->default_value(npolort),
     "number of orthogonal Legendre polynomials to use")
    ("maxiter",
     po::value<int>(&maxiter),
     "maximum number of iterations to perform")
    ("randomseed",
     po::value<unsigned>(&random_seed),
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
    verbose = true;
  }

  /* Convert hrf to lower case */
  std::transform(hrf.begin(), hrf.end(), hrf.begin(), ::tolower);
  if (!(hrf=="gam")) {
    std::cout<<"Unknown HRF '"<<hrf<<"'\n";
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
  }

}






void pre_report() 
/* Prints a report of what we are going to do */
{
  std::cout << "----------------------------------------\n";
  std::cout << "     PARAMETERS               \n";
  std::cout << "----------------------------------------\n";
  std::cout << "  # time points:              "<<ntp<<"\n";
  std::cout << "  TR (sec):                   "<<TR<<"\n";
  std::cout << "  # trials:                   "<<ntrials<<"\n";
  std::cout << "  Trial duration (# of TRs):  "<<trial_duration<<"\n";
  std::cout << "  Hypothesis HRF:             "<<hrf<<"\n";
  std::cout << "  # of orthogonal polys:      "<<npolort<<"\n";
  std::cout << "  Max # of iterations:        "<<maxiter<<"\n";
  std::cout << "----------------------------------------\n";
}






int main(int argc, char* argv[])
{
  deal_with_cl(argc,argv);

  pre_report();

  /* Preliminaries */
  srand (random_seed);

  /* Initiate the chain */
  Chain chain(ntrials,ntp,TR,trial_duration,npolort,hrf);
  bool success = chain.run(maxiter,verbose);

  if (success) {
    // Look at output
    std::cout<<"completed.\n";;
    //chain.result_design->print();
    
    if (afniout!="") {
      /* Write the stimulus times to an AFNI-style output file */
      chain.result_design->to_afni_file(afniout.c_str());
    }

    if (xout!="") {
      /* Write the final design matrix to an output file */
      ublas::matrix<double> X;
      X = chain.result_design->get_matrix(hrf,ntp,TR,npolort);
      matrix_to_file(X,xout.c_str(),(char*)" ");

    }

  }
  
  return 0;
  
}
