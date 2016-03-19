#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "irf.hpp"

using namespace boost::numeric::ublas;

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
  vector<float> get_trial_onsets();
  void print_trial_onsets();
  std::string trial_onsets();
  IRF* get_irf(std::string hrftype); // This should probably be private later on
  float get_efficiency(std::string hrftype,int ntp,float TR,int npolort);
  matrix<double> get_matrix(std::string hrftype,int ntp,float TR,int npolort);
};

