#ifndef STATISTICIAN_H_
#define STATISTICIAN_H_

#include <vector>

// Finds the largest value in the passed vector
// Assumes nums is not empty
float
maximum (const std::vector<float>& nums);


// Finds the smallest value in the passed vector
// Assumes nums is not empty
float
minimum (const std::vector<float>& nums);


// Finds the sum of values from the passed vector
// Should return zero if the vector is empty
float
sumOfValues (const std::vector<float>& nums);


// Finds the average of all values from the passed vector
// assumes nums is not empty
float
average (const std::vector<float>& nums);


// Creates and returns a new vector. Reads in count number
// of values from the user by prompting for each one
// should return an empty vector if count <= 0
std::vector<float>
populate (int count);

#endif
