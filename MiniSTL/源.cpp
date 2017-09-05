#include <iostream>
using namespace std;

int main() {
	int p[] = { 1,25,32,50,16 };
	cout << sizeof(unsigned int) << endl;
	cout << size_t(-1) << endl;
	cout << fixed << pow(2, 64) << endl;
	cout << &p[3] << endl;
	cout << &p[3] - &p[0] << endl;

	new(p) int(2);

	system("Pause");
}