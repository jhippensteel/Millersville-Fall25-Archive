/***************************************************
 Name: Jaysen Hippensteel
 Course: CSMC 362
 Date: 9/10/25
 Assignment: Statistician Redux
 Description: Number analysis program using vectors
 
 ***************************************************/

// TODO: other includes go here

#include <iostream>
#include "Statistician.h"
#include <limits>
// Function Implementations
// **************************************************

// Finds the largest value in the passed vector
// Assumes nums is not empty
float
maximum (const std::vector<float>& nums)
{
	auto it1 = nums.begin();
	auto it2 = nums.end();
	float max = std::numeric_limits<float>::min();
	while (it1 != it2) {
		if (*it1 > max){
			max = *it1;
		}
		it1++;
	}
	return max;
}


// Finds the smallest value in the passed vector
// Assumes nums is not empty
float
minimum (const std::vector<float>& nums)
{
	auto it1 = nums.begin();
	auto it2 = nums.end();
	float min = std::numeric_limits<float>::max();
	while (it1 != it2){
		if(*it1 < min){
			min = *it1;
		}
		it1++;
	}
	return min;
}


// Finds the sum of values from the passed vector
// Should return zero if the vector is empty
float
sumOfValues (const std::vector<float>& nums)
{
	auto it1 = nums.begin();
	auto it2 = nums.end();
	if (it1 == it2){
		return 0;
	}
	float sum = 0;
	while (it1 != it2){
		sum += *it1;
		it1++;
	}
	return sum;
}


// Finds the average of all values from the passed vector
// assumes nums is not empty
float
average (const std::vector<float>& nums)
{
	return sumOfValues(nums) / nums.size();
}

// Creates and returns a new vector. Reads in count number
// of values from the user by prompting for each one
// should return an empty vector if count <= 0
std::vector<float>
populate (int count)
{	
	std::vector<float> result;
	if (count <= 0){
		return result;
	}
	result.resize(count);
	auto it1 = result.begin();
	auto it2 = result.end();
	float value;
	while (it1 != it2){
		std::cout << "Enter value ==> ";
		std::cin >> value;
		*it1 = value;
		it1++;
	}
	return result;
}
