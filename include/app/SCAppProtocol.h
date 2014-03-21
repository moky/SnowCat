//
//  SCAppProtocol.h
//  SnowCat
//
//  Created by Moky on 13-9-27.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCAppProtocol_h
#define SnowCat_SCAppProtocol_h

#include "scMacros.h"
#include "SCGeometry.h"

#define APP_NAMESPACE app

NAMESPACE_BEGIN(SC_NAMESPACE)
NAMESPACE_BEGIN(APP_NAMESPACE)

class IBoxModel {
	
public:
	IBoxModel(void) {
		setMargin(0);
		setPadding(0);
		setBorder(0);
	}
	
	// margin
	virtual float getMarginTop(void) const { return m_fMarginTop; }
	virtual float getMarginLeft(void) const { return m_fMarginLeft; }
	virtual float getMarginRight(void) const { return m_fMarginRight; }
	virtual float getMarginBottom(void) const { return m_fMarginBottom; }
	
	virtual void setMarginTop(const float marginTop) { m_fMarginTop = marginTop; }
	virtual void setMarginLeft(const float marginLeft) { m_fMarginLeft = marginLeft; }
	virtual void setMarginRight(const float marginRignt) { m_fMarginRight = marginRignt; }
	virtual void setMarginBottom(const float marginBottom) { m_fMarginBottom = marginBottom; }
	inline void setMargin(const float margin) {
		setMarginTop(margin);
		setMarginLeft(margin);
		setMarginRight(margin);
		setMarginBottom(margin);
	}
	
	// padding
	virtual float getPaddingTop(void) const { return m_fPaddingTop; }
	virtual float getPaddingLeft(void) const { return m_fPaddingLeft; }
	virtual float getPaddingRight(void) const { return m_fPaddingRight; }
	virtual float getPaddingBottom(void) const { return m_fPaddingBottom; }
	
	virtual void setPaddingTop(const float paddingTop) { m_fPaddingTop = paddingTop; }
	virtual void setPaddingLeft(const float paddingLeft) { m_fPaddingLeft = paddingLeft; }
	virtual void setPaddingRight(const float paddingRignt) { m_fPaddingRight = paddingRignt; }
	virtual void setPaddingBottom(const float paddingBottom) { m_fPaddingBottom = paddingBottom; }
	inline void setPadding(const float padding) {
		setPaddingTop(padding);
		setPaddingLeft(padding);
		setPaddingRight(padding);
		setPaddingBottom(padding);
	}
	
	// border
	virtual float getBorderTop(void) const { return m_fBorderTop; }
	virtual float getBorderLeft(void) const { return m_fBorderLeft; }
	virtual float getBorderRight(void) const { return m_fBorderRight; }
	virtual float getBorderBottom(void) const { return m_fBorderBottom; }
	
	virtual void setBorderTop(const float borderTop) { m_fBorderTop = borderTop; }
	virtual void setBorderLeft(const float borderLeft) { m_fBorderLeft = borderLeft; }
	virtual void setBorderRight(const float borderRignt) { m_fBorderRight = borderRignt; }
	virtual void setBorderBottom(const float borderBottom) { m_fBorderBottom = borderBottom; }
	inline void setBorder(const float border) {
		setBorderTop(border);
		setBorderLeft(border);
		setBorderRight(border);
		setBorderBottom(border);
	}
	
protected:
	// margin
	float m_fMarginTop;
	float m_fMarginLeft;
	float m_fMarginRight;
	float m_fMarginBottom;
	// padding
	float m_fPaddingTop;
	float m_fPaddingLeft;
	float m_fPaddingRight;
	float m_fPaddingBottom;
	// border
	float m_fBorderTop;
	float m_fBorderLeft;
	float m_fBorderRight;
	float m_fBorderBottom;
};

class IGeometry {
	
public:
	IGeometry(void) {
		setSize(Size(0, 0));
		setAnchorPoint(Point(0.5f, 0.5f));
		setPosition(Point(0, 0));
		setScale(1);
		setRotation(0);
		setZOrder(0);
	}
	
	// size: box without margin
	virtual const Size & getSize(void) const { return m_tSize; }
	inline float getWidth(void) const { return getSize().width; }
	inline float getHeight(void) const { return getSize().height; }
	
	virtual void setSize(const Size & size) { m_tSize = size; }
	inline void setWidth(const float width) { setSize(Size(width, getHeight())); }
	inline void setHeight(const float height) { setSize(Size(getWidth(), height)); }
	
	// anchor
	virtual const Point & getAnchorPoint(void) const { return m_tAnchorPoint; }
	virtual void setAnchorPoint(const Point & anchor) { m_tAnchorPoint = anchor; }
	
	// position
	virtual const Point & getPosition(void) const { return m_tPosition; }
	inline float getPositionX(void) const { return getPosition().x; }
	inline float getPositionY(void) const { return getPosition().y; }
	
	virtual void setPosition(const Point & position) { m_tPosition = position; }
	inline void setPositionX(const float x) { setPosition(Point(x, getPositionY())); }
	inline void setPositionY(const float y) { setPosition(Point(getPositionX(), y)); }
	
	// scale
	virtual float getScaleX(void) const { return m_fScaleX; }
	virtual float getScaleY(void) const { return m_fScaleY; }
	inline float getScale(void) const { return getScaleX(); }
	
	virtual void setScaleX(const float scaleX) { m_fScaleX = scaleX; }
	virtual void setScaleY(const float scaleY) { m_fScaleY = scaleY; }
	inline void setScale(const float scale) {
		setScaleX(scale);
		setScaleY(scale);
	}
	
	// rotation
	virtual float getRotation(void) const { return m_fRotation; }
	virtual void setRotation(const float rotation) { m_fRotation = rotation; }
	
	// z-order
	virtual int getZOrder(void) const { return m_iZOrder; }
	virtual void setZOrder(const int z) { m_iZOrder = z; }
	
protected:
	Size m_tSize;
	Point m_tAnchorPoint;
	Point m_tPosition;

	float m_fScaleX;
	float m_fScaleY;
	float m_fRotation;
	
	int m_iZOrder;
};

class IView : public IGeometry, public IBoxModel {
	
public:
	IView(void) : IGeometry(), IBoxModel(), m_iTag(-1) {
	}
	
	// tag
	virtual int getTag(void) const { return m_iTag; }
	virtual void setTag(const int tag) { m_iTag = tag; }
	
	// events
	virtual void onEnter(void) = 0;
	virtual void onExit(void) = 0;
	virtual void onResize(void) = 0;
	
	// box model
	inline Size getContentSize(void) const {
		Size size = getSize();
		size.width -= getBorderLeft() + getBorderRight() + getPaddingLeft() + getPaddingRight();
		size.height -= getBorderTop() + getBorderBottom() + getPaddingTop() + getPaddingBottom();
		if (size.width < 0) {
			size.width = 0;
		}
		if (size.height < 0) {
			size.height = 0;
		}
		return size;
	}
	
protected:
	int m_iTag;
};

#pragma mark Layout

class ILayout {
	
public:
	static const int fill_parent  = -9527001;
	static const int wrap_content = -9527002;
	
	ILayout(void) : m_fLayoutWidth(wrap_content), m_fLayoutHeight(wrap_content), m_fLayoutWeight(0) {
	}
	
	inline void setLayoutWidth(const float layoutWidth) { m_fLayoutWidth = layoutWidth; }
	inline void setLayoutHeight(const float layoutHeight) { m_fLayoutHeight = layoutHeight; }
	inline void setLayoutWeight(const float layoutWeight) { m_fLayoutWeight = layoutWeight; }
	
	virtual float getLayoutWidth(void) const = 0;
	virtual float getLayoutHeight(void) const = 0;
	inline float getLayoutWeight(void) const { return m_fLayoutWeight; }
	
protected:
	float m_fLayoutWidth;
	float m_fLayoutHeight;
	float m_fLayoutWeight;
};

NAMESPACE_END
NAMESPACE_END

#endif
