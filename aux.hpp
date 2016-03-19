

#include <boost/numeric/ublas/vector.hpp>


std::string intvec2str  (boost::numeric::ublas::vector<int> vec,  const char* delim);
std::string floatvec2str(boost::numeric::ublas::vector<float> vec,const char* delim);

boost::numeric::ublas::vector<int> arange(int start, int end);
boost::numeric::ublas::vector<float> scalmult(boost::numeric::ublas::vector<int> vect, float scalar);
