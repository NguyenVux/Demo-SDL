#include "pch.h"
#include "FSM.h"

FSM::FSM()
	: m_currentStateID(-1), m_nextStateID(-1)
{}

FSM::~FSM() {
	m_stateMap.clear();
}

void FSM::AddState(int stateID, std::unique_ptr<IState>&& state) {
	if (state == nullptr) {
		return;
	}
	m_stateMap[stateID] = std::move(state);
}

void FSM::RequestStateChange(int stateID) {
	if (m_stateMap.find(stateID) == m_stateMap.end()) {
		ASSERT("Cannot find state")
		return;
	}
	m_nextStateID = stateID;
}

void FSM::Update() {
	if (m_nextStateID != -1) {
		ChangeState(m_nextStateID);
		m_nextStateID = -1;
	}
	if (m_currentStateID != -1 && m_stateMap.find(m_currentStateID) != m_stateMap.end()) {
		m_stateMap[m_currentStateID]->Update();
	}
}

int FSM::GetCurrentStateID() const {
	   return m_currentStateID;
}

void FSM::PreFrame() {
	   if (m_currentStateID != -1 && m_stateMap.find(m_currentStateID) != m_stateMap.end()) {
			   m_stateMap[m_currentStateID]->PreFrame();
	   }
}

void FSM::Render() {
	   if (m_currentStateID != -1 && m_stateMap.find(m_currentStateID) != m_stateMap.end()) {
			   m_stateMap[m_currentStateID]->Render();
	   }
}

void FSM::PostFrame() {
	   if (m_currentStateID != -1 && m_stateMap.find(m_currentStateID) != m_stateMap.end()) {
			   m_stateMap[m_currentStateID]->PostFrame();
	   }
}

void FSM::ChangeState(int stateID) {
	   if (m_stateMap.find(stateID) == m_stateMap.end()) {
			   return;
	   }
	   if (m_stateMap.find(m_currentStateID) != m_stateMap.end()) {
			   m_stateMap[m_currentStateID]->Exit();
	   }
	   m_currentStateID = stateID;
	   if (m_stateMap.find(m_currentStateID) != m_stateMap.end()) {
			   m_stateMap[m_currentStateID]->Enter();
	   }
}
