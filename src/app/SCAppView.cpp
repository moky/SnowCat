//
//  SCAppView.cpp
//  SnowCat
//
//  Created by Moky on 13-9-27.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCLog.h"
#include "SCString.h"
#include "SCAppView.h"
#include "SCAppJNI.h"

NAMESPACE_BEGIN(SC_NAMESPACE)
NAMESPACE_BEGIN(APP_NAMESPACE)

View::View(void)
: Object()
, IView()
, ILayout()
#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)
, m_layout(NULL)
, m_avatar(NULL)
#else
, m_pAvatar(NULL)
#endif
, m_pParent(NULL)
, m_pChildren(NULL)
, m_bGeometryDirty(true)
, m_bContentSizeDirty(true)
, m_bIsRunning(false)
{
	
}

View::~View(void)
{
	if (m_pChildren)
	{
		delete m_pChildren;
		m_pChildren = NULL;
	}
	setAvatar(NULL);
#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)
	setLayout(NULL);
#endif
}

const char * View::description(void) const
{
	String * string = new String();
	SCAssert(string, "Not enough memory");
	StringUtils::stringWithFormat(*string, "%s tag: %d, layout: {%.2f, %.2f, %.2f}, size: {%.2f, %.2f}, position: {%.2f, %.2f}, children count: %d",
								  Object::description(),
								  this->getTag(),
								  this->getLayoutWidth(), this->getLayoutHeight(), this->getLayoutWeight(),
								  this->getWidth(), this->getHeight(),
								  this->getPositionX(), this->getPositionY(),
								  (m_pChildren ? m_pChildren->count() : 0)
								  );
	string->autorelease();
	return string->c_str();
}

void View::onEnter(void)
{
	IObject * obj;
	SC_ARRAY_FOREACH(m_pChildren, obj)
	{
		((IView *)obj)->onEnter();
	}
	
	m_bIsRunning = true;
}

void View::onExit(void)
{
	m_bIsRunning = false;
	
	IObject * obj;
	SC_ARRAY_FOREACH_REVERSE(m_pChildren, obj)
	{
		((IView *)obj)->onExit();
	}
}

void View::onResize(void)
{
//	SCLog(description());
}

void View::updateChildrenLayout(void)
{
	if (!m_bContentSizeDirty)
	{
		return;
	}
	m_bContentSizeDirty = true;
	
	Size contentSize = getContentSize();
	if (contentSize == Size(0, 0))
	{
		return;
	}
	
	float remainWidth = contentSize.width;
	float remainHeight = contentSize.height;
	
	float weight;
	float totalWeight = 0;
	
	IObject * obj;
	View * subview;
	
	// 1. measure
	SC_ARRAY_FOREACH(m_pChildren, obj)
	{
		subview = (View *)obj;
		// size
		remainWidth -= subview->getLayoutWidth();
		remainHeight -= subview->getLayoutHeight();
		// weight
		weight = subview->getLayoutWeight();
		if (FloatGreater(weight, 0))
		{
			totalWeight += weight;
		}
	}
	
	// 2. layout
	bool bHorizontalLayout = remainWidth > 0 && totalWeight > 0;
	bool bVerticalLayout = remainHeight > 0 && totalWeight > 0;
	
	Point offset(0, 0);
	
	Size size;
	Point anchor;
	Point position;
	
	SC_ARRAY_FOREACH(m_pChildren, obj)
	{
		subview = (View *)obj;
		// size
		size.width = subview->getLayoutWidth();
		size.height = subview->getLayoutHeight();
		
		weight = subview->getLayoutWeight();
		if (weight > 0)
		{
			if (bHorizontalLayout)
			{
				size.width += remainWidth * weight / totalWeight;
			}
			if (bVerticalLayout)
			{
				size.height += remainHeight * weight / totalWeight;
			}
		}
		subview->setWidth(DIV(size.width, subview->getScaleX()));
		subview->setHeight(DIV(size.height, subview->getScaleY()));
		
		subview->updateChildrenLayout(); // recursively
		
		// position
		position = offset;
		anchor = subview->getAnchorPoint();
		
		position.x += size.width * anchor.x;
		position.y += size.height * anchor.y;
		subview->setPosition(position);
		subview->updateGeometry();
		
		if (bHorizontalLayout)
		{
			offset.x += size.width;
		}
		if (bVerticalLayout)
		{
			offset.y += size.height;
		}
	}
}

float View::getLayoutWidth(void) const
{
	float width = m_fLayoutWidth;
	if (FloatEqual(width, fill_parent))
	{
		if (m_pParent)
		{
			return m_pParent->getContentSize().width;
		}
	}
	else if (FloatEqual(width, wrap_content))
	{
		// TODO: trim content width
		return getSize().width;
	}
	
	return MAX(width, 0);
}

float View::getLayoutHeight(void) const
{
	float height = m_fLayoutHeight;
	if (FloatEqual(height, fill_parent))
	{
		if (m_pParent)
		{
			return m_pParent->getContentSize().height;
		}
	}
	else if (FloatEqual(height, wrap_content))
	{
		// TODO: trim content height
		return getSize().height;
	}
	
	return MAX(height, 0);
}

void View::removeFromSuperview(void)
{
	if (!m_pParent)
	{
		SCError("no parent, this: %s", this->description());
		return;
	}
	m_pParent->removeSubview(this);
}

#pragma mark geometry

void View::setSize(const Size & size)
{
	if (size != m_tSize)
	{
		super::setSize(size);
		m_bGeometryDirty = true;
		m_bContentSizeDirty = true;
	}
}

void View::setAnchorPoint(const Point & anchor)
{
	if (anchor != m_tAnchorPoint)
	{
		super::setAnchorPoint(anchor);
		m_bGeometryDirty = true;
	}
}

void View::setPosition(const Point & position)
{
	if (position != m_tPosition)
	{
		super::setPosition(position);
		m_bGeometryDirty = true;
	}
}

void View::setScaleX(const float scaleX)
{
	if (scaleX != m_fScaleX)
	{
		super::setScaleX(scaleX);
		m_bGeometryDirty = true;
		m_bContentSizeDirty = true;
	}
}

void View::setScaleY(const float scaleY)
{
	if (scaleY != m_fScaleY)
	{
		super::setScaleY(scaleY);
		m_bGeometryDirty = true;
		m_bContentSizeDirty = true;
	}
}

void View::setRotation(const float rotation)
{
	if (rotation != m_fRotation)
	{
		super::setRotation(rotation);
		m_bGeometryDirty = true;
	}
}

void View::setZOrder(const int z)
{
	if (z != m_iZOrder)
	{
		super::setZOrder(z);
		View * parent = m_pParent;
		if (parent)
		{
			removeFromSuperview();
			parent->addSubview(this);
		}
	}
}

#pragma mark box model

// margin
void View::setMarginTop(const float marginTop)
{
	if (marginTop != m_fMarginTop)
	{
		super::setMarginTop(marginTop);
		m_bGeometryDirty = true;
	}
}

void View::setMarginLeft(const float marginLeft)
{
	if (marginLeft != m_fMarginLeft)
	{
		super::setMarginLeft(marginLeft);
		m_bGeometryDirty = true;
	}
}

void View::setMarginRight(const float marginRight)
{
	if (marginRight != m_fMarginRight)
	{
		super::setMarginRight(marginRight);
		m_bGeometryDirty = true;
	}
}

void View::setMarginBottom(const float marginBottom)
{
	if (marginBottom != m_fMarginBottom)
	{
		super::setMarginBottom(marginBottom);
		m_bGeometryDirty = true;
	}
}

// padding
void View::setPaddingTop(const float paddingTop)
{
	if (paddingTop != m_fPaddingTop)
	{
		super::setPaddingTop(paddingTop);
		m_bGeometryDirty = true;
		m_bContentSizeDirty = true;
	}
}

void View::setPaddingLeft(const float paddingLeft)
{
	if (paddingLeft != m_fPaddingLeft)
	{
		super::setPaddingLeft(paddingLeft);
		m_bGeometryDirty = true;
		m_bContentSizeDirty = true;
	}
}

void View::setPaddingRight(const float paddingRignt)
{
	if (paddingRignt != m_fPaddingRight)
	{
		super::setPaddingRight(paddingRignt);
		m_bGeometryDirty = true;
		m_bContentSizeDirty = true;
	}
}

void View::setPaddingBottom(const float paddingBottom)
{
	if (paddingBottom != m_fPaddingBottom)
	{
		super::setPaddingBottom(paddingBottom);
		m_bGeometryDirty = true;
		m_bContentSizeDirty = true;
	}
}

// border
void View::setBorderTop(const float borderTop)
{
	if (borderTop != m_fBorderTop)
	{
		super::setBorderTop(borderTop);
		m_bGeometryDirty = true;
		m_bContentSizeDirty = true;
	}
}

void View::setBorderLeft(const float borderLeft)
{
	if (borderLeft != m_fBorderLeft)
	{
		super::setBorderLeft(borderLeft);
		m_bGeometryDirty = true;
		m_bContentSizeDirty = true;
	}
}

void View::setBorderRight(const float borderRignt)
{
	if (borderRignt != m_fBorderRight)
	{
		super::setBorderRight(borderRignt);
		m_bGeometryDirty = true;
		m_bContentSizeDirty = true;
	}
}

void View::setBorderBottom(const float borderBottom)
{
	if (borderBottom != m_fBorderBottom)
	{
		super::setBorderBottom(borderBottom);
		m_bGeometryDirty = true;
		m_bContentSizeDirty = true;
	}
}

#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)

#pragma mark - JNI

// Android
jobject View::getAvatar(void)
{
	if (!m_avatar)
	{
		m_avatar = createViewJNI("android.view.View");
	}
	return m_avatar;
}

void View::setAvatar(jobject avatar)
{
	if (avatar != m_avatar)
	{
		// retain, release
		m_avatar = avatar;
	}
}

jobject View::getLayout(void)
{
	if (!m_layout)
	{
		m_layout = createViewJNI("android.widget.AbsoluteLayout");
	}
	return m_layout;
}

void View::setLayout(jobject layout)
{
	if (layout != m_layout)
	{
		// retain, release
		m_layout = layout;
	}
}

void View::addSubview(View * view)
{
	//	SCAssert(view, "subview cannot be null");
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
	view->m_pParent = this;
	m_pChildren->insertObject(view, index);
	
	// insert avatar
	insertSubviewJNI(getLayout(), view->getAvatar(), index * 2);
	insertSubviewJNI(getLayout(), view->getLayout(), index * 2 + 1);
	
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
	removeSubviewJNI(getLayout(), index * 2 + 1);
	removeSubviewJNI(getLayout(), index * 2);
	
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
		
		SCLog("set avatar frame");
		setViewFrameJNI(getAvatar(), m_tSize, point);
		SCLog("set layout frame");
		setViewFrameJNI(getLayout(), m_tSize, point);
		
		m_bGeometryDirty = false;
	}
}

#endif // EOF '#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)'

NAMESPACE_END
NAMESPACE_END
