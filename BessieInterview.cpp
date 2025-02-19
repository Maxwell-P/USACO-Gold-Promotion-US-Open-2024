#include <iostream>
#include <set>
#include <queue>
using namespace std;

const int MAX_NODES = 500009;

int nodeValue[MAX_NODES], totalNodes, edgesCount, query[MAX_NODES], visited[MAX_NODES], parent[MAX_NODES], head[MAX_NODES], edgesAddedCount;
set<pair<long long, int>> valueSet;
queue<int> bfsQueue;

struct Edge {
    int next, target;
} edges[MAX_NODES * 5];

void addEdge(int from, int to) {
    edges[++edgesAddedCount] = {head[from], to};
    head[from] = edgesAddedCount;
}

void insertIntoSet(pair<long long, int> pairValue) {
    valueSet.insert(pairValue);
    int x = pairValue.second;
    auto it = valueSet.find(pairValue), prev = it, next = ++it;
    if (prev != valueSet.begin() && (--prev)->first == pairValue.first) {
        addEdge(x, prev->second); addEdge(prev->second, x);
    }
    if (next != valueSet.end() && next->first == pairValue.first) {
        addEdge(x, next->second); addEdge(next->second, x);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    cin >> totalNodes >> edgesCount;
    for (int i = 1; i <= totalNodes; i++) {
        cin >> nodeValue[i];
    }

    for (int i = 1; i <= edgesCount; i++) {
        query[i] = i;
        insertIntoSet(make_pair(nodeValue[i], i));
    }

    int newNode = edgesCount;
    for (int i = edgesCount + 1; i <= totalNodes; i++) {
        auto minValuePair = *valueSet.begin();
        newNode++;
        query[newNode] = query[minValuePair.second];
        addEdge(newNode, minValuePair.second);
        valueSet.erase(valueSet.begin());
        minValuePair.first += nodeValue[i];
        minValuePair.second = newNode;
        insertIntoSet(minValuePair);
    }

    int startNode = valueSet.begin()->second;
    cout << valueSet.begin()->first << '\n';
    bfsQueue.push(startNode);
    visited[startNode] = 1;
    parent[query[startNode]] = 1;
    while (!bfsQueue.empty()) {
        int current = bfsQueue.front(); bfsQueue.pop();
        for (int i = head[current]; i; i = edges[i].next) {
            int nextNode = edges[i].target;
            if (visited[nextNode]) continue;
            visited[nextNode] = 1;
            parent[query[nextNode]] = 1;
            bfsQueue.push(nextNode);
        }
    }

    for (int i = 1; i <= edgesCount; i++) cout << parent[i];
    cout << '\n';
    return 0;
}