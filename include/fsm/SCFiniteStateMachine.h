//
//  SCFiniteStateMachine.h
//  SnowCat
//
//  Created by Moky on 13-8-28.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCFiniteStateMachine_h
#define SnowCat_SCFiniteStateMachine_h

#include "SCObject.h"
#include "SCDictionary.h"
#include "SCFsmMachine.h"
#include "SCFiniteState.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class FiniteStateMachine : public FSM_NAMESPACE::Machine, public Object
{
	typedef Machine super;
	
public:
	FiniteStateMachine(void);
	virtual ~FiniteStateMachine(void);
	
	// states pool
	inline void addState(FiniteState * state, const std::string & name) {
		m_pStates->setObject(state, name);
	}
	inline FiniteState * getState(const std::string & name) const {
		return (FiniteState *)m_pStates->objectForKey(name);
	}
	
	// default state
	virtual void setDefaultStateName(const std::string & name);
	virtual FSM_NAMESPACE::IState * getDefaultState(void);
	
	// delegate, will not retain it
	inline void setDelegate(FSM_NAMESPACE::IDelegate * delegate) {
		m_pDelegate = delegate;
	}
	inline FSM_NAMESPACE::IDelegate * getDelegate(void) const {
		return m_pDelegate;
	}
	
	// property
	inline void setProperty(IObject * property, const std::string & name) {
		m_pProperties->setObject(property, name);
	}
	inline IObject * getProperty(const std::string & name) {
		return m_pProperties->objectForKey(name);
	}
	
protected:
	Dictionary * m_pStates;
	std::string m_sDefaultStateName;
	
	FSM_NAMESPACE::IDelegate * m_pDelegate;
	Dictionary * m_pProperties;
};

NAMESPACE_END

#endif
