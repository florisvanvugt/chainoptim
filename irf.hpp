
/* This is the Ideal Response Function (IRF) for a particular design. */

#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric::ublas;

/* This is an abstract class that tells us */
class IRF {
public:
  virtual vector<float> evaluate(vector<float> t) = 0; // 'pure virtual' function, meaning that it must be overriden in a derived class.
};



class GAM : public IRF {
public:
  GAM(vector<float> trialtimes);
  vector<float> evaluate(vector<float> t); // overriding the base
private:
  vector<float> trialtimes;
};




