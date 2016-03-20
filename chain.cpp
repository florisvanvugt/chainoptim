


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




Design Chain::run()
/* Runs this current chain and returns the final design */
{

  Design design(this->ntrials,this->TR,this->trial_duration);
  
  int n_null_trs = ntp-(this->ntrials*this->trial_duration);
  design.randomise(n_null_trs);

  design.print();
  
  this->efficiency = design.get_efficiency(this->hrftype,this->ntp,this->TR,this->npolort);

  std::vector<Move> moves = design.findMoves();
  printmoves(moves);

  // TODO: get possible moves

  // Decide which moves improve

  // Perform the move...
  return design;
  
}

