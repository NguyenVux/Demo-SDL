#pragma once
#include "FSM.h"

class FSM;

class IState {
public:
	IState() 
		: m_fsm(nullptr)
	{
	}
	virtual void Enter() {}
	virtual void Exit() {}
	virtual void PreFrame() {}
	virtual void Update() {}
	virtual void Render() {}
	virtual void PostFrame() {}
	virtual ~IState() {}
	void SetFSM(FSM* fsm) {
		m_fsm = fsm;
	}
	bool CanTransitionToSelf() const { return m_canTransitionToSelf; }
private:
	FSM* m_fsm;
	bool m_canTransitionToSelf = false;
};
