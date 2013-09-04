//
//  SCFsmMachine.h
//  SnowCat
//
//  Created by Moky on 13-8-27.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCFsmMachine_h
#define SnowCat_SCFsmMachine_h

#include <string>

#include "SCFsmProtocol.h"

NAMESPACE_BEGIN(SC_NAMESPACE)
NAMESPACE_BEGIN(FSM_NAMESPACE)

class Machine : public IMachine
{
public:
	Machine(void) : m_pDefaultState(NULL), m_pCurrentState(NULL) {}
	virtual ~Machine(void) {}
	
	virtual void setDefaultState(IState * state) {
		m_pDefaultState = state; // this will not retain the state
	}
	virtual IState * getDefaultState(void) {
		return m_pDefaultState;
	}
	
	// start machine: set default state as the current state
	inline void start(void) {
		changeState(getDefaultState());
	}
	
	// stop machine: exit the current state
	inline void stop(void) {
		if (m_pCurrentState) {
			m_pCurrentState->exit(this);
			m_pCurrentState = NULL;
		}
	}
	
#pragma mark IMachine
	
	inline void tick(void) {
		if (m_pCurrentState) m_pCurrentState->tick(this);
	}
	
	inline bool changeState(IState * state) {
		if (m_pCurrentState) m_pCurrentState->exit(this);
		m_pCurrentState = state;
		if (m_pCurrentState) m_pCurrentState->enter(this);
		return m_pCurrentState != NULL;
	}
	
	// pause the machine, and the current state
	inline void pause(void) {
		if (m_pCurrentState) m_pCurrentState->pause(this);
	}
	// resume the machine, and the current state
	inline void resume(void) {
		if (m_pCurrentState) m_pCurrentState->resume(this);
	}
	
protected:
	IState * m_pDefaultState;
	IState * m_pCurrentState;
};

NAMESPACE_END
NAMESPACE_END

#endif
