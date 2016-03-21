



/* 
   This class deals with making a chain of designs,
   each of which is an improvement upon the previous one,
   by making small local changes.
*/
#include <string>

#include <design.hpp>
//class Design; // Forward declaration: just tell them that this exists.


typedef struct StepResult {
  float efficiency; // the efficiency at the *beginning* of this step.
  int n_move_opportunities;
  int n_improving_moves;
  Move performed_move;
} StepResult;


class Chain {

private:
  float efficiency;
  int ntrials;
  float TR;
  int ntp;
  int npolort;
  int trial_duration;
  std::string hrftype;
  std::string move_choose;

  StepResult step(Design* current,bool verbose,ublas::vector<double> &scantimes,ublas::matrix<double> &baselineX);
  /* Performs one step in the iteration process:
     takes the current design and its efficiency and looks 
     for moves that improve upon it.
     Then it performs one move that improves and sets 
     the corresponding (updated) design.
     
     Arguments
     current : the current design (from the previous iteration step)
     verbose : whether to give verbose output
     scantimes : a vector of times at which scans take place
     baselineX : a design matrix with all the baseline regressors but without the regressor of interest
  */


public:
  Chain(int ntrials,int ntp,float TR,int trial_duration,int npolort,std::string hrftype,std::string move_choose);
  bool run(int maxiter,bool verbose);
  Design * result_design;
  int iteration;
  void history_to_file(const char* fname);
  std::vector<StepResult> history; // Keeps track of the iterations we performed
  
};




