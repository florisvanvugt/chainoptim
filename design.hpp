#include <vector>


using namespace std;

class Design {

private:
  int ntrials;
  vector<int> nulltrs;
  
public:   
  void print();
  Design(int ntrials);
  void randomise(int n_null_tp);
};

