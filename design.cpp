/* 
This is an experimental design.
 */

#include <iostream>
#include "design.hpp"
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include "aux.cpp"

using namespace std;


const char *delim = ", ";


Design::Design(int ntrials) {
  this->ntrials = ntrials;
  
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






vector<float> Design::get_trial_onsets(float tr)
/* Returns the trial onsets of the given design.

Arguments
tr : the TR duration (in seconds) 
*/
{
  vector<float> trialtimes(this->ntrials);
  int preceding_trs = 0;
  for (int i=0; i<this->ntrials; ++i) {
    preceding_trs += this->nulltrs[i];
    trialtimes[i] = tr*preceding_trs;
  }
  return (trialtimes);
};



void Design::print_trial_onsets(float tr)
/* Prints the trial onsets of the given design.

Arguments
tr : the TR duration (in seconds) 
*/
{
  cout<<this->trial_onsets(tr);
}




string Design::trial_onsets(float tr)
{
  // Source: http://stackoverflow.com/questions/8581832/converting-a-vector-to-string
  vector<float> vec = this->get_trial_onsets(tr);
  return floatvec2str(vec,delim);
}
