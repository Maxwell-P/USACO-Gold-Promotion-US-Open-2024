#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

typedef long long ll;
ll numElements, sequenceLength;
vector<vector<ll>> indexMatrix; 
vector<vector<ll>> sequenceGroups;
string sequence;
ll zero = 0;
ll one = 1;

bool CompareElements(ll x, ll y){
  if(sequence[x] != sequence[y]){
    return sequence[x] < sequence[y];
  }
  if(sequenceLength - one == zero){
    return x < y;
  }
  return indexMatrix[x + one][sequenceLength - one] < indexMatrix[y + one][sequenceLength - one];
}

int main(){
  cin >> numElements;
  vector<ll> count(numElements + one);
  indexMatrix = vector<vector<ll>>(numElements, vector<ll>(numElements + one));
  sequenceGroups = vector<vector<ll>>(numElements + one);

  cin >> sequence;
  for(ll i = zero; i < numElements; i++) {
    indexMatrix[i][zero] = i; 
    sequenceGroups[zero].push_back(i);
  }

  for(sequenceLength = one; sequenceLength <= numElements; sequenceLength++){

    for(ll i = zero; i <= numElements - sequenceLength; i++){
        sequenceGroups[sequenceLength].push_back(i);
    }

    vector<ll> temp(numElements - sequenceLength + 1);
    sort(sequenceGroups[sequenceLength].begin(), sequenceGroups[sequenceLength].end(), CompareElements);

    for(ll i = zero; i <= numElements - sequenceLength; i++){
      indexMatrix[sequenceGroups[sequenceLength][i]][sequenceLength] = i;
      temp[sequenceGroups[sequenceLength][i]] = i;
    }

    stack<pair<ll, ll>> stackDecreasing; 
    stack<pair<ll, ll>> stackIncreasing;

    stackDecreasing.push({numElements - sequenceLength + one, -one});
    stackIncreasing.push({-one, -one});

    vector<ll> leftBounds(numElements - sequenceLength + 1); 
    vector<ll> rightBounds(numElements - sequenceLength + 1);

    for(ll i = zero; i <= numElements - sequenceLength; i++){
      while(!stackDecreasing.empty() && stackDecreasing.top().second >= temp[numElements - sequenceLength - i]){
        stackDecreasing.pop();
      }
      while(!stackIncreasing.empty() && stackIncreasing.top().second >= temp[i]){
        stackIncreasing.pop();
      }
      rightBounds[numElements - sequenceLength - i] = stackDecreasing.top().first - one;
      leftBounds[i] = stackIncreasing.top().first + one; 
      stackIncreasing.push({i, temp[i]}); 
      stackDecreasing.push({numElements - sequenceLength - i, temp[numElements - sequenceLength - i]});
    }
    vector<ll> output; 
    output.push_back(0); 
    for(ll i = zero; i <= numElements - sequenceLength; i++){
      output.push_back(rightBounds[i] - leftBounds[i] + 1);
    }
    sort(output.begin(), output.end());
    for(ll i = one; i <= numElements - sequenceLength + 1; i++){
      count[numElements - sequenceLength - i + one] += output[i] - output[i - one];
    }
  } 
  for(ll i = zero; i < numElements; i++){
    cout << count[i] << endl;
  }
}