//
//  SCSequenceTransitions.cpp
//  SnowCat
//
//  Created by Moky on 13-8-28.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCSequenceTransitions.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

USING_NAMESPACE(FSM_NAMESPACE);

SequenceTransitions::SequenceTransitions(FiniteState * target)
: StateTransition(target)
{
	m_pTransitions = new BaseArray<StateTransition *>();
}

SequenceTransitions::SequenceTransitions(const std::string & target)
: StateTransition(target)
{
	m_pTransitions = new BaseArray<StateTransition *>();
}

SequenceTransitions::~SequenceTransitions(void)
{
	if (m_pTransitions)
	{
		StateTransition * pTrans;
		SC_BASE_ARRAY_FOREACH(m_pTransitions, pTrans, StateTransition *)
		{
			if (pTrans) pTrans->release();
		}
		
		delete m_pTransitions;
		m_pTransitions = NULL;
	}
}

void SequenceTransitions::addTransition(StateTransition * transition)
{
	if (transition)
	{
		m_pTransitions->add(transition);
		transition->retain();
	}
}

IState * SequenceTransitions::evaluate(IState * current, IMachine * fsm)
{
	StateTransition * pTrans;
	SC_BASE_ARRAY_FOREACH(m_pTransitions, pTrans, StateTransition *)
	{
		if (pTrans && pTrans->evaluate(current, fsm) == NULL)
		{
			return NULL;
		}
	}
	return super::evaluate(current, fsm);
}

NAMESPACE_END
