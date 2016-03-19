/* Some generic utilities */

#include <sstream>
#include <fstream>
#include <iterator>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>


namespace ublas = boost::numeric::ublas;


std::string floatvec2str(ublas::vector<float> vec,const char* delim) {
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




std::string intvec2str(ublas::vector<int> vec,const char* delim) {
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






ublas::vector<int> arange(int start, int end)
/* 
   Returns the vector 
   start, start+1, start+2, start+3, ..., end
*/
{
  ublas::vector<int> v(end-start);
  for (int i=start; i<end; ++i) {
    v[i-start]=i;
  }
  return v;
}


ublas::vector<float> scalmult(ublas::vector<int> vect, float scalar) 
/* Scalar multiplication of an int vector to yield a float vector. */
{
  ublas::vector<float> v(vect.size());
  for (unsigned i=0; i<vect.size(); ++i) {
    v[i] = vect[i]*scalar;
  }
  return v;
}





ublas::matrix<float> polort(int npolort, ublas::vector<float> times)
/* 
   Returns an orthonormal Legendre basis.
   Also reserves space for a first empty column where we can
   stick our regressor of interest

   Arguments
   npolort : the number of orthogonal polynomials (the columns)
   times : the time points at which the polynomials are evaluated (the rows)
*/
{

  /*
    This is a nice and practical definition of what these polynomials are about!
    https://people.sc.fsu.edu/~jburkardt/cpp_src/legendre_polynomial/legendre_polynomial.html

    This uses Bonnet’s recursion formula.
    Source: https://en.wikipedia.org/wiki/Legendre_polynomials
  */

  ublas::matrix<float> mat(times.size(),npolort+2); // +2 because npolort=0 means we add one (constant) column
  
  /* Now let's go fill up the columns one by one */
  float minval = *std::min_element( times.begin(), times.end() );
  float maxval = *std::max_element( times.begin(), times.end() );
  //float x;

  for (unsigned j=0; j<times.size(); ++j)
    mat(j,0) = -1; // Leave the first column empty: we are going to stick our regressor of interest in there.


  //std::cout<<"Min"<<minval<<"\n";
  //std::cout<<"Max"<<maxval<<"\n";

  /*
    Recipe
    P(0,x) = 1
    P(1,x) = x
    P(n,x) = (2*n-1)/n * x * P(n-1,x) - (n-1)/n * P(n-2,x)
  */

  // Column iterator; we fill the matrix up column by column
  for (int n=0; n<npolort+1; ++n) {

    // Row iterator
    for (unsigned j=0; j<times.size(); ++j) {
      //std::cout<<"("<<n<<","<<j<<")";
      if (n==0) { // First order polynomial is constant (all one)
	mat(j,n+1) = 1; // Note that orthogonal polynomial n goes in location n+1
      } else if (n==1) { // Second-order polynomial is identity
	// Calculate the proportion of the time we've gone through
	// If times[j]==min(times) then x=-1
	// If times[j]==max(times) then x=+1
	// In between x will be linear with times[j].
	//t = times[j];
	mat(j,n+1) = 2*((times[j]-minval)/(maxval-minval))-1;
      } else { // Higher-order polynomial (n = order)
	// Note that mat(j,2) equals x (by definition)
	float n1 = (((2*(float)n-1)/(float)n));
	float n2 = ((float)n-1)/(float)n;
	float a = n1 * mat(j,2) * mat(j,n);
	float b = n2 * mat(j,n-1);
	//std::cout<<a<<" "<<b;
	mat(j,n+1) = a - b;
      }
      //std::cout<<"\n";
    }
    
  }
  
  return mat;
    
}








void matrix_to_file(ublas::matrix<double> mat, char* fname, char* delim) 
/* Write the matrix to a file */
{
  std::ofstream dat(fname);
  for (unsigned i = 0; i < mat.size1() ; i++) {
    for (unsigned j = 0; j < mat.size2() ; j++) {
      dat << mat(i, j) << delim; // Must seperate with Tab
    }
    dat << std::endl; // Must write on New line
  }
  dat.close();
}





// Simplied header for matrices of doubles only, no other types allowed
// Source: http://www.volopta.com/ComputerCode/CPlusPlus/Matrix_Operations_Boost.cpp
bool MInvBoost(ublas::matrix<float> InputMatrix, ublas::matrix<float> &InverseMatrix)
/* Usage: make sure you have defined and allocated the matrices Input and Inverse before you call this. */
{
  // Create a duplicate of the input matrix
  //std::cout<<"Defining";
  int N = InputMatrix.size1();
  ublas::matrix<float> A = InputMatrix;

  //std::cout<<"Perm";
  
  // Create the permutation matrix
  typedef ublas::permutation_matrix<std::size_t> pmatrix;
  pmatrix P(N);
  
  //std::cout<<"Assign";
  // Assign the identity matrix to the inverse
  InverseMatrix.assign(ublas::identity_matrix<float>(N));
  
  //std::cout<<"Factorise";
  //	LU factorization and substitution
  lu_factorize(A,P);
  //std::cout<<"Substitute";
  lu_substitute(A,P,InverseMatrix);
  return true;
}



