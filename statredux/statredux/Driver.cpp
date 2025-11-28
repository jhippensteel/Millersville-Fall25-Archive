/***************************************************
 Name: Jaysen Hippensteel
 Course: CSMC 362
 Date: 9-10-25
 Assignment: Statistician Redux
 Description: A number analysis program using vectors
 
 ***************************************************/

// Include Directives
// **************************************************
#include <iostream>
// TODO: other includes go here

#include "Statistician.h"

// Using Statements
// **************************************************
using std::cout;
// TODO: any extra using statements would go here

// Forward Declarations
// **************************************************

// TODO: any functions you implement AFTER main must be defined here


// Main
// **************************************************

int
main(int argc, char* argv[])
{
	int count;
	std::cout << "Enter number of values ==> ";
	std::cin >> count;
	auto num_vec = populate(count);
	std::cout << std::endl << "The statistics of all 5 values are:";
	std::cout << std::endl << "  Sum: " << sumOfValues(num_vec) << std::endl << "  Avg: " << average(num_vec);
	std:: cout << std::endl << "  Min: " << minimum(num_vec) << std::endl << "  Max: " << maximum(num_vec) << std::endl;
}


// Function Implementations
// **************************************************

// TODO: any functions you call within main that are a part of this
// file must be implemented AFTER main
