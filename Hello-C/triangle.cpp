#include <iostream>
#include <cmath>
#include <algorithm>
#include "triangle.h"

/*
* @file triangle.cpp
* This cpp file initializes triangle objects using the triangle class defined in triangle.h
* @copyright (c) 2025 Jaysen Hippensteel. All rights reserved.
*/

/*=========Main Function========*/
int main(){
	triangle t1(3, 4, 5); //create a triangle with sides 3, 4, 5
	triangle t2;
	triangle* p1;
	triangle* p2;
	p1 = &t1;
	p2 = &t2;
	std::cout<<"Is t1 a right triangle? "<<p1->is_right()<<std::endl;
	std::cout<<"Is t2 a right triangle? "<<p2->is_right()<<std::endl;
	std::cout<<"The perimeter of t1 is "<<p1->perimeter()<<std::endl;
	std::cout<<"The perimeter of t2 is "<<p2->perimeter()<<std::endl;
}
