#include <iostream>
#include <fstream>
#include <cstdlib>

#include <boost/program_options.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <design.hpp>
#include <aux.hpp>


namespace ublas = boost::numeric::ublas;

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

/* The hypothesised HRF function */
std::string hrf = "gam";

/* The number of orthogonal polynomials to stick into the design matrix */
int npolort = 3;


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
     po::value<int>(&trial_duration),
     "duration of a trial (in # of TRs)")
    ("hrf",
     po::value<std::string>(&hrf)->default_value("gam"),
     "hypothesised HRF function")
    ("polort",
     po::value<int>(&npolort)->default_value(npolort),
     "number of orthogonal Legendre polynomials to use")
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
  std::cout << "----------------------------------------\n";
}






int main(int argc, char* argv[])
{
  deal_with_cl(argc,argv);

  pre_report();

  /* Preliminaries */
  srand (random_seed);

  Design design(ntrials,TR,trial_duration);

  int n_null_trs = ntp-(ntrials*trial_duration);
  design.randomise(n_null_trs);

  design.print();
  printf("Trial onsets:\n");
  design.print_trial_onsets();
  printf("\n");

  IRF* irf = design.get_irf(hrf);
  ublas::vector<float> scantimes = scalmult(arange(0,ntp),TR);
  std::cout<< "Scan times:" << floatvec2str(scantimes," ") << "\n";
  ublas::vector<float> scans = irf->evaluate(scantimes);
  std::cout<< "Scan values:" << floatvec2str(scans," ") << "\n";

  // Write to a file
  std::ofstream myfile;
  myfile.open ("scanBOLD.txt");
  myfile << "scan.BOLD\n" << floatvec2str(scans,"\n");
  myfile.close();
  
  //ublas::matrix<float> mat = design.get_matrix(hrf,ntp,TR,npolort);

  // Now let's calculate efficiency
  std::cout << "Efficiency: " << design.get_efficiency(hrf,ntp,TR,npolort)<<"\n";
  
  return 0;
  
}
