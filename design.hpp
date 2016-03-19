#include <vector>
#include "irf.hpp"

using namespace std;

class Design {

private:
  int ntrials;
  vector<int> nulltrs;
  
public:   
  void print();
  Design(int ntrials);
  void randomise(int n_null_tp);
  vector<float> get_trial_onsets(float tr);
  void print_trial_onsets(float tr);
  string trial_onsets(float tr);
  IRF* get_irf(float tr,string hrftype);
};

