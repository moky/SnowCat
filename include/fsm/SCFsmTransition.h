//
//  SCFsmTransition.h
//  SnowCat
//
//  Created by Moky on 13-8-28.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCFsmTransition_h
#define SnowCat_SCFsmTransition_h

#include "SCFsmProtocol.h"

NAMESPACE_BEGIN(SC_NAMESPACE)
NAMESPACE_BEGIN(FSM_NAMESPACE)

template<class S>
class Transition : ITransition
{
public:
	Transition(S * target) : m_pTargetState(target) {}
	virtual ~Transition(void) {}
	
protected:
	S * m_pTargetState;
};

NAMESPACE_END
NAMESPACE_END

#endif
