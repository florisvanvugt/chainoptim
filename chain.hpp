



/* 
   This class deals with making a chain of designs,
   each of which is an improvement upon the previous one,
   by making small local changes.
*/

#include <design.hpp>


class Chain {

private:
  float efficiency;
  int ntrials;
  float TR;
  int ntp;
  int npolort;
  int trial_duration;
  std::string hrftype;
  Design* current;

public:
  Chain(int ntrials,int ntp,float TR,int trial_duration,int npolort,std::string hrftype);
  void run();

};


