
/*
  Filename   : HeapSort.cpp
  Author     : Jingnan Xie & Jaysen Hippensteel
  Course     : CSCI 362
  Description: Implementation of Heap Sort
*/

#include <iostream>
#include <list>
#include <numeric>
#include <vector>
#include <set>
#include <algorithm>

//'temp' is an arbitrary vector (not sorted)
//temp[0] is meaningless and is NOT used
//heapify the node with index 'i'
//'last' is the last index of the vector 'temp'
void heapify(std::vector<int>& temp, int i, int last){
  //Hint: use recursion
  //Base case: i is a leaf
  if (i*2 > last){
    return;
  }
  if (i*2+1 > last){
    if (temp[i] > temp[i*2]){
      std::swap(temp[i], temp[i*2]);
    }
    return;
  }
  int min = std::min({temp[i], temp[i*2], temp[i*2+1]});
  if(min == temp[i*2]){
    std::swap(temp[i], temp[i*2]);
    heapify(temp, i*2, last);
    return;
  }
  if(min == temp[i*2+1]){
    std::swap(temp[i], temp[i*2+1]);
    heapify(temp, i*2+1, last);
    return;
  }
  return;

}

//heapify all the internal nodes to build a heap
//'temp' is an arbitrary vector, last is its last index
//temp[0] is meaningless and is NOT used
void buildHeap(std::vector<int>& temp, int last){
  //TO DO
  for (int i=last/2;i>0;i--){
    heapify(temp, i, last);
  }

}

//A heapSort helper
//'heap' is a min heap and 'last' is its last index
//heap[0] is meaningless and is NOT used
void heapSort(std::vector<int>& heap, int last){
  //step1: swap the minimum value in 'heap' (root) with the last value of 'heap'
  //step2: heapify the root (just heapify one node) 
  //       'last' should be changed
  //step3: repeat step 1 and 2 until the size of the heap is 1
  while (last > 1){
    std::swap(heap[1], heap[last]);
    last--;
    heapify(heap, 1, last);
  }

}

//main heapSort function
//Heap Sort the vector 'sort' in non-ascending order
//'sort' is an arbitrary vector
//first index of sort is 0
void heapSort(std::vector<int>& sort){
  //TO DO
  //Be Careful: The index of sort starts at 0
  //            The index of temp/heap in above functions starts at 1
  //            You MUST find a way to adjust this issue
  sort.insert(sort.begin(), 0);
  buildHeap(sort, sort.size()-1);
  heapSort(sort, sort.size()-1);
  sort.erase(sort.begin());
  
  
}

int 
main ()  
{
 std::vector<int> myVect ={20, -13, 29, 3, 8 , 17, 13, 10, 26, 0, -11, -5, 7, 19, 8, -11, 36, -5, 49, 13};
 
 heapSort(myVect);
 for(int e: myVect){
   std::cout<<e<<" ";
 }
 return 0;
}
