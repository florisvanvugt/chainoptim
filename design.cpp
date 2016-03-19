/* 
This is an experimental design.
 */

#include <iostream>
#include "design.hpp"
#include <stdio.h>
#include <cstdlib>
#include "aux.hpp"


namespace ublas = boost::numeric::ublas;

const char *delim = ", ";


Design::Design(int ntrials,float tr,int trial_duration) {
  this->ntrials = ntrials;
  this->tr = tr;
  this->trial_duration = trial_duration;
  
  // Initialise the ntrials array
  ublas::vector<int> nt(ntrials+1);
  this->nulltrs = nt;
}


void Design::print()
/* Print the contents of this design */
{
  std::cout << "(" << this->ntrials << " trials) [";
  std::cout << intvec2str(this->nulltrs,delim);
  std::cout << "]\n";
}






void Design::randomise(int n_null_tp)
/* 
   Randomly initialise this design by inserting 
   the number of null TRs at random positions.
*/
{
  std::cout<<"Randomly allocating "<<n_null_tp<<" null TRs\n";

  // First make sure we empty the null TR list (just in case)
  for (int i=0; i<this->ntrials+1; ++i) {
    this->nulltrs[i]=0;
  }

  // Randomly distribute the null TRs
  int bin;
  for (int i=0; i<n_null_tp; ++i) {
    // Decide which bin we will add a null TR to
    bin = rand() % (this->ntrials+1); // produces a number 0..(ntrials)
    this->nulltrs[bin] += 1;
  }

  // Now sum(this->nulltrs) should equal n_null_tp
  
}








ublas::vector<float> Design::get_trial_onsets()
/* 
   Returns the trial onsets of the given design.
   
   Arguments
   tr : the TR duration (in seconds) 
*/
{
  ublas::vector<float> trialtimes(this->ntrials);
  int preceding_trs = 0;
  for (int i=0; i<this->ntrials; ++i) {
    preceding_trs += this->nulltrs[i];
    trialtimes[i] =  this->tr*preceding_trs;
    preceding_trs += trial_duration;
  }
  return (trialtimes);
};



void Design::print_trial_onsets()
/* Prints the trial onsets of the given design.

Arguments
tr : the TR duration (in seconds) 
*/
{
  std::cout<<this->trial_onsets();
}




std::string Design::trial_onsets()
/* Returns the trial onset times for this design. */
{
  // Source: http://stackoverflow.com/questions/8581832/converting-a-vector-to-string
  ublas::vector<float> vec = this->get_trial_onsets();
  return floatvec2str(vec,delim);
}






IRF* Design::get_irf(std::string hrftype)
/* 
   Return the ideal response function for this design.
   
   Arguments
   tr : TR duration (in seconds)
   hrftype : type of HRF hypothesised (e.g. GAM)
*/
{
  IRF *irf;
  if (hrftype=="gam") {
    irf = new GAM(this->get_trial_onsets());
  }
  //if (hrftype=="gam") {
  //return 
  //  }
  return (irf);
}








ublas::matrix<double> Design::get_matrix(std::string hrftype,
					 int ntp,
					 float TR,
					 int npolort) 
/* 
   Get the design matrix for this design 

   Arguments
   hrftype : hypothesised HRF (e.g. "gam")
   ntp : number of time points (scans)
   TR : duration of volume acquisition (in seconds)
   npolort : number of orthogonal polynomials to use
*/
{
  /* First get the regressor of interest, namely the predicted BOLD response */
  IRF* irf = this->get_irf(hrftype);
  ublas::vector<float> scantimes = scalmult(arange(0,ntp),TR);
  //std::cout<< "Scan times:" << floatvec2str(scantimes," ") << "\n";
  ublas::vector<float> scans = irf->evaluate(scantimes);
  //std::cout<< "Scan values:" << floatvec2str(scans," ") << "\n";

  /* Now get the remaining orthogonal polynomials */
  ublas::matrix<float> X = polort(npolort,scantimes);
  // Stick our regressor of interest into the design matrix first column
  for (unsigned i=0; i<scans.size(); ++i) {
    X(i,0) = scans[i];
  }

  return X;
  
}




float Design::get_efficiency(std::string hrftype,int ntp,float TR,int npolort)
/* 
   Calculate the efficiecny of this design.

   Arguments
   hrftype : hypothesised HRF (e.g. "gam")
   ntp : number of time points (scans)
   TR : duration of volume acquisition (in seconds)
   npolort : number of orthogonal polynomials to use
 */
{
  /* Get our design matrix */
  ublas::matrix<float> X = this->get_matrix(hrftype,ntp,TR,npolort);

  /* Now perform the statistical magic of inverting and all. */
  float eff = 0.0;

  // Now let's get the efficiency... exciting!
  /* 
     Definition of efficiency:
     https://afni.nimh.nih.gov/pub/dist/edu/2007_11_extra_3day/pdf_handouts/ExptDsgn.pdf 
  */

  // Define the contrast vector
  //# the contrast: for now we're just interested in the regressor associated with our neural activity.
  ublas::vector<float> contr(npolort+2);
  for (int i=0; i<npolort+2; ++i) {
    if (i==0) { 
      contr[i]=1; 
    } else { 
      contr[i]=0; 
    };
  };

  // Compute X'X
  ublas::matrix<float> XX = ublas::prod(ublas::trans(X),X);  

  //std::cout<<"made XX";
  //matrix_to_file(XX,(char*)"tmp.XX.txt",(char*)" ");

  // Now invert XX
  ublas::matrix<float> XXinv(XX.size1(),XX.size2());
  bool res = MInvBoost(XX,XXinv); //= ublas::invert(XX);
  if (not res) {
    //std::cout<<"Error inverting matrix.\n";
    exit(EXIT_FAILURE);
  }
  //  std::cout<<"inverted XX";

  ublas::vector<float> XXinvc = ublas::prod(XXinv,contr);

  float nsd = sqrt( ublas::inner_prod(contr,XXinvc) );
  eff = 1/nsd;

  return eff;

}
