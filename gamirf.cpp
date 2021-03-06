

/* 
   This is the Ideal Response Function (IRF) for a particular design. 
*/

#include "irf.hpp"
#include <math.h>
#include <boost/numeric/ublas/vector.hpp>


GAM::GAM(vector<float> trialtimes) {
  this->trialtimes = trialtimes;
}



// Parameters for the GAM function
// Source: https://afni.nimh.nih.gov/pub/dist/doc/program_help/3dDeconvolve.html
const double p = 8.6;
const double q = .547;

/* The time after which we consider the stimulus effect has died out (BOLD return to baseline) */
const double max_extent_t = 35;



vector<double> GAM::evaluate(vector<float> times)
/* 
   Evaluate the predicted BOLD response
   at particular time points (typically scan times)
   
   Arguments:
   times : the scan onset times (in seconds)
*/
{
  /* Since we don't do this by convolution, but rather
     assume that we copy-paste an HRF at each trial onset,
     we can simply compute this on the fly as we have
     scan times. */
  
  // This will hold the predicted BOLD reading at the given time
  vector<double> bold(times.size());
  for (unsigned i=0; i<times.size(); ++i)
    bold[i]=0;
  
  // Now for each trial we will simply add the corresponding activation
  // to the existing list.
  double t;
  double trialt;
  for (unsigned i=0; i<this->trialtimes.size(); ++i) {
    
    trialt = this->trialtimes[i]; // The onset time of the trial under consideration
    
    for (unsigned j=0; j<times.size(); ++j) {

      // This is the time of this current target time relative to the
      // onset of the trial in question.
      t = times[j] - trialt;
      
      // Update the predicted BOLD for the given trial using the AFNI GAM equation
      // from the AFNI 3dDeconvolve list:
      //(t/(p*q))^p * exp(p-t/q)                      
      //Defaults: p=8.6 q=0.547 if only 'GAM' is used   
      if (t>0 && t<max_extent_t) // If this time point comes after the start of this trial...
	bold[j] += pow(t/(p*q),p) * exp(p-(t/q));
      
    }
  }
  return bold;

}





