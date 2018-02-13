#pragma once
#include <vector>
#include <string>
#include <memory>
#include <assert.h>
#include <initializer_list>
#include <math.h>

//%TODO: Template this to take data types other than double and restrict the types taken to be numeric using:
// https://stackoverflow.com/questions/14294267/class-template-for-numeric-types
class cVec
{
	// Data Members
private:
	int len; // Vector rows
public:
	std::unique_ptr<double[]> data; //Unique pointer so it kills itself and can't be aliased

	//Constructor/Destructors
public:
	explicit cVec(int len_init = 3); // Create uninitialized
	cVec(const cVec& c); // Copy
	cVec(std::initializer_list<double>values); // Copy
	~cVec();


public:
	friend std::ostream& operator<<(std::ostream&, const cVec&); //Print
	cVec& operator=(const cVec& c);

	// Elementwise Ops
	cVec operator+(const cVec& rhs);
	cVec operator-(const cVec& rhs);
	cVec operator*(const cVec& rhs);
	cVec operator/(const cVec& rhs);

	//Scalar Ops
	cVec operator+(double rhs);
	cVec operator-(double rhs);
	cVec operator*(double rhs);
	cVec operator/(double rhs);

	// Special Ops
	double abs();
	double dotProd(const cVec& rhs);
	cVec crossProd3D(const cVec& rhs);


	

	



	//cVec& operator=(std::initializer_list<double> values);
	//cVec& operator+(std::initializer_list<double> values);

	
};



