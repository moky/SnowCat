//
//  SCPropertyTransition.h
//  SnowCat
//
//  Created by Moky on 13-8-28.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCPropertyTransition_h
#define SnowCat_SCPropertyTransition_h

#include "SCStateTransition.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class PropertyTransition : StateTransition
{
public:
	// compare condition
	typedef enum {
		EQUAL,
		NOT_EQUAL,
		GREATER,
		LOWER,
		GREATER_OR_EQUAL,
		LOWER_OR_EQUAL,
		FUNCTION_COMPARE,
	} Condition;
	// value type
	typedef enum {
		STRING,
		INTEGER,
		FLOAT,
	} ValueType;
	
public:
	PropertyTransition(FiniteState * target, const std::string & key, const std::string & value, const Condition cond = EQUAL);
	PropertyTransition(const std::string & target, const std::string & key, const std::string & value, const Condition cond = EQUAL);
	virtual ~PropertyTransition(void);
	
	virtual bool init(const std::string & key, const std::string & value, const Condition cond);
	
#pragma mark ITransition
	
	virtual FSM_NAMESPACE::IState * evaluate(FSM_NAMESPACE::IState * current, FSM_NAMESPACE::IMachine * fsm);
	
protected:
	std::string m_sPropertyName;
	std::string m_sPropertyValue;
	Condition m_iCondition;
	ValueType m_iValueType;
};

NAMESPACE_END

#endif
