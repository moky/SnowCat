//
//  SCInteger.cpp
//  SnowCat
//
//  Created by Moky on 13-8-24.
//  Copyright (c) 2013 Slanissue Inc. All rights reserved.
//

#include <stdlib.h>
#include <string.h>

#include "SCLog.h"
#include "SCString.h"
#include "SCInteger.h"

NAMESPACE_BEGIN(SC_NAMESPACE)

Integer::Integer(void)
: Object()
, m_pData(NULL)
, m_uLen(0)
, m_bNegative(false)
{
	init();
}

Integer::Integer(const Integer & other)
: Object()
, m_pData(NULL)
, m_uLen(0)
, m_bNegative(false)
{
	*this = other;
}

Integer::~Integer(void)
{
	if (m_pData)
	{
		free(m_pData);
		m_pData = NULL;
	}
}

bool Integer::init(unsigned int size)
{
	SCAssert(size > 0, "error size");
	if (m_pData)
	{
		free(m_pData);
		m_pData = NULL;
	}
	m_uLen = 0;
	m_bNegative = false;
	
	return resize(size);
}

bool Integer::resize(unsigned int size)
{
	if (size == 0 && m_uLen > 0)
	{
		// detect empty spaces
		size = m_uLen;
		int mid;
		while (size > 1)
		{
			mid = size / 2;
			
			for (int i = mid; i < size; i++)
			{
				if (m_pData[i] != 0)
				{
					mid = -1;
					break;
				}
			}
			
			if (mid < 1)
			{
				break;
			}
			size = mid;
		}
	}
	else if (size <= m_uLen)
	{
		SCWarning("ignore this operation: %u <= %u", size, m_uLen);
		return false;
	}
	
	if (size < 1 || size == m_uLen)
	{
		SCLog("ignore this operation: %u, %u", size, m_uLen);
		return false;
	}
	
	if (!m_pData)
	{
		UNIT_TYPE * p = (UNIT_TYPE *)malloc(size * sizeof(UNIT_TYPE));
		SCAssert(p, "Not enough memory");
		if (p)
		{
			memset(p, 0, size * sizeof(UNIT_TYPE));
			m_pData = p;
			m_uLen = size;
		}
	}
	else
	{
		UNIT_TYPE * p = (UNIT_TYPE *)realloc(m_pData, size * sizeof(UNIT_TYPE));
		SCAssert(p, "Not enough memory");
		if (p)
		{
			if (size > m_uLen)
			{
				memset(p + m_uLen, 0, (size - m_uLen) * sizeof(UNIT_TYPE)); // make sure the new memory is empty
			}
			m_pData = p;
			m_uLen = size;
		}
	}
	
	if (!m_pData || m_uLen != size)
	{
		SCError("Not enough memory");
		return false;
	}
	
	return true;
}

const char * Integer::description(void) const
{
	String * string = new String();
	SCAssert(string, "Not enough memory");
	StringUtils::stringWithFormat(*string, "%s data size: %d, value: %s",
								  Object::description(),
								  this->m_uLen * sizeof(UNIT_TYPE), this->stringValue(",")->c_str());
	string->autorelease();
	return string->c_str();
}

String * Integer::stringValue(const char * separator) const
{
	String * string = new String(m_bNegative ? "-0x" : "0x");
	SCAssert(string, "Not enough memory");
	string->autorelease();
	
	UNIT_TYPE * p = m_pData + m_uLen - 1;
	char sz[9];
	for (int i = 0; i < m_uLen; i++, p--)
	{
		memset(sz, 0, 9);
		snprintf(sz, 9, "%08lX", *p);
		string->append(sz);
		if (separator && i < m_uLen - 1)
		{
			string->append(separator);
		}
	}
	return string;
}

/**
 *  returns:
 *      -1, a < b
 *       0, a == b
 *       1, a > b
 */
int Integer::compare(const Integer & a, const Integer & b)
{
	// pre-check: ZERO ?
	if (a.isZero())
	{
		if (b.isZero())
		{
			return 0; // 0 == 0
		}
		return b.m_bNegative ? 1 : -1; // 0 <=> b
	}
	else if (b.isZero())
	{
		return a.m_bNegative ? -1 : 1; // a <=> 0
	}
	
	// pre-check: Negative ?
	if (a.m_bNegative && !b.m_bNegative)
	{
		return -1; // -a < +b
	}
	else if (!a.m_bNegative && b.m_bNegative)
	{
		return 1; // +a < -b
	}
	SCAssert(a.m_bNegative == b.m_bNegative, "");
	
	//-------------------------------------------
	
	unsigned int end = b.m_uLen;
	UNIT_TYPE * x;
	
	// part 1
	unsigned int i;
	if (a.m_uLen < b.m_uLen)
	{
		// case 1
		x = b.m_pData + a.m_uLen;
		for (i = a.m_uLen; i < b.m_uLen; i++, x++)
		{
			if (*x != 0)
			{
				return a.m_bNegative ? 1 : -1; // abs(a) < abs(b)
			}
		}
		end = a.m_uLen;
	}
	else if (a.m_uLen > b.m_uLen)
	{
		// case 2
		x = a.m_pData + b.m_uLen;
		for (i = b.m_uLen; i < a.m_uLen; i++, x++)
		{
			if (*x != 0)
			{
				return a.m_bNegative ? -1 : 1; // abs(a) > abs(b)
			}
		}
	}
	
	// part 2
	UNIT_TYPE * y;
	x = a.m_pData + (end - 1);
	y = b.m_pData + (end - 1);
	for (i = 0; i < end; i++, x--, y--)
	{
		if (*x < *y)
		{
			return a.m_bNegative ? 1 : -1; // abs(a) < abs(b)
		}
		else if (*x > * y)
		{
			return a.m_bNegative ? -1 : 1; // abs(a) > abs(b)
		}
	}
	
	return 0; // a == b
}

#pragma mark - operators

Integer & Integer::operator = (const Integer & other)
{
	if (init(other.m_uLen))
	{
		memcpy(this->m_pData, other.m_pData, other.m_uLen * sizeof(UNIT_TYPE));
		this->m_bNegative = other.m_bNegative;
	}
	return *this;
}

Integer & Integer::operator + (const Integer & other) const
{
	Integer * res = new Integer(*this);
	SCAssert(res, "Not enough memory");
	*res += other;
	res->autorelease();
	return *res;
}

Integer & Integer::operator - (const Integer & other) const
{
	Integer * res = new Integer(*this);
	SCAssert(res, "Not enough memory");
	*res -= other;
	res->autorelease();
	return *res;
}

Integer & Integer::operator * (const Integer & other) const
{
	Integer * res = new Integer(*this);
	SCAssert(res, "Not enough memory");
	*res *= other;
	res->autorelease();
	return *res;
}

Integer & Integer::operator += (const Integer & other)
{
	SCAssert(this->m_uLen > 0, "");
	while (this->m_uLen < other.m_uLen)
	{
		this->expand();
	}
	
	UNIT_TYPE * x = this->m_pData;
	UNIT_TYPE * y = other.m_pData;
	long long sum;
	
	int i = 0;
	int overflow = 0;
	
	// part 1
	for (; i < other.m_uLen; i++, x++, y++)
	{
		sum = *x;
		sum += overflow;
		if (this->m_bNegative == other.m_bNegative)
		{
			sum += *y;
		}
		else
		{
			sum -= *y;
		}
		
		if (sum < 0)
		{
			overflow = -1;
			*x = sum + UNIT_MAX;
		}
		else if (sum < UNIT_MAX)
		{
			overflow = 0;
			*x = sum;
		}
		else
		{
			overflow = 1;
			*x = sum - UNIT_MAX;
		}
	}
	
	// part 2
	if (overflow != 0)
	for (; i < this->m_uLen; i++, x++)
	{
		sum = *x;
		sum += overflow;
		if (sum < 0)
		{
			overflow = -1;
			*x = sum + UNIT_MAX;
		}
		else if (sum < UNIT_MAX)
		{
			overflow = 0;
			*x = sum;
			break;
		}
		else
		{
			overflow = 1;
			*x = sum - UNIT_MAX;
		}
	}
	
	// part 3
	if (overflow > 0)
	{
		this->expand();
		
		*(this->m_pData + i) = overflow;
	}
	else if (overflow < 0)
	{
		bool flag = this->m_bNegative;
		this->m_bNegative = false;
		
		Integer g;
		g.resize(this->m_uLen * 2);
		
		g.m_pData[this->m_uLen] = 1; // set value:  0x100000000...00000000
		g -= *this;
		g.m_bNegative = !flag;
		
		g.resize();
		*this = g;
	}
	
	return *this;
}

Integer & Integer::operator -= (const Integer & other)
{
	/**
	 *    a - b <=> -(-a + b)
	 */
	this->m_bNegative = !(this->m_bNegative);
	*this += other;
	this->m_bNegative = !(this->m_bNegative);
	
	return *this;
}

Integer & Integer::operator *= (const Integer & other)
{
	unsigned int size = this->m_uLen > other.m_uLen ? this->m_uLen * 2 : other.m_uLen * 2;
	
	Integer product;
	product.resize(size);
	
	UNIT_TYPE * x = this->m_pData;
	UNIT_TYPE * y;
	UNIT_TYPE * z;
	unsigned long long pro;
	
	unsigned long overflow = 0;
	
	// part 1
	for (int i = 0; i < this->m_uLen; i++, x++)
	{
		y = other.m_pData;
		z = product.m_pData + i;
		for (int j = 0; j < other.m_uLen; j++, y++, z++)
		{
			pro = *x;
			pro *= *y;
			pro += overflow;
			pro += *z;
			
			if (pro < UNIT_MAX)
			{
				*z = pro;
				overflow = 0;
			}
			else
			{
				overflow = pro / UNIT_MAX;
				*z = pro - overflow * UNIT_MAX;
			}
		}
	}
	
	// part 2
	*z = overflow;
	
	product.resize();
	*this = product;
	
	this->m_bNegative ^= other.m_bNegative;
	return *this;
}

NAMESPACE_END
