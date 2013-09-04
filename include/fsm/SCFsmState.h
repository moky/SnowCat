//
//  SCFsmState.h
//  SnowCat
//
//  Created by Moky on 13-8-27.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCFsmState_h
#define SnowCat_SCFsmState_h

#include "SCBaseArray.h"
#include "SCFsmProtocol.h"

NAMESPACE_BEGIN(SC_NAMESPACE)
NAMESPACE_BEGIN(FSM_NAMESPACE)

template<class T>
class State : public IState
{
public:
	State(void) {
		m_pTransitions = new BaseArray<T *>();
	}
	virtual ~State(void) {
		if (m_pTransitions) {
			clearTransitions();
			delete m_pTransitions;
			m_pTransitions = NULL;
		}
	}
	
	virtual void addTransition(T * transition) {
		if (m_pTransitions) m_pTransitions->add(transition); // not retain yet
	}
	virtual void clearTransitions(void) {
		if (m_pTransitions) m_pTransitions->clear(); // not release yet
	}
	
#pragma mark IState
	
	virtual void tick(IMachine * fsm) {
		IState * state;
		T * item;
		SC_BASE_ARRAY_FOREACH(m_pTransitions, item, T *) {
			state = item->evaluate(this, fsm);
			if (state && fsm->changeState(state)) return;
		}
	}
	
protected:
	BaseArray<T *> * m_pTransitions;
};

NAMESPACE_END
NAMESPACE_END

#endif
