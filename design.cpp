/* 
This is an experimental design.
 */

#include <iostream>
#include "design.hpp"
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include "aux.hpp"

using namespace std;


const char *delim = ", ";


Design::Design(int ntrials,float tr,int trial_duration) {
  this->ntrials = ntrials;
  this->tr = tr;
  this->trial_duration = trial_duration;
  
  // Initialise the ntrials array
  vector<int> nt(ntrials+1);
  this->nulltrs = nt;
}


void Design::print()
/* Print the contents of this design */
{
  cout << "(" << this->ntrials << " trials) [";
  cout << intvec2str(this->nulltrs,delim);
  cout << "]\n";
}



void Design::randomise(int n_null_tp)
/* 
   Randomly initialise this design by inserting 
   the number of null TRs at random positions.
*/
{
  printf("Randomising %d trials\n",n_null_tp);

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






vector<float> Design::get_trial_onsets()
/* 
   Returns the trial onsets of the given design.
   
   Arguments
   tr : the TR duration (in seconds) 
*/
{
  vector<float> trialtimes(this->ntrials);
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
  cout<<this->trial_onsets();
}




string Design::trial_onsets()
/* Returns the trial onset times for this design. */
{
  // Source: http://stackoverflow.com/questions/8581832/converting-a-vector-to-string
  vector<float> vec = this->get_trial_onsets();
  return floatvec2str(vec,delim);
}






IRF* Design::get_irf(string hrftype)
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
