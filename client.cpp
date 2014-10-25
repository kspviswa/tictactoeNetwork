#include <iostream>
using namespace std;

class BaseClass{
    int data;

    public:
    BaseClass(int d): data(d){};
    ~BaseClass(){};
    void display()
    {
	cout << "DATA = " << data << endl;
    }
};
int main()
{
    BaseClass base(10);
    base.display();
    return 0;
}
