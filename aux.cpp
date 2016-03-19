/* Some generic utilities */

#include <sstream>
#include <iterator>
#include <vector>

using namespace std;

string floatvec2str(vector<float> vec,const char* delim) {
  // Source
  // http://stackoverflow.com/questions/8581832/converting-a-vector-to-string
  
  std::ostringstream oss;
  if (!vec.empty())
    {
      // Convert all but the last element to avoid a trailing ","
      std::copy(vec.begin(), vec.end()-1,
		std::ostream_iterator<float>(oss, delim));
      // Now add the last element with no delimiter
      oss << vec.back();
    }
  return oss.str();
    
}




string intvec2str(vector<int> vec,const char* delim) {
  // Source
  // http://stackoverflow.com/questions/8581832/converting-a-vector-to-string
  
  std::ostringstream oss;
  if (!vec.empty())
    {
      // Convert all but the last element to avoid a trailing ","
      std::copy(vec.begin(), vec.end()-1,
		std::ostream_iterator<int>(oss, delim));
      // Now add the last element with no delimiter
      oss << vec.back();
    }
  return oss.str();
    
}
