//
//  SCAppView.mm
//  SnowCat
//
//  Created by Moky on 13-9-27.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCLog.h"
#include "SCAppView.h"

NAMESPACE_BEGIN(SC_NAMESPACE)
NAMESPACE_BEGIN(APP_NAMESPACE)

void * View::getAvatar(void)
{
	if (!m_pAvatar)
	{
		m_pAvatar = [[UIView alloc] init];
	}
	return m_pAvatar;
}

void View::setAvatar(void * avatar)
{
	if (avatar != m_pAvatar)
	{
		[(UIView *)avatar retain];
		[(UIView *)m_pAvatar release];
		m_pAvatar = avatar;
	}
}

void View::addSubview(View * view)
{
	SCAssert(view, "subview cannot be null");
	int index = 0;
	if (!m_pChildren)
	{
		m_pChildren = new Array();
	}
	else if (m_pChildren->count() > 0)
	{
		View * subview;
		for (index = m_pChildren->count() - 1; index >= 0; --index)
		{
			subview = (View *)m_pChildren->objectAtIndex(index);
			if (subview->getZOrder() <= view->getZOrder())
			{
				++index; // insert after this element
				break;
			}
		}
	}
	
	m_pChildren->insertObject(view, index);
	view->m_pParent = this;
	
	// insert avatar
	[(UIView *)getAvatar() insertSubview:(UIView *)view->getAvatar() atIndex:index];
	
	updateChildrenLayout();
	
	if (m_bIsRunning)
	{
		view->onEnter();
	}
}

void View::removeSubview(View *view)
{
	if (!m_pChildren)
	{
		SCError("no children, this: %s", this->description());
		return;
	}
	int index = m_pChildren->indexOfObject(view);
	if (index == UINT_MAX)
	{
		SCError("no such view: %s", view->description());
		return;
	}
	
	if (m_bIsRunning)
	{
		view->onExit();
	}
	
	// remove avatar
	[(UIView *)view->getAvatar() removeFromSuperview];
	
	view->m_pParent = NULL;
	m_pChildren->removeObjectAtIndex(index);
}

void View::updateGeometry(void)
{
	if (m_bGeometryDirty)
	{
		Point point;
		if (m_pParent)
		{
			// get origin point of parent
			point.x = m_pParent->getBorderLeft() + m_pParent->getPaddingLeft();
			point.y = m_pParent->getBorderTop() + m_pParent->getPaddingTop();
		}
		point += m_tPosition; // position of anchor point
		
		point.x += m_tSize.width * (0.5 - m_tAnchorPoint.x);
		point.y += m_tSize.height * (0.5 - m_tAnchorPoint.y);
		
		// TODO: not consider rotation yet
		SCLog(description());
		
		UIView *uiview = (UIView *)getAvatar();
		[uiview setBounds:CGRectMake(0, 0, m_tSize.width, m_tSize.height)];
		[uiview setCenter:CGPointMake(point.x, point.y)];
		
		m_bGeometryDirty = false;
	}
}

NAMESPACE_END
NAMESPACE_END
