


#include "chain.hpp"
#include "aux.hpp"


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




StepResult Chain::step(Design* current,bool verbose)
/* Performs one step in the iteration process:
   takes the current design and its efficiency and looks 
   for moves that improve upon it.
   Then it performs one move that improves and sets 
   the corresponding (updated) design.
*/
{

  /* First find the efficiency of our current design. */
  double efficiency = current->get_efficiency(this->hrftype,this->ntp,this->TR,this->npolort);
  if (verbose)
    std::cout<<"Current efficiency: "<<efficiency<<"\n";
  
  /* Find possible moves (null-TRs that we can move left or right) */
  std::vector<Move> moves = current->find_moves(this->hrftype,this->ntp,this->npolort);
  if (verbose) printmoves(moves);
  StepResult step;
  step.n_move_opportunities = moves.size();

  /*
    Decide which moves improve, throw away the others.
  */
  std::vector<Move> candidates;
  std::vector<Move>::iterator it;
  int nkept = 0;
  for (it=moves.begin(); it<moves.end(); it++) {
    /* If this move increases efficiency... */
    if (it->efficiency > efficiency) {
      //std::cout<<"Keeping";
      candidates.push_back(*it);
      nkept += 1;
    } else {
      delete (it->result); // Clear the memory taken up by the design that we are not going in to
    }
  }
  // TODO: Remove the moves list itself??
  step.n_improving_moves = nkept;
  
  if (nkept>0) {
  
    // Choose a move from among the candidates
    int chosen = randint(nkept);
    
    // Chosen candidates[chosen-1]; --> need to pull out efficiency and design from that.
    step.performed_move = candidates[chosen];
    //Design* result = candidates[chosen-1].result;
    // We could use the efficiency already calculated: candidates[chosen-1].efficiency;

    // TODO Destroy the other objects (that we are not moving into)
  };
  
  return step;
  
}



void print_stepresult(StepResult step);


Design Chain::run(int maxiter,bool verbose)
/* 
   Runs this current chain and returns the final design.

   Arguments
   maxiter : the maximum number of iterations, after which we will stop anyway even if we haven't yet exhausted all possible moves that improve the design.
*/
{

  Design design(this->ntrials,this->TR,this->trial_duration);
  
  int n_null_trs = ntp-(this->ntrials*this->trial_duration);
  design.randomise(n_null_trs);

  if (verbose) {
    std::cout<<"\nRunning in verbose mode.\n";
    std::cout<<"\n\nInitiating chain\n";
    std::cout<<"Initial design:\n";
    design.print();
  }
  
  bool keep_going = true;
  int iteration=1;
  while (keep_going) {

    if (verbose) {
      std::cout<<"\nITERATION "<<iteration<<"\n\n";
    } else {
      std::cout<<".";
    }
    StepResult step = this->step(&design,verbose);
    if (verbose) {
      print_stepresult(step);
    }
    
    if (step.n_improving_moves>0) {
      // delete *design; // Get rid of the "old" design TODO
      // Take this as the new design
      design = *(step.performed_move.result);
    } else {
      if (verbose) {
	std::cout<<"No more improving moves.\n";
      }
      keep_going = false;
    }
    iteration++;
    if ((maxiter>-1) && (iteration>maxiter)) {
      if (verbose) {
	std::cout<<"Reached maximum number of iterations ("<<maxiter<<")\n";
      }
      keep_going = false;
    }

  }
  
  return design;
  
}





void print_stepresult(StepResult step)
{
  std::cout<<"Step: improv "<<step.n_improving_moves<<" / "<<step.n_move_opportunities<<" opportunities.\n";
  //step.performed_move.result.print();
}
