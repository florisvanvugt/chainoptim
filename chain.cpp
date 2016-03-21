

#include <sstream>
#include <fstream>

#include "chain.hpp"
#include "aux.hpp"


Chain::Chain(int ntrials,
	     int ntp,
	     float TR,
	     int trial_duration,
	     int npolort,
	     std::string hrftype,
	     std::string move_choose
	     ) 
{
  this->ntrials        = ntrials;
  this->TR             = TR;
  this->trial_duration = trial_duration;
  this->hrftype        = hrftype;
  this->ntp            = ntp;
  this->npolort        = npolort;
  this->move_choose    = move_choose;
}




StepResult Chain::step(Design* current,
		       bool verbose,
		       ublas::vector<double> &scantimes,
		       ublas::matrix<double> &baselineX)
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
{

  /* First find the efficiency of our current design. */
  double efficiency = current->get_efficiency(this->hrftype,this->ntp,this->TR,baselineX);
  if (verbose)
    std::cout<<"Current efficiency: "<<efficiency<<"\n";
  // TODO: we don't have to re-calculate the efficiency because we just calculated it in the previous step.
  
  /* Find possible moves (null-TRs that we can move left or right) */
  std::vector<Move> moves = current->try_moves(this->hrftype,scantimes,baselineX);
  if (verbose) printmoves(moves);
  StepResult step;
  step.n_move_opportunities = moves.size();
  step.efficiency = efficiency;

  /*
    Decide which moves improve, throw away the others.
  */
  std::vector<Move> candidates;
  std::vector<Move>::iterator it;
  int nkept = 0;
  double besteff = 0.0; // the best efficiency found so far
  for (it=moves.begin(); it<moves.end(); it++) {
    /* If this move increases efficiency... */
    if (it->efficiency > efficiency) {
      //std::cout<<"Keeping";
      candidates.push_back(*it);
      nkept += 1;

      /* If this is as good as we've ever had it, update */
      if (it->efficiency>besteff) {
	besteff = it->efficiency;
      }
    } else {
      delete (it->result); // Clear the memory taken up by the design that we are not going in to
    }
  }
  // TODO: Remove the moves list itself??
  step.n_improving_moves = nkept;
  
  if (nkept>0) {

    if (move_choose=="max") {

      /* If we only choose the maximum, let's restrict our choices
	 to the candidates achieving the maximum efficiency.
	 Note that in principle there may be multiple moves that
	 yield the maximum efficiency, in which case we'll choose
	 randomly from among those. */
      std::vector<Move> bestcands;
      if (verbose) {
	std::cout<<"Restricting to best candidates\n";
      }
      nkept = 0; // go back to square 1
      for (it=candidates.begin(); it<candidates.end(); it++) {
	/* If this move increases efficiency... */
	if (it->efficiency == besteff) {
	  //std::cout<<"Keeping";
	  bestcands.push_back(*it);
	  nkept++;
	}
      }
      candidates = bestcands;
      if (verbose) {
	std::cout<<"Kept "<<candidates.size()<<"candidates.\n";
      }
    }
    
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




bool Chain::run(int maxiter,bool verbose)
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

  /* Prepare a vector with the time points of the scans so that we also don't have to re-calculate that every time */
  ublas::vector<double> scantimes = scalmult(arange(0,this->ntp),this->TR);

  /* Prepare a matrix with baseline regressors so that we don't have to calculate that anew every time */
  ublas::matrix<double> baselineX = polort(this->npolort,scantimes);

  
  bool keep_going = true;
  iteration=1;
  history.clear();
  
  while (keep_going) {

    if (verbose) {
      std::cout<<"\nITERATION "<<iteration<<"\n\n";
    };

    /* Perform the step */
    StepResult step = this->step(&design,verbose,scantimes,baselineX);

    if (verbose) {
      print_stepresult(step);
    } else {
      std::cout<<iteration<<" ("<<step.n_improving_moves<<") ";
    };
    std::cout.flush();

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
    history.push_back(step);

  }

  // Install the final design as result
  this->result_design = new Design(design); // Make a copy of the design so that it doesn't get removed.
  //this->result_design->print();

  return true;
  
}





void print_stepresult(StepResult step)
{
  std::cout<<"Step: improv "<<step.n_improving_moves<<" / "<<step.n_move_opportunities<<" opportunities.\n";
  //step.performed_move.result.print();
}




void Chain::history_to_file(const char* fname)
/* 
   Write the history of this chain (the iterations) to a file.
*/
{
  std::ofstream dat(fname);
  unsigned i=0;
  std::vector<StepResult>::iterator iter;
  std::vector<StepResult> hist = this->history;
  dat<<"iteration n.move.opportunities n.improving.moves efficiency design\n";
  for(iter=hist.begin() ; iter < hist.end(); iter++,i++ ) {
    dat<<i<<" "<<(*iter).n_move_opportunities<<" "<<(*iter).n_improving_moves<<" "<<(*iter).efficiency;
    dat<<" \""<< (*iter).performed_move.result->trial_onsets() <<"\"";
    dat<<std::endl;
  }
  dat.close();
}
