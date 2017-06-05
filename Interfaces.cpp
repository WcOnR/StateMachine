#include "Interfaces.hpp"

void Task::Start() {
	_internalState = BUSY;
}

void Task::Stop() {
	_internalState = (_internalState == BUSY) ? PAUSED : _internalState;
}