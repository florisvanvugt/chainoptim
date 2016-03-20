



/* 
   This class deals with making a chain of designs,
   each of which is an improvement upon the previous one,
   by making small local changes.
*/
#include <string>

#include <design.hpp>
//class Design; // Forward declaration: just tell them that this exists.


typedef struct StepResult {
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
  StepResult step( Design *current, bool verbose );
  std::vector<StepResult> history; // Keeps track of the iterations we performed

public:
  Chain(int ntrials,int ntp,float TR,int trial_duration,int npolort,std::string hrftype,std::string move_choose);
  bool run(int maxiter,bool verbose);
  Design * result_design;
  int iteration;
  void history_to_file(const char* fname);
  
};




