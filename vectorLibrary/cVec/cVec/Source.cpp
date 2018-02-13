#include <iostream>
#include <vector>
#include "cVec.h"

int main() {
	cVec A = { 1,2,3 };
	cVec B = { 3,4,5 };
	cVec c = { 1 };

	//cVec c = A.crossProd3D(B);
	std::cout << c << std::endl;
	/*A = { 6,7,8 };*/
	std::cout << A << std::endl;

	std::cin.ignore();

	return EXIT_SUCCESS;
}