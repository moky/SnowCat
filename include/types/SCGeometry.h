//
//  SCGeometry.h
//  SnowCat
//
//  Created by Moky on 13-9-17.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCGeometry_h
#define SnowCat_SCGeometry_h

#include "scMacros.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

#define FloatEqual(a, b) ((a) - (b) < 0.000001 && (a) - (b) > -0.000001)

class Size;

class Point
{
public:
	float x;
	float y;
	
public:
	Point(void);
	Point(const float _x, const float _y);
	Point(const Point & other);
	
	// assign
	inline Point & operator = (const Point & other) {
		x = other.x;
		y = other.y;
		return *this;
	}
	Point & operator = (const Size & size);
	
	// equal
	inline bool operator == (const Point & other) const {
		return FloatEqual(x, other.x) && FloatEqual(y, other.y);
	}
	
	// add
	inline Point & operator + (const Point & other) const {
		return Point(*this) += other;
	}
	inline Point & operator += (const Point & other) {
		x += other.x;
		y += other.y;
		return *this;
	}
	
	// sub
	inline Point & operator - (const Point & other) const {
		return Point(*this) -= other;
	}
	inline Point & operator -= (const Point & other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	
	// multiply
	inline Point & operator * (const Point & other) const {
		return Point(*this) *= other;
	}
	inline Point & operator *= (const Point & other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}
};

class Size
{
public:
	float width;
	float height;
	
public:
	Size(void);
	Size(const float w, const float h);
	Size(const Size & other);
	
	// assign
	inline Size & operator = (const Size & other) {
		width = other.width;
		height = other.height;
		return *this;
	}
	Size & operator = (const Point & point);
	
	// equal
	inline bool operator == (const Size & other) const {
		return FloatEqual(width, other.width) && FloatEqual(height, other.height);
	}
	
	// multiply
	inline Size & operator * (const float scale) const {
		return Size(*this) *= scale;
	}
	inline Size & operator *= (const float scale) {
		width *= scale;
		height *= scale;
		return *this;
	}
};

class Rect
{
public:
	Point origin;
	Size size;
	
public:
	Rect(void);
	Rect(const float x, const float y, const float width, const float height);
	Rect(const Point & p, const Size & s);
	Rect(const Rect & other);
	
	inline Rect & operator = (const Rect & other) {
		origin = other.origin;
		size = other.size;
		return *this;
	}
	
	inline bool operator == (const Rect & other) const {
		return origin == other.origin && size == other.size;
	}
	
	// center
	inline Point & center(void) const {
		return Point(size.width * 0.5, size.height * 0.5) += origin;
	}
	
	// contain
	inline bool contains(const Point & point) const {
		return origin.x <= point.x
			&& origin.y <= point.y
			&& origin.x + size.width >= point.x
			&& origin.y + size.height >= point.y
		;
	}
	
	inline bool contains(const Rect & other) const {
		return origin.x <= other.origin.x
			&& origin.y <= other.origin.y
			&& origin.x + size.width >= other.origin.x + other.size.width
			&& origin.y + size.height >= other.origin.x + other.size.height
		;
	}
	
	// intersect
	inline bool intersects(const Rect & other) const {
		return origin.x <= other.origin.x + other.size.width
			&& origin.y <= other.origin.y + other.size.height
			&& origin.x + size.width >= other.origin.x
			&& origin.y + size.height >= other.origin.y
		;
	}
};

NAMESPACE_END

#endif
