//
// Created by Prerthan Munireternam on 04/04/2022.
//

#include <random>
#include <vector>

#include "Hashing.h"

int Hashing::hashFunction(Entity entity) {
    int n = getTotalEntitiesEstimate();
    int size = ceil( (n)*(log10(n*1.00)) );
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, size);
    std::vector<int> storage;

    for(int n = 0; n<5; n++){
        int randomInt = distr(gen);
        storage.push_back(randomInt);
    }

    int ans = 0;

    for(int i = 0; i < storage.size(); i++){
        int num = storage[i];
        ans = ans + num;
    }

    return ans;

}

int Hashing::getTotalEntitiesEstimate() {
    return totalEntitiesEstimate;
}

void Hashing::setTotalEntitiesEstimate(int input) {
    totalEntitiesEstimate = input;
}

