#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> Coordinates;

ll CalculateManhattanDistance(Coordinates a, Coordinates b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

vector<ll> cumulativeDistance;
ll totalPathDistance = 0;

ll ComputeCircularPathDistance(ll start, ll end) {
    if (start > end) swap(start, end);
    return min(totalPathDistance - (cumulativeDistance[end] - cumulativeDistance[start]), cumulativeDistance[end] - cumulativeDistance[start]);
}

ll pointsCount, patrolsCount;
map<ll, set<Coordinates>> verticalPaths;
map<ll, set<Coordinates>> horizontalPaths;
vector<Coordinates> patrolCoordinates;

set<ll> IdentifyPossiblePatrolIndices(Coordinates query) {
    set<ll> potentialIndices;
    auto verticalIterator = verticalPaths[query.first].lower_bound({query.second, 0});

    if (verticalIterator != verticalPaths[query.first].end()) {
        Coordinates next = *verticalIterator;
        ll index = next.second;

        if (next.first == query.second) {
            potentialIndices.insert(index);
            return potentialIndices;
        }

        if (patrolCoordinates[index + 1].first == query.first && patrolCoordinates[index + 1].second <= query.second) {
            potentialIndices.insert(index);
            potentialIndices.insert((index + 1) % patrolsCount);
        } else if (patrolCoordinates[index + patrolsCount - 1].first == query.first && patrolCoordinates[index + patrolsCount - 1].second <= query.second) {
            potentialIndices.insert(index);
            potentialIndices.insert((index + patrolsCount - 1) % patrolsCount);
        }
    }

    verticalIterator = horizontalPaths[query.second].lower_bound({query.first + 1, 0});
    if (verticalIterator != horizontalPaths[query.second].end()) {
        Coordinates next = *verticalIterator;
        ll index = next.second;

        if (patrolCoordinates[index + 1].second == query.second && patrolCoordinates[index + 1].first <= query.first) {
            potentialIndices.insert(index);
            potentialIndices.insert((index + 1) % patrolsCount);
        } else if (patrolCoordinates[index + patrolsCount - 1].second == query.second && patrolCoordinates[index + patrolsCount - 1].first <= query.first) {
            potentialIndices.insert(index);
            potentialIndices.insert((index + patrolsCount - 1) % patrolsCount);
        }
    }
    return potentialIndices;
}

signed main() {

    cin >> pointsCount >> patrolsCount;
    patrolCoordinates.resize(2 * patrolsCount);
    cumulativeDistance.resize(patrolsCount + 1);

    map<Coordinates, ll> locationToIndex;
    map<ll, vector<Coordinates>> xCoordPatrols;
    map<ll, vector<Coordinates>> yCoordPatrols;

    for (ll i = 0; i < patrolsCount; i++) {
        cin >> patrolCoordinates[i].first >> patrolCoordinates[i].second;
        locationToIndex[patrolCoordinates[i]] = i;
        xCoordPatrols[patrolCoordinates[i].first].push_back({patrolCoordinates[i].second, i});
        yCoordPatrols[patrolCoordinates[i].second].push_back({patrolCoordinates[i].first, i});
    }

    vector<vector<ll>> adjacencyMatrix(patrolsCount);

    for (auto &entry: xCoordPatrols) {
        sort(entry.second.begin(), entry.second.end());
        for (ll i = 0; i < entry.second.size(); i += 2) {
            ll index1 = entry.second[i].second;
            ll index2 = entry.second[i + 1].second;
            adjacencyMatrix[index1].push_back(index2);
            adjacencyMatrix[index2].push_back(index1);
        }
    }

    for (auto &entry: yCoordPatrols) {
        sort(entry.second.begin(), entry.second.end());
        for (ll i = 0; i < entry.second.size(); i += 2) {
            ll index1 = entry.second[i].second;
            ll index2 = entry.second[i + 1].second;
            adjacencyMatrix[index1].push_back(index2);
            adjacencyMatrix[index2].push_back(index1);
        }
    }

    vector<ll> patrolOrder;
    patrolOrder.push_back(0);
    patrolOrder.push_back(adjacencyMatrix[0][0]);

    while (patrolOrder.size() < patrolsCount) {
        ll currentSize = patrolOrder.size();
        ll nextPatrolIndex = adjacencyMatrix[patrolOrder.back()][0] + adjacencyMatrix[patrolOrder.back()][1] - patrolOrder[currentSize - 2];
        patrolOrder.push_back(nextPatrolIndex);
    }

    vector<Coordinates> extendedPatrolPoints(2 * patrolsCount);
    for (ll i = 0; i < patrolsCount; i++) {
        extendedPatrolPoints[i] = patrolCoordinates[patrolOrder[i]];
        extendedPatrolPoints[i + patrolsCount] = extendedPatrolPoints[i];
        verticalPaths[extendedPatrolPoints[i].first].insert({extendedPatrolPoints[i].second, i});
        horizontalPaths[extendedPatrolPoints[i].second].insert({extendedPatrolPoints[i].first, i});
    }

    swap(extendedPatrolPoints, patrolCoordinates);

    for (ll i = 0; i < patrolsCount; i++) {
        cumulativeDistance[i + 1] = cumulativeDistance[i] + CalculateManhattanDistance(patrolCoordinates[i], patrolCoordinates[i + 1]);
    }
    totalPathDistance = cumulativeDistance[patrolsCount];

    vector<ll> startingFrequency(patrolsCount, 0);

    while (pointsCount--) {
        Coordinates startPoint, endPoint;
        cin >> startPoint.first >> startPoint.second >> endPoint.first >> endPoint.second;

        set<ll> startIndices = IdentifyPossiblePatrolIndices(startPoint);
        set<ll> endIndices = IdentifyPossiblePatrolIndices(endPoint);

        if (startIndices == endIndices && startIndices.size() > 1) continue;

        ll shortestDistance = 1e18;
        Coordinates optimalIndices;
        for (ll startIdx : startIndices) {
            for (ll endIdx : endIndices) {
                ll distance = CalculateManhattanDistance(patrolCoordinates[startIdx], startPoint)
                             + CalculateManhattanDistance(patrolCoordinates[endIdx], endPoint)
                             + ComputeCircularPathDistance(startIdx, endIdx);
                if (distance < shortestDistance) {
                    optimalIndices = {startIdx, endIdx};
                    shortestDistance = distance;
                }
            }
        }

        ll startIndex = min(optimalIndices.first, optimalIndices.second) % patrolsCount;
        ll endIndex = max(optimalIndices.first, optimalIndices.second) % patrolsCount;
        if (cumulativeDistance[endIndex] - cumulativeDistance[startIndex] < totalPathDistance - (cumulativeDistance[endIndex] - cumulativeDistance[startIndex])) {
            if (endIndex < patrolsCount - 1) startingFrequency[endIndex + 1]--;
            startingFrequency[startIndex]++;
        } else {
            startingFrequency[0]++;
            if (startIndex < patrolsCount - 1) startingFrequency[startIndex + 1]--;
            startingFrequency[endIndex]++;
        }
    }

    for (ll i = 1; i < patrolsCount; i++) startingFrequency[i] += startingFrequency[i - 1];

    vector<ll> finalFrequencies(patrolsCount);
    for (ll i = 0; i < patrolsCount; i++) {
        finalFrequencies[locationToIndex[patrolCoordinates[i]]] = startingFrequency[i];
    }

    for (ll frequency : finalFrequencies) {
      cout << frequency << endl;
    }
}