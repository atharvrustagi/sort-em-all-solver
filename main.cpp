#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cstring>
using namespace std;

#define dtype char
#define C 4

unordered_set<basic_string<dtype>> cache;
vector<pair<int, int>> moves;

// how to represent the thing?
// short -> 1 bottle == 4 values
// 12 shorts -> 12 bottles
// store in cache as a string

dtype bottles[12][C] = {
    { 1, 4, 9, 4 },
    { 2, 1, 10, 7 },
    { 3, 7, 6, 1 },
    { 4, 6, 1, 10 },
    { 5, 2, 4, 6 },
    { 6, 8, 3, 9 },
    { 10, 9, 10, 5 },
    { 5, 2, 8, 7 },
    { 3, 8, 3, 7 },
    { 2, 5, 8, 9 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
};

// dtype bottles[12][C] = {
//     { 1, 6, 2, 9 },
//     { 2, 6, 8, 2 },
//     { 3, 7, 1, 3 },
//     { 4, 8, 5, 4 },
//     { 2, 1, 3, 10 },
//     { 5, 7, 9, 5 },
//     { 7, 8, 3, 7 },
//     { 6, 10, 5, 1 },
//     { 10, 9, 8, 4 },
//     { 9, 4, 6, 10 },
//     { 0, 0, 0, 0 },
//     { 0, 0, 0, 0 },
// };

// dtype bottles[12][C] = {
//     { 1, 2, 9, 4 },
//     { 2, 5, 4, 5 },
//     { 3, 7, 5, 2 },
//     { 4, 6, 1, 10 },
//     { 5, 4, 7, 1 },
//     { 6, 8, 2, 9 },
//     { 10, 8, 10, 6 },
//     { 7, 6, 1, 7 },
//     { 3, 10, 9, 8 },
//     { 3, 8, 3, 9 },
//     { 0, 0, 0, 0 },
//     { 0, 0, 0, 0 },
// };


// function to check if we're successful
bool winCheck() {
    for (int b = 0; b < 12; ++b) {
        if (!(bottles[b][0] == bottles[b][1] && 
              bottles[b][1] == bottles[b][2] &&
              bottles[b][2] == bottles[b][3]))
            return false;
    }
    return true;
}

// function to pour from b1 to b2
// returns true is pouring is successful
bool pour(int b1, int b2) {
    // if bottle b2 is already full
    if (bottles[b2][0]) return false;

    // if b1 is already full with same color
    if (bottles[b1][0] == bottles[b1][1] && 
        bottles[b1][1] == bottles[b1][2] &&
        bottles[b1][2] == bottles[b1][3])
        return false;

    // find the indexes
    int i = -1, j = 0;
    while (!bottles[b1][++i]);
    while (++j < C && !bottles[b2][j]);

    // if both colors are not the same
    if (j != C && bottles[b1][i] != bottles[b2][j]) return false;

    // if b1 has only 1 color and b2 is empty, don't pour
    if (!bottles[b2][C-1]) {
        int k = i+1;
        while (k < C && bottles[b1][k-1] == bottles[b1][k]) ++k;
        if (k == C)
            return false;
    }

    // start pouring
    while (i < C && j && (j == C || bottles[b1][i] == bottles[b2][j])) {
        bottles[b2][--j] = bottles[b1][i];
        bottles[b1][i++] = 0;
    }
    return true;
}

bool hashCheck()  {
    // make a mapping
    vector<vector<dtype>> tmp(12, vector<dtype>(4));
    int mapping[12], cnt = 0;
    memset(mapping, 0, 48);
    for (int b = 0; b < 12; ++b)
        for (int i = 0; i < C; ++i)
            if (bottles[b][i] && !mapping[bottles[b][i]])
                mapping[bottles[b][i]] = ++cnt;
        
    for (int b = 0; b < 12; ++b)
        for (int i = 0; i < C; ++i)
            tmp[b][i] = mapping[bottles[b][i]];
        
    sort(tmp.begin(), tmp.end());

    // make string
    basic_string<dtype> s;
    for (int b = 0; b < 12; ++b)
        for (int i = 1; i < C; i += 2)
            s += (dtype)((tmp[b][i] << C) | tmp[b][i-1]);
    if (!cache.count(s)) {
        cache.insert(s);
        return false;
    }
    return true;
}

// dfs backtracking
bool dfs() {
    if (hashCheck())
        return false;
    if (winCheck())
        return true;
    
    // try all possible pourings
    for (int b1 = 0; b1 < 12; ++b1) {
        for (int b2 = 0; b2 < 12; ++b2) {
            if (b1 == b2)
                continue;
            dtype bot1[C], bot2[C];
            memcpy(bot1, bottles[b1], sizeof(bot1));
            memcpy(bot2, bottles[b2], sizeof(bot2));
            
            if (pour(b1, b2) && dfs()) {
                moves.push_back({b1+1, b2+1});
                return true;
            }
            
            // pour back
            memcpy(bottles[b1], bot1, sizeof(bot1));
            memcpy(bottles[b2], bot2, sizeof(bot2));
        }
    }
    return false;
}

int main() {
    cout << "Started\n";
    bool out = dfs();
    if (out) {
        int m = moves.size();
        printf("Solved in %d moves\n", m);
        for (int i = m-1; i > -1; --i)
            printf("%d)\tBottle number %d -> %d\n", m-i, moves[i].first, moves[i].second);
    }
    else {
        printf("Solution not found.");
    }
    return 0;
}
