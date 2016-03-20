#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "irf.hpp"

using namespace boost::numeric::ublas;


class Design;

// Declare a type for a move (small change in a design)
typedef struct Move {
  int location; // the null-TR location
  int direction; // where to move to: -1 means to move one back, +1 means to move one forward.

  Design* result; // the resulting design (when this move is performed)
  double efficiency; // the efficiency of the resulting design.
  Move(): location(-1), direction(0), efficiency(-1) {};
  Move(int loc, int dir) : efficiency(-1) {
    location=loc;
    direction=dir;
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
  IRF* get_irf(std::string hrftype); // This should probably be private later on
  double get_efficiency(std::string hrftype,int ntp,float TR,int npolort);
  matrix<double> get_matrix(std::string hrftype,int ntp,float TR,int npolort);
  
  std::vector<Move> find_moves(std::string hrf,int ntp,int npolort);
  Design* move(int location,int direction);
  Move try_move(int location,int direction,std::string hrf,int ntp,int npolort);
};



