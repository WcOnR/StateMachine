#include "StateMachine.hpp"

using namespace std;

StateMachine::StateMachine():_startState(new State("Start")),
							_endState(new State("End")),
							_currentState(nullptr) {

	AddState(_startState);
	AddState(_endState);
}

void StateMachine::AddState(StatePtr obj) {
	size_t id = obj->_id;
	while (_allStates.count(id) || !id)
		++id;
	obj->_id = id;
	_allStates[id] = obj;
}

size_t StateMachine::AddState(string name, Predicate init,
										Predicate con, Predicate res) {
	StatePtr newState = (StatePtr) new State(name, init, con, res);
	AddState(newState);
	return newState->_id;
}

void StateMachine::RemoveState(size_t id) {
	_allStates.erase(id);
}

void StateMachine::ConnectTo(size_t from, size_t to) {
	_allStates[from]->_nextStates.insert(_allStates[to]);
}

void StateMachine::Disconnect(size_t from, size_t to) {
	_allStates[from]->_nextStates.erase(_allStates[to]);
}

size_t StateMachine::IdByName(std::string name) {
	auto ptr = _allStates.begin();
	for (; ptr != _allStates.end(); ++ptr) {
		if ((*ptr).second->_name == name)
			return (*ptr).first;
	}
	return 0;
}

map<size_t, string> StateMachine::GetMap() {
	map<size_t, string> resultMap;
	for_each(_allStates.begin(), _allStates.end(),
							[&resultMap](pair<const size_t, StatePtr>& p){
								resultMap[p.first] = p.second->_name;
							});
	return resultMap;
}

void StateMachine::ReplaceInitializationById(Predicate init, size_t id) {
	if (!_allStates.count(id)) return;
	_allStates[id]->_initialization = init ? init: [](){return true;};
}

void StateMachine::ReplaceConditionById(Predicate con, size_t id) {
	if (!_allStates.count(id)) return;
	_allStates[id]->_condition = con ? con: [](){return true;};
}

void StateMachine::ReplaceResponseById(Predicate res, size_t id) {
	if (!_allStates.count(id)) return;
	_allStates[id]->_response = res ? res: [](){return true;};
}

void StateMachine::SetNextState(StatePtr state) {
	_currentState = state;
	_currentState->Start();
	if (!_currentState->IsBusy()) _internalState = PAUSED;
}

void StateMachine::Start() {
	Task::Start();
	SetNextState(_startState);
}

void StateMachine::Update() {
	if (_internalState != BUSY) return;
	_currentState->Update();
	auto ptr = _currentState->OpenState();
	if (ptr)
		SetNextState(ptr);
	else if (_currentState->IsCompleted() && _currentState == _endState)
		_internalState = COMPLETED;
	else if (_currentState->IsCompleted() || !_currentState->IsBusy())
		Task::Stop();
}

void StateMachine::Stop() {
	Task::Stop();
}

string StateMachine::CurrentState() const {
	return (_currentState) ? _currentState->_name : "";
}

bool StateMachine::WaitingNextState() const {
	if (_internalState != BUSY) return false;
	return !(_currentState == _endState || _currentState->IsCompleted());
}

StateMachine::State::State(string name, Predicate init,
							Predicate con, Predicate res):
					_id(hash<string>()(name)), _name(name),
					_initialization(init ? init: [](){return true;}),
					_condition(con ? con: [](){return true;}),
					_response(res ? res: [](){return true;}){}

StateMachine::StatePtr StateMachine::State::OpenState() const{
	for (auto it = _nextStates.begin(); it != _nextStates.end(); ++it) {
		if ((*it)->_condition()) return (*it);
	}
	return nullptr;
}

void StateMachine::State::Start() {
	Task::Start();
	if (!_initialization()) _internalState = PAUSED;
}

void StateMachine::State::Update() {
	if (_internalState != BUSY) return;
	if (_response()) _internalState = COMPLETED;
}

void StateMachine::State::Stop() {
	Task::Stop();
}