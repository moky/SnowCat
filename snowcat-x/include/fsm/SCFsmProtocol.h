//
//  SCFsmProtocol.h
//  SnowCat
//
//  Created by Moky on 13-8-27.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCFsmProtocol_h
#define SnowCat_SCFsmProtocol_h

#include "scMacros.h"

#define FSM_NAMESPACE fsm

NAMESPACE_BEGIN(SC_NAMESPACE)
NAMESPACE_BEGIN(FSM_NAMESPACE)

class IState;

class IDelegate
{
public:
	virtual void fsmEnterState(IState * state) = 0;
	virtual void fsmExitState(IState * state) = 0;
	
	virtual void fsmPauseState(IState * state) {}
	virtual void fsmResumeState(IState * state) {}
};

class IMachine
{
public:
	virtual void start(void) {}
	virtual void stop(void) {}
	
	virtual void tick(void) = 0;
	
	virtual bool changeState(IState * state) = 0;
	
	virtual void pause(void) {}
	virtual void resume(void) {}
};

class IState
{
public:
	virtual void tick(IMachine * fsm) = 0;
	
	virtual void enter(IMachine * fsm) = 0;
	virtual void exit(IMachine * fsm) = 0;
	
	virtual void pause(IMachine * fsm) {}
	virtual void resume(IMachine * fsm) {}
};

class ITransition
{
public:
	virtual IState * evaluate(IState * current, IMachine * fsm) = 0;
};

NAMESPACE_END
NAMESPACE_END

#endif
