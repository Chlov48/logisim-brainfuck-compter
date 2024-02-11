#include <iostream>
using namespace std;

string s = "++++++++[>++++++++<-]>+.+.+.+.+.";

char cur;

int main()
{
    int x = 0;
    for(int i = 0; i < (int)s.size(); i++){
        if(x%20 == 0){cout << endl<< "00" << x << ": ";}
        cur = s.at(i);
        if(cur == '+'){cout <<"0";}
        if(cur == '-'){cout <<"1";}
        if(cur == '>'){cout <<"2";}
        if(cur == '<'){cout <<"3";}
        if(cur == '.'){cout <<"4";}
        if(cur == ','){cout <<"5";}
        if(cur == '['){cout <<"6";} 
        if(cur == ']'){cout <<"7";}
        cout << " ";
        x++;
       
    }
    return 0;
}
