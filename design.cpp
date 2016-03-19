/* 
This is an experimental design.
 */

#include <iostream>
#include "design.hpp"
#include <vector>
#include <stdio.h>
#include <cstdlib>

using namespace std;


Design::Design(int ntrials) {
  this->ntrials = ntrials;

  // Initialise the ntrials array
  vector<int> nt(ntrials+1);
  this->nulltrs = nt;
}


void Design::print()
/* Print the contents of this design */
{
  printf("(%d trials) [",this->ntrials);
  vector<int>::const_iterator i;
  for (i=this->nulltrs.begin(); i!=this->nulltrs.end(); ++i) {
    cout << (*i) << ",";
  }
  printf("]\n");
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


