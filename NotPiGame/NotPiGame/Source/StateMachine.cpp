#include "../Headers/StateMachine.h"

#pragma region State Class
State::State(std::string stateName, void(*function)(void* instance))
	: stateName(stateName)
	, stateFunction(function) {
    // Empty
}

State::~State() {
	stateFunction = nullptr;
}
#pragma endregion


#pragma region StateMachine
StateMachine::StateMachine() {
    // Empty
}

StateMachine::~StateMachine() {
    // Just set the function state to nullptr;
	currentFunctionState = nullptr;

	    // Delete all the states that this State Machines created
	for (unsigned int i = 0; i < m_states.size(); i++) {
		delete m_states[i];
		m_states[i] = nullptr;
	}
}

bool StateMachine::update(void* instance) {
    // Get the current function state and assign it to the function state
	currentFunctionState = this->getCurrentState()->stateFunction;

	    // Now we exectute whetever the function pointer is pointing to
	if (currentFunctionState != nullptr) { currentFunctionState(instance); return true; }
	return false;
}

State* StateMachine::getCurrentState() {
	return m_functionStates.back();
}

void StateMachine::createNewState(State * &pointer, std::string NameOfState, void function(void*)) {
	pointer = new State(NameOfState, function);
	m_states.push_back(pointer);
}

void StateMachine::pushState(State* state) {
	m_functionStates.push_back(state);
}

void StateMachine::popState() {
	m_functionStates.pop_back();
}
#pragma endregion The State Machine