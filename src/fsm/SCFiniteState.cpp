//
//  SCFiniteState.cpp
//  SnowCat
//
//  Created by Moky on 13-8-28.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCFiniteStateMachine.h"
#include "SCStateTransition.h"
#include "SCFiniteState.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

USING_NAMESPACE(FSM_NAMESPACE);

FiniteState::FiniteState(const std::string & name)
: Object()
, State<StateTransition>()
, m_sName(name)
{
	
}

FiniteState::~FiniteState(void)
{
	
}

void FiniteState::addTransition(StateTransition * transition)
{
	if (transition)
	{
		transition->retain();
	}
	super::addTransition(transition);
}

void FiniteState::clearTransitions(void)
{
	StateTransition * pTrans;
	SC_BASE_ARRAY_FOREACH(m_pTransitions, pTrans, StateTransition *)
	{
		if (pTrans) pTrans->release();
	}
	super::clearTransitions();
}

void FiniteState::enter(IMachine * fsm)
{
	IDelegate * delegate = ((FiniteStateMachine *)fsm)->getDelegate();
	if (delegate)
	{
		delegate->fsmEnterState(this);
	}
}

void FiniteState::exit(IMachine * fsm)
{
	IDelegate * delegate = ((FiniteStateMachine *)fsm)->getDelegate();
	if (delegate)
	{
		delegate->fsmExitState(this);
	}
}

NAMESPACE_END
