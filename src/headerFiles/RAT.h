//
// Created by Anushri Hatti on 4/22/23.
//

#ifndef UNTITLED_RAT_H
#define UNTITLED_RAT_H

#include <map>
#include <string>
#include <queue>

using namespace std;

class RAT {
public:
    map<string, string> mappings;
    queue<string> freeList;

    RAT();
};


#endif //UNTITLED_RAT_H
