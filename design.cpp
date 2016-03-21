/* 
This is an experimental design.
 */

#include <iostream>
#include "design.hpp"
#include <stdio.h>
#include <cstdlib>
#include "aux.hpp"
#include <fstream>


namespace ublas = boost::numeric::ublas;

const char *delim = ", ";


Design::Design(int ntrials,float tr,int trial_duration) {
  this->ntrials = ntrials;
  this->tr = tr;
  this->trial_duration = trial_duration;
  
  // Initialise the ntrials array
  ublas::vector<int> nt(ntrials+1);
  this->nulltrs = nt;
}


void Design::print()
/* Print the contents of this design */
{
  std::cout << "(" << this->ntrials << " trials) [";
  std::cout << intvec2str(this->nulltrs,delim);
  std::cout << "]\n";
}






void Design::randomise(int n_null_tp)
/* 
   Randomly initialise this design by inserting 
   the number of null TRs at random positions.
*/
{
  //std::cout<<"Randomly allocating "<<n_null_tp<<" null TRs\n";

  // First make sure we empty the null TR list (just in case)
  for (int i=0; i<this->ntrials+1; ++i) {
    this->nulltrs[i]=0;
  }

  // Randomly distribute the null TRs
  int bin;
  for (int i=0; i<n_null_tp; ++i) {
    // Decide which bin we will add a null TR to
    bin = rand() % (this->ntrials+1); // produces a number 0..(ntrials)
    this->nulltrs[bin] += 1;
  }

  // Now sum(this->nulltrs) should equal n_null_tp
  
}








ublas::vector<double> Design::get_trial_onsets()
/* 
   Returns the trial onsets of the given design.
   
   Arguments
   tr : the TR duration (in seconds) 
*/
{
  ublas::vector<double> trialtimes(this->ntrials);
  int preceding_trs = 0;
  for (int i=0; i<this->ntrials; ++i) {
    preceding_trs += this->nulltrs[i];
    trialtimes[i] =  this->tr*preceding_trs;
    preceding_trs += trial_duration;
  }
  return (trialtimes);
}



void Design::print_trial_onsets()
/* Prints the trial onsets of the given design.

Arguments
tr : the TR duration (in seconds) 
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






IRF* Design::get_irf(std::string hrftype)
/* 
   Return the ideal response function for this design.
   
   Arguments
   tr : TR duration (in seconds)
   hrftype : type of HRF hypothesised (e.g. GAM)
*/
{
  IRF *irf;
  if (hrftype=="gam") {
    irf = new GAM(this->get_trial_onsets());
  }
  //if (hrftype=="gam") {
  //return 
  //  }
  return (irf);
}






void Design::add_irf_to_design_matrix(std::string hrftype,int ntp,float TR,ublas::matrix<double> &X)
{
  /* First get the regressor of interest, namely the predicted BOLD response */
  ublas::vector<double> scantimes = scalmult(arange(0,ntp),TR);
  //std::cout<< "Scan times:" << doublevec2str(scantimes," ") << "\n";
  this->add_irf_to_design_matrix(hrftype,scantimes,X);
}



void Design::add_irf_to_design_matrix(std::string hrftype,ublas::vector<double> &scantimes,ublas::matrix<double> &X) 
{
  IRF* irf = this->get_irf(hrftype);
  ublas::vector<double> scans = irf->evaluate(scantimes);
  //std::cout<< "Scan values:" << doublevec2str(scans," ") << "\n";
  // Stick our regressor of interest into the design matrix first column
  for (unsigned i=0; i<scans.size(); ++i) {
    X(i,0) = scans[i];
  }
}





ublas::matrix<double> Design::get_matrix(std::string hrftype,
					 int ntp,
					 float TR,
					 int npolort) 
/* 
   Get the design matrix for this design 

   Arguments
   hrftype : hypothesised HRF (e.g. "gam")
   ntp : number of time points (scans)
   TR : duration of volume acquisition (in seconds)
   npolort : number of orthogonal polynomials to use
*/
{

  /* First get the regressor of interest, namely the predicted BOLD response */
  ublas::vector<double> scantimes = scalmult(arange(0,ntp),TR);

  /* Now get the remaining orthogonal polynomials */
  ublas::matrix<double> X = polort(npolort,scantimes);

  // Add the regressor of interest to the matrix.
  this->add_irf_to_design_matrix(hrftype,ntp,TR,X);

  return X;
  
}





double Design::get_efficiency(std::string hrftype,int ntp,float TR,int npolort)
{
  /* Get our design matrix */
  ublas::matrix<double> X = this->get_matrix(hrftype,ntp,TR,npolort);
  return this->get_efficiency(hrftype,ntp,TR,X);
}






std::vector<Move> Design::try_moves(std::string hrf,
				    ublas::vector<double> &scantimes,
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
  for (unsigned i=0; i<this->nulltrs.size(); ++i) {
    if (this->nulltrs[i]>0) {
      if (i>0)
	// We can move it to the left
	moves.push_back( this->try_move(i,-1,hrf,scantimes,baselineX) );
      if (i<this->nulltrs.size()-1)
	// We can move it to the right
	moves.push_back( this->try_move(i,+1,hrf,scantimes,baselineX) );
    }
  }
  
  return moves;
}




Design* Design::move(int location,int direction)
/* Return a copy of the current design but with a particular move applied. */
{
  Design* moved = new Design(*this);
  // Now apply the move
  moved->nulltrs[location]-=1;
  moved->nulltrs[location+direction]+=1;
  /* TODO: Make sure we copy everything relevant from the old design */
  return moved;
}



/*Move Design::try_move(int location,int direction,std::string hrf,int ntp,int npolort) */
Move Design::try_move(int location,int direction,
		      std::string hrftype,
		      ublas::vector<double> &scantimes,
		      ublas::matrix<double> &baselineX)
/* Try a particular move (changing the location of null TRs)
   and see what the new efficiency is. Return this is a move. */
{
  /* Make a new move object to tell the world about what came out. */
  Move move = Move(location,direction);

  /* Perform the move in the design, which will return a new design object with the move applied. */
  Design* manip = this->move(location,direction);
  move.result = manip; /* Set this pointer */

  /* Now calculate the efficiency */
  manip->add_irf_to_design_matrix(hrftype,scantimes,baselineX);
  move.efficiency = efficiency_for_design(baselineX,0);

  return move;
}





double Design::get_efficiency(std::string hrftype,ublas::vector<double> &scantimes,ublas::matrix<double> &baselineX) 
{
  /* Stick the regressor of interest into the baselineX matrix (this modifies the baselineX matrix).*/
  this->add_irf_to_design_matrix(hrftype,scantimes,baselineX);
  
  /* Now calculate the efficiency */
  double eff = efficiency_for_design(baselineX,0);
  return eff;
}



double Design::get_efficiency(std::string hrftype,int ntp,float TR,ublas::matrix<double> &baselineX)
{
  /* Calculate when the scan times occur */
  ublas::vector<double> scantimes = scalmult(arange(0,ntp),TR);
  return this->get_efficiency(hrftype,scantimes,baselineX);
}










void printmoves(std::vector<Move> moves)
/* Prints a list of moves to standard out. */
{
  std::cout<<"Moves: [\n";
  for (unsigned i=0; i<moves.size(); ++i) {
    //std::cout<<moves[i].location<<"("<<moves[i].direction<<")-->"<<moves[i].efficiency<<" ";
    std::cout<<moves[i].location<<"("<<moves[i].direction<<")-->";
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
  dat<<"1D: "<<outdata<<"\n";
  dat.close();
}
