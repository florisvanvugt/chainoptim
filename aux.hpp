

#include <boost/numeric/ublas/vector.hpp>

namespace ublas = boost::numeric::ublas;

std::string intvec2str  (ublas::vector<int> vec,  const char* delim);
std::string floatvec2str(ublas::vector<double> vec,const char* delim);

ublas::vector<int> arange(int start, int end);
ublas::vector<double> scalmult(ublas::vector<int> vect, double scalar);

ublas::matrix<double> polort(int npolort, ublas::vector<double> times);

void matrix_to_file(ublas::matrix<double> mat, char* fname, char* delim);

bool MInvBoost(matrix<double> InputMatrix, matrix<double> &InverseMatrix);

int randint(int range);
