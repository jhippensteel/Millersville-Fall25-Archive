////////////////////////////////////////////////////////////////////////////////
// Name: Jaysen Hippensteel
// Date: 9/24/25
// Assignment: Josephus
// Description: A program simulating the josephus problem
//
////////////////////////////////////////////////////////////////////////////////

#include <list>
#include <iostream>
// TODO: add additional includes

#include "Josephus.h"

/* Simulate the Josephus problem modeled as a std::list.
 * This function will modify the passed list with only a
 * single survivor remaining.
 *
 * @param circle -- the linked list of people
 * @param k -- skip amount. NOTE: k > 0
 *
 * @return a list of those who are executed in chronological order
 */
 /*The complexity of the execute function is T(N*k). The derivation of T is
 equal to the variable count, instantiated within the function. N represents
 the size of the list, so for each element in the list, the algorithm must
 traverse a link k time (the size of the jump), until the list size is 1.
  Occasionally, the variable count is greater than N*k (especially when k is
  greater than N). This is because the function must iterate over the "dummy
  head", increasing the count of links traversed, even though the "dummy head"
  is not counted in the size of the list, N.*/ 
template <typename T>
std::list<T>
execute (std::list<T>& circle, int k)
{
  std::list<T> killed;
  auto it1 = circle.begin();
  int count = 0;
  while(circle.size()>1){
    for(int i=0;i<k-1;i++){
      it1++;
      count++;
      if(it1 == circle.end()){
        it1++;
        count++;
      }
    }
    killed.push_back(*it1);
    it1 = circle.erase(it1);
    count++;
    if(it1 == circle.end()){
      it1++;
      count++;
    }
  }
  //std::cout<<count<<std::endl;
  
  return killed;
}

/* entry point to the Josephus problem from the autograder / main
 *
 * @param n -- number of people in the circle
 * @param k -- skip amount. NOTE: k > 0
 */
int
josephus (int n, int k)
{
  std::list<int> nums;
  for(int i=1;i<=n;i++){//For loop populating nums with N elements
    nums.push_back(i);
  }
  std::list<int> killed = execute(nums,k);
  return *nums.begin();
  // 1. make a list
  // 2. populate it with values [1, 2, 3, ... , N]
  // 3. call execute
  // 4. return the lone survivor

  // HINT: While working on this lab, you may also find
  //       it useful to print out the "kill" order.
  
  return 0;
}
