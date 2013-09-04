//
//  SCFiniteStateMachine.cpp
//  SnowCat
//
//  Created by Moky on 13-8-28.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCFiniteStateMachine.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

USING_NAMESPACE(FSM_NAMESPACE);

FiniteStateMachine::FiniteStateMachine(void)
: Object()
, Machine()
{
	m_pStates = new Dictionary();
	m_pProperties = new Dictionary();
	m_pDelegate = NULL;
}

FiniteStateMachine::~FiniteStateMachine(void)
{
	if (m_pStates)
	{
		delete m_pStates;
		m_pStates = NULL;
	}
	if (m_pProperties)
	{
		delete m_pProperties;
		m_pProperties = NULL;
	}
	m_pDelegate = NULL;
}

// default state
void FiniteStateMachine::setDefaultStateName(const std::string & name)
{
	setDefaultState(getState(name));
	m_sDefaultStateName = name;
}

IState * FiniteStateMachine::getDefaultState(void)
{
	if (!m_pDefaultState)
	{
		m_pDefaultState = getState(m_sDefaultStateName);
	}
	return Machine::getDefaultState();
}

NAMESPACE_END
