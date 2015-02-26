//
//  SCGeometry.cpp
//  SnowCat
//
//  Created by Moky on 13-9-17.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include "SCGeometry.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

// point

Point::Point(void) : x(0), y(0)
{
	
}

Point::Point(const float _x, const float _y) : x(_x), y(_y)
{
	
}

Point::Point(const Point & other) : x(other.x), y(other.y)
{
	
}

Point & Point::operator = (const Size & size)
{
	x = size.width;
	y = size.height;
	return *this;
}

// size

Size::Size(void) : width(0), height(0)
{
	
}

Size::Size(const float w, const float h) : width(w), height(h)
{
	
}

Size::Size(const Size & other) : width(other.width), height(other.height)
{
	
}

Size & Size::operator = (const Point & point)
{
	width = point.x;
	height = point.y;
	return *this;
}

// rect

Rect::Rect(void) : origin(), size()
{
	
}

Rect::Rect(const float x, const float y, const float width, const float height) : origin(x, y), size(width, height)
{
	
}

Rect::Rect(const Point & p, const Size & s) : origin(p), size(s)
{
	
}

Rect::Rect(const Rect & other) : origin(other.origin), size(other.size)
{
	
}

NAMESPACE_END
