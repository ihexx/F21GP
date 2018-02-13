#include "cVec.h"


cVec::cVec(int len_init) :len(len_init), data(new double[len])
{
}

cVec::cVec(const cVec& c) : cVec(c.len) {
	for ( int i = 0; i < len; i++) {
		data[i] = c.data[i];
	}
}

cVec::cVec(std::initializer_list<double> values):cVec(values.size())
{
	int i = 0;
	for (double object : values) {
		data[i] = object;
		i++;
	}
}

cVec::~cVec()
{
	//Remider: unique_ptr self destructs; no need to del
}

cVec & cVec::operator=(const cVec & c)
{
	// Copy assignment. Check dimensions then copy.
	// If this is not done, then it copies a pointer to vector c's data. Aliasing
	if (this == &c) return *this;
	assert(len == c.len);
	for ( int i = 0; i < len; i++) {
		data[i] = c.data[i];
	}
	return *this;
}

cVec cVec::operator+(const cVec & rhs)
{
	assert(len == rhs.len);
	cVec temp(len);
	for (int i = 0; i < len; i++) {
		temp.data[i] = this->data[i] + rhs.data[i];
	}
	return temp;
}
cVec cVec::operator-(const cVec & rhs)
{
	assert(len == rhs.len);
	cVec temp(len);
	for (int i = 0; i < len; i++) {
		temp.data[i] = this->data[i] - rhs.data[i];
	}
	return temp;
}
cVec cVec::operator*(const cVec & rhs)
{
	assert(len == rhs.len);
	cVec temp(len);
	for (int i = 0; i < len; i++) {
		temp.data[i] = this->data[i] * rhs.data[i];
	}
	return temp;
}
cVec cVec::operator/(const cVec & rhs)
{
	assert(len == rhs.len);
	cVec temp(len);
	for (int i = 0; i < len; i++) {
		temp.data[i] = this->data[i] / rhs.data[i];
	}
	return temp;
}

cVec cVec::operator+(double rhs)
{
	cVec temp(len);
	for (int i = 0; i < len; i++) {
		temp.data[i] = this->data[i] + rhs;
	}
	return temp;
}

cVec cVec::operator-(double rhs)
{
	cVec temp(len);
	for (int i = 0; i < len; i++) {
		temp.data[i] = this->data[i] - rhs;
	}
	return temp;
}

cVec cVec::operator*(double rhs)
{
	cVec temp(len);
	for (int i = 0; i < len; i++) {
		temp.data[i] = this->data[i] * rhs;
	}
	return temp;
}

cVec cVec::operator/(double rhs)
{
	cVec temp(len);
	for (int i = 0; i < len; i++) {
		temp.data[i] = this->data[i] / rhs;
	}
	return temp;
}

double cVec::abs()
{
	double sum = 0;
	for (int i = 0; i < len; i++) {
		sum += data[i] * data[i];
	}
	return sqrt(sum);
}

double cVec::dotProd(const cVec & rhs)
{
	assert(len == rhs.len);
	double result = 0;
	for (int i = 0; i < len; i++) {
		result += this->data[i] * rhs.data[i];
	}
	return result;
}

cVec cVec::crossProd3D(const cVec & rhs)
{
	assert(len == 3);
	cVec temp(len);
	temp.data[0] = (this->data[1] * rhs.data[2]) - (this->data[2] * rhs.data[1]);
	temp.data[1] = (this->data[2] * rhs.data[0]) - (this->data[0] * rhs.data[2]);
	temp.data[2] = (this->data[1] * rhs.data[2]) - (this->data[2] * rhs.data[1]);
	return temp;
}





std::ostream & operator<<(std::ostream & os, const cVec & v)
{
	std::string output;
	for (int i = 0; i < v.len;i++) {
		output += std::to_string(v.data[i]) + ",";
	}
	os << output;
	return os;
}


