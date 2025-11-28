/***************************************************
 Name: Jaysen Hippensteel
 Course: CSMC 362
 Date: 10/21/25
 Assignment: Sieve Of Eratosthenes
 Description: A program to calculate the number of prime numbers in a given
 range using both Set and Vector containers
 File: Sieve.cpp
 
 ***************************************************/

//Includes
#include <iostream>
#include <set>
#include <vector>
#include "Timer.hpp"

//Sieve Method Headers
std::set<unsigned>
sieveSet(unsigned N);
std::set<unsigned>
sieveVector(unsigned N);

/*
*Main method driver to run Set implementation or Vector implementation
*Takes arguments implementation ("set" or "vector") and n (range of prime
*values)
*/
int main(int argc, char* argv[]){
    //Must take 3 arguments (including program name)
    if(argc != 3){
        std::cerr << "Usage: " << argv[0] << " <implementation> <N>" 
        << std::endl;
        exit(EXIT_FAILURE);
    }

    //Creates Timer object
    Timer timer = Timer();

    //Convert arguments to string and unsigned
    std::string implementation (argv[1]);
    std::string arg2 (argv[2]);
    unsigned N = stoul (arg2);
    
    //Runs implementation specified in argument
    std::set<unsigned> primes;
    if (implementation == "set"){
        timer.start();
        primes = sieveSet(N);
        timer.stop();
    }
    else if (implementation == "vector"){
        timer.start();
        primes = sieveVector(N);
        timer.stop();
    }
    else{
        std::cerr << "Unknown argument: " << implementation << " in " << 
        argv[0] << std::endl;
        exit(EXIT_FAILURE);
    }

    //Outputs count of prime numbers as well as implementation used
    std::cout << "Pi[" << N << "] = " << primes.size() << " (using a " <<
    implementation << ")" << std::endl;
    std::cout << "Time: " << timer.getElapsedMs() << " ms" << std::endl;
}

// Return the set of primes between 2 and N.
// Use a set to implement the sieve.
std::set<unsigned>
sieveSet (unsigned N){
    //Fills set primes with unsigned ints from 2 to N
    std::set<unsigned> primes;
    for (unsigned i=2;i<=N;++i){ //Complexity: O(N)
        primes.insert(primes.end(), i); //Complexity: O(1) with hint
    }

    auto it = primes.begin();
    while (it != primes.end()){ //Complexity: O(N)
        unsigned val = *it + *it;
        while (val <= N){ //Complexity: O(log N)
            primes.erase(val); //Complexity: O(log N)
            val += *it;
        }
        ++it;
    }
    return primes;
}

// Return the set of primes between 2 and N.
// Use a vector to implement the sieve.
// After filtering out the composites, put the primes in a set
//   to return to the caller. 
std::set<unsigned>
sieveVector (unsigned N){
    //Fills vector primes wth unsigned ints from 2 to N
    std::vector<bool> primesVector;
    for (unsigned i=2;i<=N;++i){ //Complexity: O(N)
        primesVector.push_back(true); //Complexity O(1)
    }

    //Sets all multiples to false
    for (unsigned i=2;i<=N;++i){ //Complexity: O(N)
        if (!primesVector[i-2]) continue;
        for (unsigned j=i+i;j<=N;j+=i){ //Complexity: O(log N)
            primesVector[j-2] = false; //Complexity: O(1)
        }
    }

    //Fills primes set with vector primes
    std::set<unsigned> primesSet;
    for (unsigned i=0;i<primesVector.size();++i){ //Complexity: O(N)
        if(primesVector[i]){
            primesSet.insert(primesSet.end(), i+2); //Complexity:O(1) with hint
        }
    }
    return primesSet;
}

/*
N       10,000,000    20,000,000   40,000,000
=============================================
set      9517.74       22250.27     50604.80
vector   84.73         163.09       316.04

The vector implementation was much faster than the set implementation. One
reason for this is that vectors allow constant time element access while 
the set implementation uses an erase method that takes O(log n) time.
Additionally,the set implementation must dereference iterators while vector
moves through a range of integers. The complexity for the set implementation
is O(N (log N)^2) and the complexity for the vector implementation is 
O(N log n).
*/