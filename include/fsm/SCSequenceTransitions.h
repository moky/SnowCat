//
//  SCSequenceTransitions.h
//  SnowCat
//
//  Created by Moky on 13-8-28.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCSequenceTransitions_h
#define SnowCat_SCSequenceTransitions_h

#include "SCBaseArray.h"
#include "SCStateTransition.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

/**
 *
 *  NOTICE:
 *
 *      When all of the transitions passed through, the evaluate() will return the target state.
 *
 */

class SequenceTransitions : StateTransition
{
public:
	SequenceTransitions(FiniteState * target);
	SequenceTransitions(const std::string & target);
	virtual ~SequenceTransitions(void);
	
	void addTransition(StateTransition * transition);
	
#pragma mark ITransition
	
	virtual FSM_NAMESPACE::IState * evaluate(FSM_NAMESPACE::IState * current, FSM_NAMESPACE::IMachine * fsm);
	
protected:
	BaseArray<StateTransition *> * m_pTransitions;
};

NAMESPACE_END

#endif
