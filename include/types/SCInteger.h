//
//  SCInteger.h
//  SnowCat
//
//  Created by Moky on 13-8-24.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#ifndef SnowCat_SCInteger_h
#define SnowCat_SCInteger_h

#include "SCObject.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

class String;

/**
 *
 *  NOTICE:
 *
 *      It should only used for Unlimited Big Integer !
 *
 */

class Integer : Object
{
public:
	typedef unsigned long UNIT_TYPE;
	static const long long UNIT_MAX = 0x100000000; // 32 bits
	
	Integer(void);
	Integer(const Integer & other);
	virtual ~Integer(void);
	virtual bool init(unsigned int size = 1);
	
	virtual bool resize(unsigned int size = 0); // 'size = 0' means to cut down empty spaces
	
	inline bool expand(void) {
		return resize(m_uLen * 2);
	}
	
	virtual const char * description(void) const;
	
	virtual Integer * copy(void) const {
		return new Integer(*this);
	}
	
	virtual String * stringValue(const char * separator = ",") const;
	
	virtual long long integerValue(void) const {
		long long ago = m_pData ? *m_pData : 0;
		return m_bNegative ? -ago : ago;
	}
	
	static inline const Integer & ZERO(void) {
		static const Integer _zero;
		return _zero;
	}
	static inline const Integer & ONE(void) {
		static const Integer _one(1);
		return _one;
	}
	static inline const Integer & NEG_ONE(void) {
		static const Integer _neg_one(-1);
		return _neg_one;
	}
	
	inline bool isZero(void) const {
		UNIT_TYPE * x = m_pData;
		for (unsigned int i = 0; i < m_uLen; ++i, ++x) {
			if (*x != 0) return false;
		}
		return true; // no mater what the negative value is.
	}
	
	/**
	 *  returns:
	 *      -1, a < b
	 *       0, a == b
	 *       1, a > b
	 */
	static int compare(const Integer & a, const Integer & b);
	
	inline int compare(const Integer & other) const {
		return compare(*this, other);
	}
	
#pragma mark - operators
	
	Integer & operator = (const Integer & other);
	
	Integer & operator + (const Integer & other) const;
	Integer & operator - (const Integer & other) const;
	Integer & operator * (const Integer & other) const;
	
	Integer & operator += (const Integer & other);
	Integer & operator -= (const Integer & other);
	Integer & operator *= (const Integer & other);
	
#pragma mark in/decreasement
	
	// ++i;
	inline Integer & operator ++ (void) {
		*this += ONE();
		return *this;
	}
	// --i;
	inline Integer & operator -- (void) {
		*this += NEG_ONE();
		return *this;
	}
	// i++;
	inline const Integer & operator ++ (int) {
		Integer * tmp = new Integer(*this);
		SCAssert(tmp, "Not enough memory");
		tmp->autorelease();
		*this += ONE();
		return *tmp;
	}
	// i--;
	inline const Integer & operator -- (int) {
		Integer * tmp = new Integer(*this);
		SCAssert(tmp, "Not enough memory");
		tmp->autorelease();
		*this += NEG_ONE();
		return *tmp;
	}
	
#pragma mark compares
	
	inline bool operator == (const Integer & other) const {
		return this->compare(other) == 0;
	}
	inline bool operator < (const Integer & other) const {
		return this->compare(other) < 0;
	}
	inline bool operator > (const Integer & other) const {
		return this->compare(other) > 0;
	}
	inline bool operator <= (const Integer & other) const {
		return this->compare(other) <= 0;
	}
	inline bool operator >= (const Integer & other) const {
		return this->compare(other) >= 0;
	}
	inline bool operator != (const Integer & other) const {
		return this->compare(other) != 0;
	}
	
#pragma mark - templates
	
	template<typename T>
	Integer(const T & t)
	: m_pData(NULL)
	, m_uLen(0)
	, m_bNegative(false)
	{
		*this = t;
	}
	
	template<typename T>
	Integer & operator = (const T & t) {
		if (t > -UNIT_MAX && t < UNIT_MAX) {
			if (init()) {
				if (t < 0) {
					m_bNegative = true;
					*m_pData = -t;
				} else {
					*m_pData = t;
				}
			}
		} else {
			if (init(2)) {
				if (t < 0) {
					m_bNegative = true;
					*(m_pData + 1) = -t / UNIT_MAX;
					*m_pData = -t - *(m_pData + 1) * UNIT_MAX;
				} else {
					*(m_pData + 1) = t / UNIT_MAX;
					*m_pData = t - *(m_pData + 1) * UNIT_MAX;
				}
			}
		}
		return *this;
	}
	
#pragma mark add
	
	template<typename T>
	inline Integer & operator + (const T & t) const {
		Integer * res = new Integer(*this);
		res->autorelease();
		*res += Integer(t);
		return *res;
	}
	
	template<typename T>
	inline Integer & operator += (const T & t) {
		*this += Integer(t);
		return *this;
	}
	
#pragma mark minus
	
	template<typename T>
	inline Integer & operator - (const T & t) const {
		Integer * res = new Integer(*this);
		res->autorelease();
		*res -= Integer(t);
		return *res;
	}
	
	template<typename T>
	inline Integer & operator -= (const T & t) {
		*this -= Integer(t);
		return *this;
	}
	
#pragma mark multiply
	
	template<typename T>
	inline Integer & operator * (const T & t) const {
		Integer * res = new Integer(*this);
		res->autorelease();
		*res *= Integer(t);
		return *res;
	}
	
	template<typename T>
	inline Integer & operator *= (const T & t) {
		*this *= Integer(t);
		return *this;
	}
	
#pragma mark compares
	
	template<typename T>
	inline bool operator == (const T & t) const {
		return *this == Integer(t);
	}
	
	template<typename T>
	inline bool operator < (const T & t) const {
		return *this < Integer(t);
	}
	
	template<typename T>
	inline bool operator > (const T & t) const {
		return *this > Integer(t);
	}
	
	template<typename T>
	inline bool operator <= (const T & t) const {
		return *this <= Integer(t);
	}
	
	template<typename T>
	inline bool operator >= (const T & t) const {
		return *this >= Integer(t);
	}
	
	template<typename T>
	inline bool operator != (const T & t) const {
		return *this != Integer(t);
	}
	
private:
	UNIT_TYPE *  m_pData;
	unsigned int m_uLen;
	bool         m_bNegative;
};

NAMESPACE_END

#endif
