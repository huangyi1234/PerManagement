//////////////////////////////////////////////////////////////////
// hierStateMachine.hpp
//
// Date: 2021/03/19
// Author: yoxia
// Change history:
//      2021/03/19: This is the initialization version.
//
// Discription:
// Implematation common hierarchy state machine
//
//
// Copyright (c) 2021-2031, xxxxxxx.
//////////////////////////////////////////////////////////////////
#ifndef HIER_STATE_MACHINE_HPP
#define HIER_STATE_MACHINE_HPP
#include<iostream>
#include<map>
#include<string>
#include<vector>
#include "typeDef.hpp"
#include "consoleLogger.hpp"
namespace App_dev{

template<class TAGT, typename STATE_T, typename EVENT_T, bool logFlag>
class ChierStateMachine
{
public:
	using Fsm = ChierStateMachine<TAGT, STATE_T, EVENT_T, logFlag>;
	class Ctransition
	{
	public:
		Ctransition(){
			_transition = NULL;
		}
		
		Ctransition(STATE_T toState, void (TAGT::*transition)()){
			_transition = transition;
			_destState = toState;
		}

		STATE_T doTransition(TAGT *obj) {
			if(_transition != NULL) {
				(obj->*_transition)();
			}
      		return(_destState);
		}

		STATE_T destination() const
		{
			return _destState;
		}

	private:
		void (TAGT::*_transition)();
    	STATE_T _destState;
	};

	class CstateBase
	{
	public:
		CstateBase():
			_name(""),
			_enterFunc(NULL),
			_exitFunc(NULL),
			_dispatchEvent(NULL)
		{
		}

		CstateBase(STATE_T state,
			       STATE_T paState,
				   const std::string stateName,
				   void (TAGT::*enterFunc)(),
      		   	   void (TAGT::*exitFunc)(),
				   EVENT_T (TAGT::*dispatchEvent)(EVENT_ENUM)):
			_myState(state),
			_paState(paState),
			_name(stateName),
			_enterFunc(enterFunc),
			_exitFunc(exitFunc),
			_dispatchEvent(dispatchEvent)
		{
		}

		void enter(TAGT *obj)
		{
			if(_enterFunc != NULL) {
				(obj->*_enterFunc)();
			}
		}

		bool exit(TAGT *obj)
		{
			if(_exitFunc != NULL) {
				(obj->*_exitFunc)();
			}
			return true;
		}

		EVENT_T dispatchEvent(TAGT *obj, EVENT_ENUM event)
		{
			if(_dispatchEvent == NULL) {
				return event;
			}
			return((obj->*_dispatchEvent)(event));
		}

		void addTransition(EVENT_T event, 
						   STATE_T toState, 
						   void(TAGT::*transition)())
		{
			_eventMap[event] = Ctransition(toState, transition);
		}

		STATE_T transition(TAGT *obj, EVENT_T event, Fsm* pFsm)
		{
			typename EventMap::iterator iter = _eventMap.find(event);
			if(iter != _eventMap.end()){
				return(iter->second.doTransition(obj));
			}
			pFsm->noTransitionBehaviour(_myState, event);

			return(_myState); //keep current state 
		}

		STATE_T destination(TAGT *, EVENT_T event, Fsm const*) const
		{
			typename EventMap::const_iterator iter = _eventMap.find(event);
			if(iter != _eventMap.end()){
				return(iter->second.destination());
			}
			return(_myState); //keep current state
		}

		void getEventList(STATE_T toState,
					   	  std::vector<EVENT_T> &eventList) const
		{
			for(typename EventMap::const_iterator iter = _eventMap.begin();
				iter != _eventMap.end();
				iter++){
				if(iter->second.destination() == toState){
					eventList.push_back(iter->first);
				}
			}
		}

		App_dev::USHORT countNotSupportedTransitions(TAGT *obj, 
				std::vector<EVENT_T> &eventList,
				Fsm const* pFsm) const
		{
			typename std::vector<EVENT_T>::const_iterator iter;
			App_dev::USHORT notSupportedTransitions = 0;

			for(iter = eventList.begin(); iter != eventList.end(); iter++){
				if(_eventMap.find(*iter) == _eventMap.end()){
					notSupportedTransitions++;
        		}
			}
			return(notSupportedTransitions);
		}

		std::string getStateName() const
		{
			return(_name);
		}

		STATE_T getPaState() const
		{
			return(_paState);
		}

	private:
		STATE_T _myState;
		STATE_T _paState;//parent state
		std::string _name;
		void (TAGT::*_enterFunc)();		
		void (TAGT::*_exitFunc)();
		EVENT_T (TAGT::*_dispatchEvent)(EVENT_ENUM);
		std::map< EVENT_T, Ctransition > _eventMap;
	};

	ChierStateMachine(TAGT * obj, STATE_T startState):
		_obj(obj),
		_curState(startState),
		_pLogger(0),
		_startState(startState)
	{
	}

	ChierStateMachine(TAGT * obj, STATE_T startState, 
				 App_dev::CconsoleLogger::Ptr pLogger):
		_obj(obj),
		_curState(startState),
		_pLogger(pLogger),
		_startState(startState)
	{
	}

	virtual ~ChierStateMachine()
	{
		return;
	}

	void restart()
	{
		_stateMap[_curState].exit(_obj);
		STATE_T paState = _stateMap[_curState].getPaState();
		//exit parent state
		if(_stateMap.find(paState) == _stateMap.end()){
			logger_error_f(_pLogger, "Can't find (%s)'s parent state", _stateMap[_curState].getName()); 
		}else{
			_stateMap[paState].exit(_obj);
		}
		_curState = _startState;
		start();
	}
	
	void addState(STATE_T state, 
				  STATE_T paState, std::string name,
			      void(TAGT::*enter)(), void(TAGT::*exit)(),
			      EVENT_T(T::*dispatchEvent)(EVENT_ENUM) = NULL)
	{
		_stateMap[state] = CstateBase(state, paState, name, enter, 
									  exit, dispatchEvent);
	}

	void addEvent(EVENT_T event, const std::string name)
	{
		_eventMap[event] = name;
	}

	void addTransition(STATE_T fromState, EVENT_T event,
					   STATE_T toState, void(TAGT::*transition)())
	{
		if(_stateMap.find(fromState) == _stateMap.end()){
			return;
		}
		_stateMap[fromState].addTransition(event, toState, 
										   transition);
	}

	void start()
	{
		if(_stateMap.find(_curState) == _stateMap.end()){
			return;
		}
		_stateMap[_curState].enter(_obj);
	}

	void processEvent(EVENT_T event)
	{
		STATE_T dest;
		EVENT_T newEvent = event;
		
		/*************************************************************/
		/*First step: check sublayer if it can support this event    */
		/*Second step: After first step when dest == current state   */
		/*Run Third step. when dest != current state run Fourth step */         
		/*Third step: check parent layer if it can support this event*/
		/*Fouth step: process it in sublayer                         */ 
		/*************************************************************/
		newEvent = _stateMap[_curState].dispatchEvent(_obj, newEvent);
		if((dest = _stateMap[_curState].destination(_obj, newEvent, this)) == _curState){
			_stateMap[_curState].transition(_obj, newEvent, this);
			STATE_T paState = _stateMap[_curState].getPaState();
			//exit parent state
			if(_stateMap.find(paState) == _stateMap.end()){
				logger_error_f(_pLogger, "Can't find (%s)'s parent state", _stateMap[_curState].getName()); 
			}else{
				if((dest = _stateMap[paState].destination(_obj, newEvent, this))
					!= paState){
					_stateMap[_curState].exit(_obj);
					_stateMap[paState].exit(_obj);
					_curState = _stateMap[paState].transition(_obj, newEvent,
												    this);
					_stateMap[_curState].enter(_obj);
				}
			}
			return;
		}
		_stateMap[_curState].exit(_obj);
		_curState = _stateMap[_curState].transition(_obj, newEvent,
												    this);
		_stateMap[_curState].enter(_obj);
	}

	virtual void noTransition(STATE_T, EVENT_T)
	{
	}

	STATE_T currentState() const {return(_curState);}
	
	void getEventList(STATE_T fromState, STATE_ENUM toState,
				      std::vector<EVENT_T> &eventList) const
	{
		typename StateMap::const_iterator iter = _stateMap.find(fromState);
		if (iter != _stateMap.end()){
			iter->second.getEventList(toState, eventList);
		}
	}

	void getStateList(std::vector<STATE_T> &stateList) const
	{
		for(typename StateMap::const_iterator iter = _stateMap.begin();
        	iter != _stateMap.end();
			iter++) {
			stateList.push_back(iter->first);
		}
	}

	std::string getStateName(STATE_T state) const
	{
		typename StateMap::const_iterator iter = _stateMap.find(state);
		if(iter != _stateMap.end()){
			return(iter->second.getStateName());
		}
		return("Can't find");
	}

	std::string getCurrentStateName() const
	{
		return(getStateName(_curState));
	}

	std::string getEventName(EVENT_T event) const
	{
		typename EventMap::const_iterator iter = _eventMap.find(event);
		if(iter != _eventMap.end()){
			return iter->second;
		}
		return("Can't find");
	}

	App_dev::USHORT countNotSupportedTransitions() const
	{
		typename std::vector<EVENT_T> events;
		typename std::map<EVENT_T, std::string>::const_iterator eventIter;
		typename std::map<STATE_T, CstateBase>::const_iterator fromIter;
		App_dev::USHORT  notSupportedCount = 0;

		for(eventIter = _eventMap.begin(); 
			eventIter != _eventMap.end(); 
			eventIter++){
			events.push_back(eventIter->first);
		}

		for(fromIter = _stateMap.begin(); 
			fromIter != _stateMap.end();
			fromIter++){
			notSupportedCount += fromIter->second.countNotSupportedTransitions(_obj, events, this);
		}
		return(notSupportedCount);
	}

private:
	TAGT *_obj;
	using StateMap = std::map<STATE_T, CstateBase>;
	StateMap _stateMap;
	using EventMap = std::map<EVENT_T, std::string>;
	EventMap _eventMap;
	STATE_T _curState;
	App_dev::CconsoleLogger::Ptr _pLogger;
	const STATE_T _startState;
};

}

#endif

