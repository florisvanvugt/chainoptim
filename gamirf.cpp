

/* 
   This is the Ideal Response Function (IRF) for a particular design. 
*/

#include <vector>
#include "irf.hpp"


GAM::GAM(vector<float> trialtimes)
/* Constructor: here we need to inhale the trial times */
{
  this->trialtimes = trialtimes;
}


vector<float> GAM::evaluate(vector<float> t) {
  /* Evaluate the predicted BOLD response
     at particular time points (typically scan times) t */

  /* Since we don't do this by convolution, but rather
     assume that we copy-paste an HRF at each trial onset,
     we can simply compute this on the fly as we have
     scan times. */
  
}



