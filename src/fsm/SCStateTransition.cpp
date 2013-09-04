//
//  SCStateTransition.cpp
//  SnowCat
//
//  Created by Moky on 13-8-28.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCFiniteStateMachine.h"
#include "SCFiniteState.h"
#include "SCStateTransition.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

USING_NAMESPACE(FSM_NAMESPACE);

StateTransition::StateTransition(FiniteState * target)
: Object()
, Transition<FiniteState>(target)
, m_sTargetStateName(target->getName())
{
	if (m_pTargetState)
	{
		m_pTargetState->retain();
	}
}

StateTransition::StateTransition(const std::string & target)
: Object()
, Transition<FiniteState>(NULL)
, m_sTargetStateName(target)
{
	
}

StateTransition::~StateTransition(void)
{
	if (m_pTargetState)
	{
		m_pTargetState->release();
	}
}

IState * StateTransition::evaluate(IState * current, IMachine * fsm)
{
	if (!m_pTargetState)
	{
		m_pTargetState = ((FiniteStateMachine *)fsm)->getState(m_sTargetStateName);
	}
	return m_pTargetState;
}

NAMESPACE_END
