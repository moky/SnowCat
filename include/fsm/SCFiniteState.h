//
//  SCFiniteState.h
//  SnowCat
//
//  Created by Moky on 13-8-28.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCFiniteState_h
#define SnowCat_SCFiniteState_h

#include <string>
#include "SCObject.h"
#include "SCFsmState.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class StateTransition;

class FiniteState : public FSM_NAMESPACE::State<StateTransition>, public Object
{
	typedef FSM_NAMESPACE::State<StateTransition> super;
	
public:
	FiniteState(const std::string & name);
	virtual ~FiniteState(void);
	
	inline const std::string & getName(void) const {
		return m_sName;
	}
	
	virtual void addTransition(StateTransition * transition);
	virtual void clearTransitions(void);
	
#pragma mark IState
	
	virtual void enter(FSM_NAMESPACE::IMachine * fsm);
	virtual void exit(FSM_NAMESPACE::IMachine * fsm);
	
protected:
	std::string m_sName;
};

NAMESPACE_END

#endif
