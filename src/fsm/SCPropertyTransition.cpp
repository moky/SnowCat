//
//  SCPropertyTransition.cpp
//  SnowCat
//
//  Created by Moky on 13-8-28.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCString.h"
#include "SCFiniteStateMachine.h"
#include "SCPropertyTransition.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

USING_NAMESPACE(FSM_NAMESPACE);

PropertyTransition::PropertyTransition(FiniteState * target, const std::string & key, const std::string & value, const Condition cond)
: StateTransition(target)
{
	init(key, value, cond);
}

PropertyTransition::PropertyTransition(const std::string & target, const std::string & key, const std::string & value, const Condition cond)
: StateTransition(target)
{
	init(key, value, cond);
}

PropertyTransition::~PropertyTransition(void)
{
	
}

bool PropertyTransition::init(const std::string &key, const std::string &value, const Condition cond)
{
	m_sPropertyName = key;
	m_sPropertyValue = value;
	m_iCondition = cond;
	m_iValueType = STRING;
	
	String * pString = new String(value);
	if (pString->empty())
	{
		//
	}
	else if (pString->isInteger())
	{
		m_iValueType = INTEGER;
	}
	else if (pString->isFloat())
	{
		m_iValueType = FLOAT;
	}
	pString->release();
	
	return true;
}

bool PropertyTransition_compareIntegers(PropertyTransition::Condition cond, const int value, const int defaultValue)
{
	switch (cond)
	{
		case PropertyTransition::EQUAL:
			if (value == defaultValue) return true;
			break;
			
		case PropertyTransition::NOT_EQUAL:
			if (value != defaultValue) return true;
			break;
			
		case PropertyTransition::GREATER:
			if (value > defaultValue) return true;
			break;
			
		case PropertyTransition::LOWER:
			if (value < defaultValue) return true;
			break;
			
		case PropertyTransition::GREATER_OR_EQUAL:
			if (value >= defaultValue) return true;
			break;
			
		case PropertyTransition::LOWER_OR_EQUAL:
			if (value <= defaultValue) return true;
			break;
			
		default:
			break;
	}
	
	return false;
}

bool PropertyTransition_compareFloats(PropertyTransition::Condition cond, const float value, const float defaultValue)
{
	switch (cond)
	{
		case PropertyTransition::EQUAL:
			if (value == defaultValue) return true;
			break;
			
		case PropertyTransition::NOT_EQUAL:
			if (value != defaultValue) return true;
			break;
			
		case PropertyTransition::GREATER:
			if (value > defaultValue) return true;
			break;
			
		case PropertyTransition::LOWER:
			if (value < defaultValue) return true;
			break;
			
		case PropertyTransition::GREATER_OR_EQUAL:
			if (value >= defaultValue) return true;
			break;
			
		case PropertyTransition::LOWER_OR_EQUAL:
			if (value <= defaultValue) return true;
			break;
			
		default:
			break;
	}
	
	return false;
}

bool PropertyTransition_compareStrings(PropertyTransition::Condition cond, const char * value, const char * defaultValue)
{
	switch (cond)
	{
		case PropertyTransition::EQUAL:
			if (strcmp(value, defaultValue) == 0) return true;
			break;
			
		case PropertyTransition::NOT_EQUAL:
			if (strcmp(value, defaultValue) != 0) return true;
			break;
			
		case PropertyTransition::GREATER:
			if (strcmp(value, defaultValue) > 0) return true;
			break;
			
		case PropertyTransition::LOWER:
			if (strcmp(value, defaultValue) < 0) return true;
			break;
			
		case PropertyTransition::GREATER_OR_EQUAL:
			if (strcmp(value, defaultValue) >= 0) return true;
			break;
			
		case PropertyTransition::LOWER_OR_EQUAL:
			if (strcmp(value, defaultValue) <= 0) return true;
			break;
			
		default:
			break;
	}
	
	return false;
}

IState * PropertyTransition::evaluate(IState * current, IMachine * fsm)
{
	IObject * obj = ((FiniteStateMachine *)fsm)->getProperty(m_sPropertyName);
	if (!obj)
	{
		return NULL;
	}
	
	// compare the property value here...
	
	if (String * pString = dynamic_cast<String *>(obj))
	{
		if (m_iValueType == INTEGER)
		{
			if (PropertyTransition_compareIntegers(m_iCondition, pString->intValue(), atoi(m_sPropertyValue.c_str())))
			{
				return StateTransition::evaluate(current, fsm);
			}
		}
		else if (m_iValueType == FLOAT)
		{
			if (PropertyTransition_compareFloats(m_iCondition, pString->floatValue(), atof(m_sPropertyValue.c_str())))
			{
				return StateTransition::evaluate(current, fsm);
			}
		}
		else if (m_iValueType == STRING)
		{
			if (PropertyTransition_compareStrings(m_iCondition, pString->c_str(), m_sPropertyValue.c_str()))
			{
				return StateTransition::evaluate(current, fsm);
			}
		}
	}
	
	return NULL;
}

NAMESPACE_END
