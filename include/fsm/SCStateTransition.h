//
//  SCStateTransition.h
//  SnowCat
//
//  Created by Moky on 13-8-28.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCStateTransition_h
#define SnowCat_SCStateTransition_h

#include <string>
#include "SCObject.h"
#include "SCFsmTransition.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class FiniteState;

class StateTransition : public FSM_NAMESPACE::Transition<FiniteState>, public Object
{
public:
	StateTransition(FiniteState * target);
	StateTransition(const std::string & target);
	virtual ~StateTransition(void);
	
#pragma mark ITransition
	
	virtual FSM_NAMESPACE::IState * evaluate(FSM_NAMESPACE::IState * current, FSM_NAMESPACE::IMachine * fsm);
	
protected:
	std::string m_sTargetStateName;
};

NAMESPACE_END

#endif
