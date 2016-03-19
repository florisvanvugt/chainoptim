#include <vector>
#include "irf.hpp"

using namespace std;

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
  string trial_onsets();
  IRF* get_irf(string hrftype);
};

