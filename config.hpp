#include <string>


/* Configuration options that will stay the same throughout our run 
   (although they come from the command-line options) */

namespace config {

  /* The number of volumes (measurements) */
  extern int ntp;
  
  /* The TR duration (acquisition time for a single volume in seconds) */
  extern float TR;
  
  /* The number of trials */
  extern int ntrials;
  
  /* The duration of a trial (in # of TRs) */
  extern int trial_duration;
  
  /* The duration of a trial (in seconds) */
  extern float trial_duration_sec;
  
  /* The hypothesised HRF function */
  extern std::string hrf;
  
  /* The number of orthogonal polynomials to stick into the design matrix */
  extern int npolort;
  
  /* The maximum number of iterations we will try, after which we terminate the chain. (-1 means there is no maximum) */
  extern int maxiter;
  
  /* initialize random seed: */
  extern unsigned random_seed;

  /* How to choose the movement at each iteration: randomly from all movements that improve or always take the maximum. */
  extern std::string move_choose;
  
  /* AFNI-style 1D output file with stimulus times. */
  extern std::string afniout;
  
  /* Write the final design matrix to an output file. */
  extern std::string xout;
  
  /* Write a listing of the iterations */
  extern std::string history;

  /* Whether we can move a whole stack of null time at a time */
  extern bool stack_move;

  /* Whether we can move null time far.
     Let me explain. If we have a set of null-time like this:

     [ 15, 0, 0, 3, 0, 0, 0 ]
     Then if far_move is enabled then we are in principle allowed to skip the 0s and make a move to this:
     [ 14, 0, 0, 4, 0, 0, 0 ]
     Effectively what this does is it moves trials over arbitrarily long null space.
   */
  extern bool far_move;


  /* Verbose output */
  extern bool verbose;

  void pre_report();

}


