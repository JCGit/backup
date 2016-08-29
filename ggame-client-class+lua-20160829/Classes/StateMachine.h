#ifndef STATEMACHINE_H
#define STATEMACHINE_H

//------------------------------------------------------------------------
//
//  Name:   StateMachine.h
//
//  Desc:   State machine class. Inherit from this class and create some 
//          states to give your agents FSM functionality
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <cassert>
#include <string>
#include <list>
 #include <typeinfo>

template <class entity_type>
class State
{
public:

	virtual ~State(){}

	//this will execute when the state is entered
	virtual void Enter(entity_type*)=0;

	//this is the states normal update function
	virtual void Execute(entity_type*)=0;

	//this will execute when the state is exited. (My word, isn't
	//life full of surprises... ;o))
	virtual void Exit(entity_type*)=0;

	//release state resources
	virtual void Recycle() { };
};


template <class entity_type>
class StateMachine
{
public:
	typedef std::list<State<entity_type>*> STATE_POINTER_LIST;
private:

  //a pointer to the agent that owns this instance
  entity_type*          m_pOwner;

  State<entity_type>*   m_pCurrentState;
  
  //a record of the last state the agent was in
  STATE_POINTER_LIST   m_pPreviousStateList;

  //this is called every time the FSM is updated
  STATE_POINTER_LIST   m_pGlobalStateList;
  

public:

  StateMachine(entity_type* owner):m_pOwner(owner),
                                   m_pCurrentState(NULL),
                                   m_pPreviousStateList(),
                                   m_pGlobalStateList()
  {}

  virtual ~StateMachine(){
	  //if a global state exists, call its execute method, else do nothing
	  typename STATE_POINTER_LIST::const_iterator it = m_pGlobalStateList.begin();
	  for(;it!=m_pGlobalStateList.end();++it)
	  {
		  (*it)->Exit(m_pOwner);
		  (*it)->Recycle();
	  }

	  //same for the current state
	  if (m_pCurrentState) 
	  {
		  m_pCurrentState->Exit(m_pOwner);
		  m_pCurrentState->Recycle();
	  }
  }

  void AddGlobalState(State<entity_type>* s) 
  { 
	  if(s)
	  {
		  m_pGlobalStateList.push_back(s);
		  s->Enter(m_pOwner);
	  }
  }

  void removeGlobalState(State<entity_type>* s) 
  {
	  if(s)
	  {
		  m_pGlobalStateList.remove(s);
		  s->Exit(m_pOwner);
		  s->Recycle();
	  }
  }
  
  //call this to update the FSM
  void  Update()const
  {
    //if a global state exists, call its execute method, else do nothing
    typename STATE_POINTER_LIST::const_iterator it = m_pGlobalStateList.begin();
	for(;it!=m_pGlobalStateList.end();++it)
	{
		(*it)->Execute(m_pOwner);
	}

    //same for the current state
    if (m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
  }

  //change to a new state
  void  ChangeState(State<entity_type>* pNewState)
  {
   // assert(pNewState && "<StateMachine::ChangeState>: trying to change to NULL state");
	
	if(m_pCurrentState)
	{
		//can not chage to same state
		if(pNewState == m_pCurrentState)
			return;

		//keep a record of the previous state
		if(m_pPreviousStateList.size()>100)//we presever 100 states to roll back
			m_pPreviousStateList.pop_front();

		m_pPreviousStateList.push_back(m_pCurrentState);

		//call the exit method of the existing state
		m_pCurrentState->Recycle();
		m_pCurrentState->Exit(m_pOwner);
	}

    //change state to the new state
    m_pCurrentState = pNewState;

    //call the entry method of the new state
    if ( m_pCurrentState )
		m_pCurrentState->Enter(m_pOwner);
  }

  //change state back to the previous state
  void  RevertToPreviousState()
  {
	  if(m_pCurrentState == 0)
		  return;

	  //call the exit method of the existing state
	  m_pCurrentState->Exit(m_pOwner);
	  m_pCurrentState->Recycle();

	  //change state to the previous state
	  m_pCurrentState = m_pPreviousStateList.back();
	  m_pPreviousStateList.pop_back();

	  //call the entry method of the previous state
	  m_pCurrentState->Enter(m_pOwner);
  }

  //returns true if the current state's type is equal to the type of the
  //class passed as a parameter. 
  bool  isInState(const State<entity_type>& st)const
  {
    return typeid(*m_pCurrentState) == typeid(st);
  }

  State<entity_type>*  CurrentState()  const{return m_pCurrentState;}
  const STATE_POINTER_LIST &  GlobalState()   const{return m_pGlobalStateList;}
  const STATE_POINTER_LIST &  PreviousState() const{return m_pPreviousStateList;}
};




#endif


