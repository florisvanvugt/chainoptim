

#include <boost/numeric/ublas/vector.hpp>

namespace ublas = boost::numeric::ublas;

std::string intvec2str  (ublas::vector<int> vec,  const char* delim);
std::string floatvec2str(ublas::vector<float> vec,const char* delim);

ublas::vector<int> arange(int start, int end);
ublas::vector<float> scalmult(ublas::vector<int> vect, float scalar);

ublas::matrix<float> polort(int npolort, ublas::vector<float> times);

void matrix_to_file(ublas::matrix<double> mat, char* fname, char* delim);

bool MInvBoost(matrix<float> InputMatrix, matrix<float> &InverseMatrix);

