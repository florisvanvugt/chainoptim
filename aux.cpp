/* Some generic utilities */

#include <sstream>
#include <iterator>
#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric::ublas;


std::string floatvec2str(vector<float> vec,const char* delim) {
  // Source
  // http://stackoverflow.com/questions/8581832/converting-a-vector-to-string
  
  std::ostringstream oss;
  if (!vec.empty())
    {
      // Convert all but the last element to avoid a trailing ","
      std::copy(vec.begin(), vec.end()-1,
		std::ostream_iterator<float>(oss, delim));
      // Now add the last element with no delimiter
      oss << *vec.rbegin();
    }
  return oss.str();
    
}




std::string intvec2str(vector<int> vec,const char* delim) {
  // Source
  // http://stackoverflow.com/questions/8581832/converting-a-vector-to-string
  
  std::ostringstream oss;
  if (!vec.empty())
    {
      // Convert all but the last element to avoid a trailing ","
      std::copy(vec.begin(), vec.end()-1,
		std::ostream_iterator<int>(oss, delim));
      // Now add the last element with no delimiter
      oss << *vec.rbegin();
    }
  return oss.str();
    
}






vector<int> arange(int start, int end)
/* 
   Returns the vector 
   start, start+1, start+2, start+3, ..., end
*/
{
  vector<int> v(end-start);
  for (int i=start; i<end; ++i) {
    v[i-start]=i;
  }
  return v;
}


vector<float> scalmult(vector<int> vect, float scalar) 
/* Scalar multiplication of an int vector to yield a float vector. */
{
  vector<float> v(vect.size());
  for (unsigned i=0; i<vect.size(); ++i) {
    v[i] = vect[i]*scalar;
  }
  return v;
}

