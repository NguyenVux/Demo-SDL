
#pragma once
#include <unordered_map>
#include <memory>
#include <cstdint>
#include "IState.h"

class FSM
{
public:
	FSM();
	~FSM();
	void AddState(int stateID, std::unique_ptr<IState> &&state);
	void RequestStateChange(int stateID);
	int GetCurrentStateID() const;
	
	void Update();
	void Render();
	void PostFrame();
	bool HasState(int stateID) const
	{
		return m_stateMap.find(stateID) != m_stateMap.end();
	}
	IState *GetCurrrentState()
	{
		if(!HasState(GetCurrentStateID()))
		{
			return nullptr;
		}
		return m_stateMap[GetCurrentStateID()].get();
	}

private:
	void ChangeState(int stateID);
	std::unordered_map<int, std::unique_ptr<IState>> m_stateMap;
	int m_currentStateID;
	int m_nextStateID;
};
