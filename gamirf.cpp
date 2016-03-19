

/* 
   This is the Ideal Response Function (IRF) for a particular design. 
*/

#include <vector>
#include "irf.hpp"


GAM::GAM(vector<float> trialtimes) {
  this->trialtimes = trialtimes;
}


vector<float> GAM::evaluate(vector<float> t) {
  /* Evaluate the predicted BOLD response
     at particular time points (typically scan times) t */

  /* Since we don't do this by convolution, but rather
     assume that we copy-paste an HRF at each trial onset,
     we can simply compute this on the fly as we have
     scan times. */

  // This will hold the predicted BOLD reading at the given time
  vector<float> bold(t.size());
  for (unsigned i=0; i<t.size(); ++i)
    bold[i]=0;
  
  // Now for each trial we will simply add the corresponding activation
  // to the existing list.
  for (unsigned i=0; i<this->trialtimes.size(); ++i) {

    // from the AFNI 3dDeconvolve list:
    //(t/(p*q))^p * exp(p-t/q)                      
    //Defaults: p=8.6 q=0.547 if only 'GAM' is used   

  }
  return bold;

}



