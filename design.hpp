#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "irf.hpp"

using namespace boost::numeric::ublas;


// Declare a type for a move (small change in a design)
typedef struct {
  int location; // the null-TR location
  int direction; // where to move to: -1 means to move one back, +1 means to move one forward.

  double efficiency; // the efficiency after applying this move
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
  IRF* get_irf(std::string hrftype); // This should probably be private later on
  double get_efficiency(std::string hrftype,int ntp,float TR,int npolort);
  matrix<double> get_matrix(std::string hrftype,int ntp,float TR,int npolort);
  std::vector<Move> findMoves();
};



