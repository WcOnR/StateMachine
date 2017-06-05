#ifndef _STATE_MACHINE_HPP
#define _STATE_MACHINE_HPP
#include <algorithm>
#include <iostream>
#include <set>
#include <map>
#include <memory>
#include <functional>
#include "Interfaces.hpp"


class StateMachine final: public Task{
	class State;
	typedef std::shared_ptr<State> StatePtr;
	typedef std::function <bool()> Predicate;

	struct State: public Task {
		size_t _id;
		std::string _name;
		std::set <StatePtr> _nextStates;
		Predicate _initialization;
		Predicate _condition;
		Predicate _response;

		State() = delete;
		State(std::string name, Predicate init = nullptr,
							Predicate con = nullptr, Predicate res = nullptr);
		State(State& other) = delete;
		State(State&& other) = delete;
		
		State& operator=(const State& other) = delete;
		State& operator=(State&& other) = delete;
		
		StatePtr OpenState() const;
		
		void Start();
		void Update();
		void Stop();
		
		~State() = default;
	};

	StatePtr _startState;
	StatePtr _endState;
	StatePtr _currentState;
	std::map<size_t, StatePtr> _allStates;

	void AddState(StatePtr obj);
	void SetNextState(StatePtr state);
public:
	StateMachine();
	StateMachine(StateMachine& other) = delete;
	StateMachine(StateMachine&& other) = delete;
	
	size_t AddState(std::string name, Predicate init = nullptr,
							Predicate con = nullptr, Predicate res = nullptr);
	void RemoveState(size_t id);
	void ConnectTo(size_t from, size_t to);
	void Disconnect(size_t from, size_t to);
	size_t IdByName(std::string name);
	std::map<size_t, std::string> GetMap();
	void ReplaceInitializationById(Predicate init, size_t id);
	void ReplaceResponseById(Predicate res, size_t id);
	void ReplaceConditionById(Predicate con, size_t id);

	void Start();
	void Stop();
	void Update();
	std::string CurrentState() const;
	bool WaitingNextState() const;
	
	StateMachine& operator=(const StateMachine& other) = delete;
	StateMachine& operator=(StateMachine&& other) = delete;
	
	~StateMachine() = default;
};
#endif