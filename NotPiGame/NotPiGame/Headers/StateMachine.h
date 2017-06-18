#pragma once

#include <iostream>
#include <vector>

class StateMachine;

//***********************************//
//* The States for the State Machine*//
//***********************************//
class State {
protected:
	friend StateMachine;
	State(std::string stateName, void(*function)(void* instance));
	~State();
    
	// Name of the state if we want to use it.
	std::string stateName;
	// This function pointer gets set in the constructor.
	void(*stateFunction)(void* instance) = nullptr;
};

//***********************************//
//* Stack-Based Finite State Machine*//
//***********************************//

class StateMachine {
public:
	StateMachine();
	~StateMachine();

	    // This executes whatever the function pointer is pointing to
	bool update(void* instance);

	    // This gets the active/last function in the vector of states
	State* getCurrentState();

	    // This function serves the purpose of adding states to the State Machine
	    // This does not control the State Machine
	void createNewState(State * &pointer, std::string NameOfState, void function(void*));

	    // To control what states need to be in the vector in the stack of states
	void pushState(State* state); /* This pushes a State on the top of the stack*/
	void popState();  /* This removes the top function of the stack*/

private:

    // The function pointer of this state machine
	void(*currentFunctionState)(void*) = nullptr;

	    // This vector holds all the pointers - the state machine will hold it and
	    // can delete them when the State Machine destructor gets called
	std::vector<State*> m_states;

	    // Member vector that stores all the functions
	    // that have been added by the State machine.
	    // Basically the stack;
	std::vector<State*> m_functionStates;
};