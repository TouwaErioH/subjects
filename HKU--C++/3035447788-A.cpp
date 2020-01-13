#include <iostream>
using namespace std;

struct Stack
{
	int tos;
	int stackarray[1000];
};

Stack S;

void push(int a) {
	S.tos++;
	S.stackarray[S.tos] = a;
}

void pop() {
	if (S.tos == -1)
		cout << "false" << " ";
	else
	cout << S.stackarray[S.tos] << " ";
	S.tos--;
}
void main()
{
	S.tos = -1;
	string cmd;
	cin >> cmd;
	if (cmd == "pop") {
		cout << "false";
		return;
	}
	int number;
	cin >> number;
	push(number);
	while(S.tos >= -1) {
		cin >> cmd;
		if (cmd == "pop")
			pop();
		if (cmd == "push") {
			cin >> number;
			push(number);
		}
	}
	return;
}