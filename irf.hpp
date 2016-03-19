
/* This is the Ideal Response Function (IRF) for a particular design. */

#include <vector>

using namespace std;

class IdealResponse {
public:
  vector<float> evaluate(vector<float> t);
};



class GAM : IdealResponse {
public:
  GAM(vector<float> trialtimes);
private:
  vector<float> trialtimes;
};




