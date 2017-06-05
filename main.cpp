#include <iostream>
#include "StateMachine.hpp"

using namespace std;

class CheckChar{
	char _ch1;
	static char _ch2;
public:	
	CheckChar(char ch): _ch1(ch) {}
	static void SetSecondChar(char ch) {_ch2 = ch;}
	bool operator()() {return _ch1 == _ch2;}
};

char CheckChar::_ch2 = '\0';

int main(int argc, char** argv) {
	string toTest("ababbbbabsabababbba");
	auto ptr = toTest.begin();
	CheckChar::SetSecondChar(*ptr);
	StateMachine myMachine;
	size_t startId = myMachine.IdByName("Start");
	size_t endId = myMachine.IdByName("End");
	auto print = [&myMachine]() {
					cout << myMachine.CurrentState() << endl;
					return true;
				};
	auto res = [&myMachine, &ptr]() {
					++ptr;
					CheckChar::SetSecondChar(*ptr);
					cout << myMachine.CurrentState() << endl;
					return true;
				};

	CheckChar checkA('a'), checkB('b'), check0('\0');
	size_t aId = myMachine.AddState("a", res, checkA);
	size_t bId = myMachine.AddState("b", res, checkB);

	myMachine.ReplaceInitializationById(print, startId);
	myMachine.ReplaceInitializationById(res, endId);
	myMachine.ReplaceConditionById(check0, endId);
	myMachine.ConnectTo(startId, endId);
	myMachine.ConnectTo(startId, aId);
	myMachine.ConnectTo(startId, bId);
	myMachine.ConnectTo(aId, aId);
	myMachine.ConnectTo(aId, bId);
	myMachine.ConnectTo(bId, aId);
	myMachine.ConnectTo(bId, bId);
	myMachine.ConnectTo(aId, endId);
	myMachine.ConnectTo(bId, endId);

	myMachine.Start();
	while(myMachine.IsBusy() && !myMachine.IsCompleted()) {
		myMachine.Update();
	}
	if (myMachine.IsCompleted())
		cout << "Correct State-Machine" << endl;
	else {
		cout << "Incorrect State-Machine" << endl;
		cout << "Error on \'" << (*ptr) << "\'";
		cout << "(" << ptr - toTest.begin() << ") symbol.\n";
	}
	return 0;
}
