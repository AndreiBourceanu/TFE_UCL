#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

struct obj1{
    int a;
};

int main(){
    obj1 o1{2};
    obj1 o2 = o1;
    o1.a = 4;
    cout<<o2.a<<endl;
    /*
    cout<<bitset<16>((1 << 10) | (4 << 5) | 7)<<endl;
    vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    v1.clear();
    cout<<v1.size()<<endl;
    bool v = 1;
    cout << (v<<2)<<endl;
    cout<<v<<endl;
    if(!v == 2){
        cout<<"DAA"<<endl;
    }
    */
    return 0;
}