
#include <vector>

using namespace std;



/* The precision (timestep) of the reconstruction of the HRF */
const float DT = .05;



vector<float> ideal_response_function( vector<float> trial_onsets, string hrf )
/* Given a set of trial onset times, return the ideal response function */
{


  
}



vector<float> hrf_gam() {

  // https://afni.nimh.nih.gov/pub/dist/doc/program_help/3dDeconvolve.html
  
  // From AFNI documentation:
  // (t/(p*q))^p * exp(p-t/q)

  
}
