//
//  SCAppView.h
//  SnowCat
//
//  Created by Moky on 13-9-27.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCAppView_h
#define SnowCat_SCAppView_h

#include "SCObject.h"
#include "SCArray.h"
#include "SCAppProtocol.h"

#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)
#include "SCAppJNI.h"
#endif

NAMESPACE_BEGIN(SC_NAMESPACE)
NAMESPACE_BEGIN(APP_NAMESPACE)

class View : public Object, public IView, public ILayout {
	typedef IView super;
	
public:
	View(void);
	virtual ~View(void);
	
	virtual const char * description(void) const;
	
#pragma mark box model
	// margin
	void setMarginTop(const float marginTop);
	void setMarginLeft(const float marginLeft);
	void setMarginRight(const float marginRight);
	void setMarginBottom(const float marginBottom);
	// padding
	void setPaddingTop(const float paddingTop);
	void setPaddingLeft(const float paddingLeft);
	void setPaddingRight(const float paddingRignt);
	void setPaddingBottom(const float paddingBottom);
	// border
	void setBorderTop(const float borderTop);
	void setBorderLeft(const float borderLeft);
	void setBorderRight(const float borderRignt);
	void setBorderBottom(const float borderBottom);
	
#pragma mark geometry
	
	void setSize(const Size & size);
	void setAnchorPoint(const Point & anchor);
	void setPosition(const Point & position);
	
	void setScaleX(const float scaleX);
	void setScaleY(const float scaleY);
	void setRotation(const float rotation);
	
	void setZOrder(const int z);
	
	// update geometry figuration
	virtual void updateGeometry(void);
	
	virtual void updateChildrenLayout(void);
	
	virtual float getLayoutWidth(void) const;
	virtual float getLayoutHeight(void) const;
	
#pragma mark view
	
	// subviews
	virtual void addSubview(View * view);
	virtual void removeSubview(View * view);
	virtual void removeFromSuperview(void);
	
	inline View * getParent(void) const { return m_pParent; }
	
	// events
	virtual void onEnter(void);
	virtual void onExit(void);
	virtual void onResize(void);
	
#if SC_PLATFORM_IS(SC_PLATFORM_ANDROID)
	virtual jobject getLayout(void);
	virtual void setLayout(jobject layout);
	
	virtual jobject getAvatar(void);
	virtual void setAvatar(jobject avatar);
	
	jobject m_layout;
	jobject m_avatar;
#else
	virtual void * getAvatar(void);
	virtual void setAvatar(void * avatar);
	
	void * m_pAvatar;
#endif
	
protected:
	View * m_pParent;
	Array * m_pChildren;
	
	bool m_bGeometryDirty;
	bool m_bContentSizeDirty;
	bool m_bIsRunning;
};

NAMESPACE_END
NAMESPACE_END

#endif
