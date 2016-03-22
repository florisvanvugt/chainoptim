

#include <sstream>
#include <fstream>

#include "chain.hpp"
#include "aux.hpp"


Chain::Chain()
{
  this->efficiency=0.0;
}




StepResult Chain::step(Design* current,
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
  if (config::verbose) {
    std::cout<<"Current design:\n";
    current->print();
    current->print_trial_onsets();
  }
  double efficiency = current->get_efficiency(scantimes,baselineX);
  if (config::verbose)
    std::cout<<"Current efficiency: "<<efficiency<<"\n";
  // TODO: we don't have to re-calculate the efficiency because we just calculated it in the previous step.
  
  /* Find possible moves (null-TRs that we can move left or right) */
  std::vector<Move> moves = current->try_moves(scantimes,baselineX);
  if (config::verbose) printmoves(moves);
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

    if (config::move_choose=="max") {

      /* If we only choose the maximum, let's restrict our choices
	 to the candidates achieving the maximum efficiency.
	 Note that in principle there may be multiple moves that
	 yield the maximum efficiency, in which case we'll choose
	 randomly from among those. */
      std::vector<Move> bestcands;
      if (config::verbose) {
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
      if (config::verbose) {
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




bool Chain::run()
/* 
   Runs this current chain and returns the final design.
*/
{

  Design design;
  design.randomise();
  
  if (config::verbose) {
    std::cout<<"\nRunning in verbose mode.\n";
    std::cout<<"\n\nInitiating chain\n";
    std::cout<<"Initial design:\n";
    design.print();
  }

  /* Prepare a vector with the time points of the scans so that we also don't have to re-calculate that every time */
  ublas::vector<double> scantimes = scalmult(arange(0,config::ntp),config::TR);

  /* Prepare a matrix with baseline regressors so that we don't have to calculate that anew every time */
  ublas::matrix<double> baselineX = polort(config::npolort,scantimes);

  
  bool keep_going = true;
  iteration=1;
  history.clear();
  
  while (keep_going) {

    if (config::verbose) {
      std::cout<<"\nITERATION "<<iteration<<"\n\n";
    };

    /* Perform the step */
    StepResult step = this->step(&design,scantimes,baselineX);

    if (config::verbose) {
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
      if (config::verbose) {
	std::cout<<"No more improving moves.\n";
      }
      keep_going = false;
    }
    iteration++;
    if ((config::maxiter>-1) && (iteration>config::maxiter)) {
      if (config::verbose) {
	std::cout<<"Reached maximum number of iterations ("<<config::maxiter<<")\n";
      }
      keep_going = false;
    }
    history.push_back(step);

  }

  // Install the final design as result
  this->result_design = new Design(design); // Make a copy of the design since it seems otherwise it got removed.
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
  dat<<"iteration n.move.opportunities n.improving.moves move.location move.dir move.amount efficiency design\n";
  for(iter=hist.begin() ; iter < hist.end(); iter++,i++ ) {
    Move mov = (*iter).performed_move;
    dat<<i<<" "<<(*iter).n_move_opportunities<<" "<<(*iter).n_improving_moves;
    dat<<" "<<mov.location;
    dat<<" "<<mov.direction;
    dat<<" "<<mov.amount;
    dat<<" "<<(*iter).efficiency;
    dat<<" \""<< mov.result->trial_onsets() <<"\"";
    dat<<std::endl;
  }
  dat.close();
}
