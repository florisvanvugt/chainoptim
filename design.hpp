#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "irf.hpp"

namespace ublas = boost::numeric::ublas;


class Design;

// Declare a type for a move (small change in a design)
typedef struct Move {
  int location;   // the null-TR location
  int direction;  // where to move to: -1 means to move one back, +1 means to move one forward.
  int amount;     // how many null-TRs to move.
  
  Design* result; // the resulting design (when this move is performed)
  double efficiency; // the efficiency of the resulting design.
  Move(): location(-1), direction(0), amount(1), efficiency(-1) {};
  Move(int loc, int dir) : amount(1), efficiency(-1) {
    location=loc;
    direction=dir;
  };
  Move(int loc, int dir, int am) : efficiency(-1) {
    location=loc;
    direction=dir;
    amount=am;
  };
} Move;
void printmoves(std::vector<Move> moves);


class Design {

private:
  int ntrials;
  float tr;
  int trial_duration;
  vector<int> nulltrs;
  
public:
  Design(int ntrials,float tr,int trial_duration);
  void print();
  void randomise(int n_null_tp);
  vector<double> get_trial_onsets();
  void print_trial_onsets();
  std::string trial_onsets();
  IRF * get_irf(std::string hrftype); // This should probably be private later on

  /* These functions compute the efficiency of the design.
     You can either supply a design matrix ready-made, 
     or else it will build this for you. */
  double get_efficiency(std::string hrftype,int ntp,float TR,int npolort);
  double get_efficiency(std::string hrftype,int ntp,float TR,ublas::matrix<double> &X);
  double get_efficiency(std::string hrftype,ublas::vector<double> &scantimes,ublas::matrix<double> &baselineX);
  /* Careful: the design matrix X is given but it doesn't have the first column (corresponding to the regressor of interest) filled out. */
  /* 
   Calculate the efficiency of this design.
   
   Arguments
   hrftype : hypothesised HRF (e.g. "gam")
   ntp : number of time points (scans)
   TR : duration of volume acquisition (in seconds)
   npolort : number of orthogonal polynomials to use
  */

  void add_irf_to_design_matrix(std::string hrftype,int ntp,float TR,ublas::matrix<double> &X);
  void add_irf_to_design_matrix(std::string hrftype,ublas::vector<double> &scantimes,ublas::matrix<double> &X);
  /* Takes an existing design matrix and sticks in the IRF as the first regressor. The typical
     usage of this function will be that you first build a matrix with baseline regressors and then
     you only need to stick in this regressor.*/
    

  ublas::matrix<double> get_matrix(ublas::vector<double> &scantimes);
  /* Constructs a design matrix for the given scantimes */

  ublas::matrix<double> get_matrix(std::string hrftype,int ntp,float TR,int npolort);
  /* DEPRECATED */

  void to_afni_file(const char* fname);

  /* Find possible moves, tries them, and returns the efficiency */
  std::vector<Move> try_moves(std::string hrf,
			      ublas::vector<double> &scantimes,
			      ublas::matrix<double> &baselineX);
  /* Tries moving a number "amount" of nulltrs at the given location, in the given direction */
  Move try_move(int location,int direction,int amount,
		std::string hrf,
		ublas::vector<double> &scantimes,
		ublas::matrix<double> &baselineX);
  //Move try_move(int location,int direction,std::string hrf,int ntp,int npolort);

  /* Move the nullTR at the given location in the given direction.
     For example, move(5,-1) moves one nullTR at location 5 to the left (-1) */
  Design* move(int location,int direction,int amount); 

};



