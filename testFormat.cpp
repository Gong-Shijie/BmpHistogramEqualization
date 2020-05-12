#include <iostream>
#include <stdio.h>
using namespace std;
int main()
{
    int temp = 0;
    temp = 31/32;
      cout <<"heloo"<< endl;
    cout <<temp << endl;
    for (size_t i = 0; i < 100; i++)
    {
        temp = (i + 31) / 32;
        cout << temp  << endl;
    }
    return 0;
}