#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;


int main(){
    int total = 0;
    int count = 0;
    int n;
    cin >> n;

    vector<int*> arrp;
    int input[n];
 
    for (int i = 0; i < n; i++) {
        cin >> input[i];
        arrp.push_back(&input[i]);
    }

    for (int i = 0; i < n; i++){
        cout << (arrp[i]) << " ";
    }
    cout << "\n" << " ";

    for (int i = 0; i < n; i++){
        if (*(arrp[i]) != int(0)){
            total += *(arrp[i]);
            count += 1;}
        cout << *(arrp[i]) << " ";}
    int median = total/count;
    replace(arrp.begin(), arrp.end(), string{"null"} , mids);
    }
    
    
    

}