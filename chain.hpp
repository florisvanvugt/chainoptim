



/* 
   This class deals with making a chain of designs,
   each of which is an improvement upon the previous one,
   by making small local changes.
*/
#include <string>

#include <design.hpp>
//class Design; // Forward declaration: just tell them that this exists.

class Chain {

private:
  float efficiency;
  int ntrials;
  float TR;
  int ntp;
  int npolort;
  int trial_duration;
  std::string hrftype;

public:
  Chain(int ntrials,int ntp,float TR,int trial_duration,int npolort,std::string hrftype);
  Design run();

};


