#ifndef _INTERFACES_HPP
#define _INTERFACES_HPP

class Task {
protected:
	enum InternalState {INIT, BUSY, PAUSED, COMPLETED};
	InternalState _internalState;
public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Stop() = 0;

	bool IsCompleted() const {return _internalState == COMPLETED;}
	bool IsBusy() const {return _internalState == BUSY;}

	virtual ~Task() = default;
};

#endif