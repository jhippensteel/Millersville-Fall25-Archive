#ifndef triangle_H
#define triangle_H

/*
* @file triangle.h
* triangle class
* implementation of triangle with 3 member attributes for the length of each side
* containts functions to:
*	get the length of each side of the triangle
*	get the perimeter of the triangle
* 	get the largest side of the triangle
*	get the boolean value whether or not the triangle is a right triangle
*	set the length of each side of the triangle
* @copyright (c) 2025 Jaysen Hippensteel. All rights reserved.
*/
class triangle{
/*==========Data fields===========*/
private:
	float m_side1;//length of side 1
	float m_side2;//length of side 2
	float m_side3;//length of side 3

public:
	/*=========Constructor List=========*/
	triangle(){
		m_side1 = 1;
		m_side2 = 1;
		m_side3 = 1;
	}
	triangle(const float& x){
		m_side1 = x;
		m_side2 = x;
		m_side3 = x;
	}
	triangle(const float& x, const float& y, const float& z){
		m_side1 = x;
		m_side2 = y;
		m_side3 = z;
	}
	
	/*==========Acessor Functions===============*/
	//Return the float variable m_side1 as a float
	float get_side1() const{
		return m_side1;
	}
	//Return the float variable m_side2 as a float
	float get_side2() const{
		return m_side2;
	}
	//Return the float variable m_side3 as a float
	float get_side3() const{
		return m_side3;
	}
	//Return the perimeter of the triangle by performing addition on the variables m_side1, m_side2, and m_side3
	float perimeter() const{
		return m_side1 + m_side2 + m_side3;
	}
	//Return the largest side of the triangle by calculating the max variable m_side1, m_side2, m_side3
	float largest_side() const{
		return std::max(std::max(m_side1, m_side2), m_side3);
	}
	//Return whether or not the triangle is a right triangle using the Pythagorean theorem
	bool is_right() const{
		float side_arr[] = {m_side1, m_side2, m_side3};
		std::sort(side_arr, side_arr+3);
		return (std::pow(side_arr[0], 2) + std::pow(side_arr[1], 2)) == std::pow(side_arr[2], 2);
	}

	/*==============Update Functions==============*/
	//Set the float variable m_side1
	void set_side1(const float& x) {
		m_side1 = x;
	}
	//Set the float variable m_side2
	void set_side2(const float& x) {
		m_side2 = x;
	}
	//Set the float variable m_side3
	void set_side3(const float& x) {
		m_side3 = x;
	}
};
#endif
