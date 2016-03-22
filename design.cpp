/* 
This is an experimental design.
 */

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <fstream>

#include "design.hpp"
#include "aux.hpp"


namespace ublas = boost::numeric::ublas;

const char *delim = ", ";


Design::Design() {
  // Initialise the ntrials array
  ublas::vector<int> nt(config::ntrials+1);
  /* Initialise to zero */
  for (unsigned i=0; i<nt.size(); ++i) {
    nt[i]=0;
  }
  this->nulltrs = nt;
}


void Design::print()
/* Print the contents of this design */
{
  std::cout << "(" << config::ntrials << " trials) [";
  std::cout << intvec2str(this->nulltrs,delim);
  std::cout << "]\n";
}






void Design::randomise()
/* 
   Randomly initialise this design by inserting 
   the number of null TRs at random positions.
*/
{
  //std::cout<<"Randomly allocating "<<n_null_tp<<" null TRs\n";
  if (config::verbose)
    std::cout<<"Randomising design.\n";
  int n_null_tp = config::ntp-(config::ntrials*config::trial_duration);
  if (config::verbose)
    std::cout<<"# Null-time points:"<<n_null_tp<<"\n";

  // First make sure we empty the null TR list (just in case)
  for (int i=0; i<(config::ntrials)+1; ++i) {
    this->nulltrs[i]=0;
  }
  std::cout<<"Distributing...\n";

  // Randomly distribute the null TRs
  int bin;
  for (int i=0; i<n_null_tp; ++i) {
    // Decide which bin we will add a null TR to
    bin = randint((config::ntrials)+1); // produces a number 0..(ntrials)
    this->nulltrs[bin] += 1;
  }
  std::cout<<"... complete.\n";

  // Now sum(this->nulltrs) should equal n_null_tp
  
}








ublas::vector<double> Design::get_trial_onsets()
/* 
   Returns the trial onsets of the given design.
   
   Arguments
   tr : the TR duration (in seconds) 
*/
{
  ublas::vector<double> trialtimes(config::ntrials);
  int preceding_trs = 0;
  for (int i=0; i<config::ntrials; ++i) {
    preceding_trs += this->nulltrs[i];
    trialtimes[i] =  config::TR*preceding_trs;
    preceding_trs += config::trial_duration;
  }
  return (trialtimes);
}



void Design::print_trial_onsets()
/* 
   Prints the trial onsets of the given design.
*/
{
  std::cout<<this->trial_onsets();
}




std::string Design::trial_onsets()
/* Returns the trial onset times for this design. */
{
  // Source: http://stackoverflow.com/questions/8581832/converting-a-vector-to-string
  ublas::vector<double> vec = this->get_trial_onsets();
  return floatvec2str(vec,delim);
}






IRF* Design::get_irf()
/* 
   Return the ideal response function for this design.
   
   Arguments
   tr : TR duration (in seconds)
   hrftype : type of HRF hypothesised (e.g. GAM)
*/
{
  IRF *irf;
  if (config::hrf=="gam") {
    irf = new GAM(this->get_trial_onsets());
  }
  //if (hrftype=="gam") {
  //return 
  //  }
  return (irf);
}






void Design::add_irf_to_design_matrix(ublas::matrix<double> &X)
{
  /* First get the regressor of interest, namely the predicted BOLD response */
  ublas::vector<double> scantimes = scalmult(arange(0,config::ntp),config::TR);
  //std::cout<< "Scan times:" << doublevec2str(scantimes," ") << "\n";
  this->add_irf_to_design_matrix(scantimes,X);
}



void Design::add_irf_to_design_matrix(ublas::vector<double> &scantimes,ublas::matrix<double> &X) 
{
  IRF* irf = this->get_irf();
  ublas::vector<double> scans = irf->evaluate(scantimes);
  //std::cout<< "Scan values:" << doublevec2str(scans," ") << "\n";
  // Stick our regressor of interest into the design matrix first column
  for (unsigned i=0; i<scans.size(); ++i) {
    X(i,0) = scans[i];
  }
}





ublas::matrix<double> Design::get_matrix()
/* 
   Get the design matrix for this design 
*/
{

  /* First get the regressor of interest, namely the predicted BOLD response */
  ublas::vector<double> scantimes = scalmult(arange(0,config::ntp),config::TR);

  /* Now get the remaining orthogonal polynomials */
  ublas::matrix<double> X = polort(config::npolort,scantimes);

  // Add the regressor of interest to the matrix.
  this->add_irf_to_design_matrix(scantimes,X);

  return X;
  
}









std::vector<Move> Design::try_moves(ublas::vector<double> &scantimes,
				    ublas::matrix<double> &baselineX)
/* 
Find the possible moves (moving around null-TRs) for this design,
and calculates what the efficiency of the resulting designs are.
*/
{
  std::vector<Move> moves;
  Move move;

  /* First build an inventory of possible moves, without
     actually trying them */
  std::vector<unsigned> amounts;
  std::vector<int> directions;
  std::vector<unsigned>::iterator am;
  std::vector<int>::iterator dr;

  /* Ok, now we will figure out which moves we can make. */
  for (unsigned i=0; i<this->nulltrs.size(); ++i) {

    /* The question is, how much can we move from the null-times at location i? */
    unsigned n = this->nulltrs[i];
    amounts.clear();
    if (n>0)
      amounts.push_back(1); // we can move just one null-time unit
    if (n>1 && config::stack_move)
      amounts.push_back(n); // we can move all null-time units, if stack_move is allowed.

    /* The next question, where can we move it to? 
       If we are on the left edge then we can't move it there, similarly for the right edge */
    directions.clear();
    int target; // where we are going to move to
    if (i>0) {
      target = i-1;
      directions.push_back(target); // we can move to the left

      if (config::far_move) {
	int j=target;
	// we can move across arbitrarily large portions of space as long as we don't hit another block
	while (j>=0 && (this->nulltrs[j]==0)) {
	  j--;
	}
	if (j<0) j=0; // We may have fallen below zero
	if (j!=target) // If this is actually not the same as i-1
	  directions.push_back(j);
      }

    }
    if (i<this->nulltrs.size()-1) {
      target = i+1;
      directions.push_back(target); // we can move to the right

      if (config::far_move) {
	int j=target;
	// we can move across arbitrarily large portions of space as long as we don't hit another block
	while (j<(int)nulltrs.size() && (this->nulltrs[j]==0)) {
	  j++;
	}
	if (j>=(int)nulltrs.size()) j=(int)nulltrs.size()-1; // We may have considered moving outside of the array
	if (j!=target) // If this is actually not the same as i-1
	  directions.push_back(j);
      }
    }

    
    for (am=amounts.begin() ; am < amounts.end(); am++ ) {
      for (dr=directions.begin() ; dr < directions.end(); dr++ ) {
	moves.push_back( this->try_move(i,*dr,*am,scantimes,baselineX) );
      }
    }

  }
  
  return moves;
}




Design* Design::move(int location,int target,int amount)
/* Return a copy of the current design but with a particular move applied. */
{
  Design* moved = new Design(*this);
  // Now apply the move
  moved->nulltrs[location]-=amount;
  moved->nulltrs[target]+=amount;
  /* TODO: Make sure we copy everything relevant from the old design */
  return moved;
}



/*Move Design::try_move(int location,int direction,std::string hrf,int ntp,int npolort) */
Move Design::try_move(int location,int target,int amount,
		      ublas::vector<double> &scantimes,
		      ublas::matrix<double> &baselineX)
/* Try a particular move (changing the location of null TRs)
   and see what the new efficiency is. Return this is a move. */
{
  /* Make a new move object to tell the world about what came out. */
  Move move = Move(location,target,amount);

  /* Perform the move in the design, which will return a new design object with the move applied. */
  Design* manip = this->move(location,target,amount);
  move.result = manip; /* Set this pointer */

  /* Now calculate the efficiency */
  manip->add_irf_to_design_matrix(scantimes,baselineX);
  move.efficiency = efficiency_for_design(baselineX,0);

  return move;
}





double Design::get_efficiency(ublas::vector<double> &scantimes,ublas::matrix<double> &baselineX) 
{
  /* Stick the regressor of interest into the baselineX matrix (this modifies the baselineX matrix).*/
  this->add_irf_to_design_matrix(scantimes,baselineX);
  
  /* Now calculate the efficiency */
  double eff = efficiency_for_design(baselineX,0);
  return eff;
}



double Design::get_efficiency(ublas::matrix<double> &baselineX)
{
  /* Calculate when the scan times occur */
  ublas::vector<double> scantimes = scalmult(arange(0,config::ntp),config::TR);
  return this->get_efficiency(scantimes,baselineX);
}




double Design::get_efficiency()
{
  /* Get our design matrix */
  ublas::matrix<double> X = this->get_matrix();
  return this->get_efficiency(X);
}








void printmoves(std::vector<Move> moves)
/* Prints a list of moves to standard out. */
{
  std::cout<<"Moves: [\n";
  for (unsigned i=0; i<moves.size(); ++i) {
    //std::cout<<moves[i].location<<"("<<moves[i].direction<<")-->"<<moves[i].efficiency<<" ";
    std::cout<<moves[i].location<<"("<<moves[i].direction<<") "<<moves[i].amount<<" -->";
    moves[i].result->print();
    std::cout<<"\t eff "<<moves[i].efficiency<<"\n";
  }
  std::cout<<"]\n";
}





void Design::to_afni_file(const char* fname)
/* Write the final design to an AFNI-style output file. */
{
  ublas::vector<double> stimtimes = this->get_trial_onsets();
  std::string outdata = floatvec2str(stimtimes,(char*)" ");
  std::ofstream dat(fname);
  //dat<<"1D: ";
  dat<<outdata<<"\n";
  dat.close();
}
