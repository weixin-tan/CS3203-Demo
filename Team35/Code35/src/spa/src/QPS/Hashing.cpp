//
// Created by Prerthan Munireternam on 04/04/2022.
//

#include <random>
#include <vector>
#include <algorithm>

#include "Hashing.h"
#include "Entity.h"

int Hashing::hashFunctionForEntities(Entity entity){
    // declare n to be estimate of how many entities are present
    int n = getTotalEntitiesEstimate();
    //get the upper bound for range of ints
    int size = ceil( (n)*(log10(n*1.00)) );
    //use hardware to generate random seed
    std::random_device rd;
    std::mt19937 gen(rd());
    //create uniform distribution of int from 1 to upper bound
    std::uniform_int_distribution<> distr(1, size);
    //vector to store the numbers
    std::vector<int> storage;

    //generate 5 random numbers from 1 to nlgn
    //store these numbers in the storage vector
    for(int n = 0; n<5; n++){
        int randomInt = distr(gen);
        storage.push_back(randomInt);
    }

    //shuffle storage
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(storage), std::end(storage), rng);

    //get  a and b
    int a = storage[2];
    int b = storage[4];

    //cantorPairingFunction to hash 2 int together
    int ans = cantorPairingFunction(a, b);

    //return the ans
    return ans;

}


int Hashing::hashFunctionForProgramElements(ProgramElement programElement) {
    int n = getTotalProgramElementsEstimate();
    int size = ceil( (n)*(log10(n*1.00)) );
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, size);
    std::vector<int> storage;

    for(int n = 0; n<5; n++){
        int randomInt = distr(gen);
        storage.push_back(randomInt);
    }

    //shuffle storage
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(storage), std::end(storage), rng);

    //get  a and b
    int a = storage[2];
    int b = storage[4];

    //cantorPairingFunction to hash 2 int together
    int ans = cantorPairingFunction(a, b);

    //return the ans
    return ans;

}


int Hashing::getTotalEntitiesEstimate() {
    return totalEntitiesEstimate;
}

void Hashing::setTotalEntitiesEstimate(int input) {
    totalEntitiesEstimate = input;
}

int Hashing::getTotalProgramElementsEstimate() {
    return totalProgramElementsEstimate;
}

void Hashing::setTotalProgramElementsEstimate(int input) {
    totalProgramElementsEstimate = input;
}

int Hashing::cantorPairingFunction(int a, int b) {
    int ans = 0;
    ans = ceil(((a+b)*(a+b+1))/2);
    return ans;
}




