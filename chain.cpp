


#include "chain.hpp"


Chain::Chain(int ntrials,
	     int ntp,
	     float TR,
	     int trial_duration,
	     int npolort,
	     std::string hrftype
	     ) 
{
  this->ntrials        = ntrials;
  this->TR             = TR;
  this->trial_duration = trial_duration;
  this->hrftype        = hrftype;
  this->ntp            = ntp;
  this->npolort        = npolort;
}




void Chain::run() {

  Design design(this->ntrials,this->TR,this->trial_duration);
  //this->current = *design;
  
  int n_null_trs = ntp-(this->ntrials*this->trial_duration);
  design.randomise(n_null_trs);

  this->efficiency = design.get_efficiency(this->hrftype,this->ntp,this->TR,this->npolort);

  // TODO: get possible moves

  // Decide which moves improve

  // Perform the move...
  
}

