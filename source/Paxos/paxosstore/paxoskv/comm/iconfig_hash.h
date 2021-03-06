
/*
* Tencent is pleased to support the open source community by making PaxosStore available.
* Copyright (C) 2017 THL A29 Limited, a Tencent company. All rights reserved.
* Licensed under the BSD 3-Clause License (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
* https://opensource.org/licenses/BSD-3-Clause
* Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
*/



#pragma once

#include<vector>
#include<string>

namespace Comm 
{

class CStringSlice;
class ConfigFakeVector;
class ConfigHashNode;
class ConfigHashTable;

/*!
 * \brief	locate a paticular slice of a string using two pointers, 
 * 			other than copying to another memory space.
 * \note    the string must be accessed in memory.
 * 			It's not advisable to use this class unless you test how to.           
 * \as	testIConfigHashCStringSlice.cpp
 */
/* Store index of string */
class CStringSlice {

	public:

	
	/*!
	 * \brief	constructor 初始化一个空字符串
	 */
	CStringSlice(); 

	/*!
	 * \brief	根据一个字符串的首尾指针构造对象
	 *
	 * \param	pcStart 字符串的头指针
	 * \param	pcEnd 字符串的尾指针
	 */
	CStringSlice(const char *pcStart, const char *pcEnd);

	/*!
	 * \brief	根据上string对象构造CSTringSlice对象
	 *
	 * \param	strOther
	 */
	CStringSlice(const std::string & strOther) ;	

	/*!
	 * \brief	以 string 类型返回 CStringSlice? 械哪谌?
	 *
	 * \return  string 字符串
	 */
	std::string ToStr();

	/*!
	 * \brief 消除首尾的 sDelimiter 字符
	 *
	 * \param	sDelimiter 要去除的字符
	 */
	void StrTrim(const char *sDelimiter);

	/*!
	 * \brief	得到字符串的头指针
	 *
	 * \return 字符串的头指针	
	 */
	inline const char *GetStart() const { return m_pcStart; }
	/*!
	 * \brief	得到字符串的尾指针
	 *
	 * \return	字符串的尾指针
	 */
	inline const char *GetEnd() const { return m_pcEnd; }

	/*!
	 * \brief	比较两个 CStringSlice 对象是否不等
	 *
	 * \param	strOther 另一个要比较的对象
	 *
	 * \retval true not equal
	 * \retval false  equal
	 */
	bool operator!=(const CStringSlice &strOther ) const; 

	/*!
	 * \brief	比较两个 CStringSlice 对象是否相等
	 *
	 * \param	strOther
	 *
	 * \retval true equal
	 * \retval false not equal	
	 */
	bool operator==(const CStringSlice &strOther ) const ; 

	/*!
	 * \brief	string 和 CStringSlice 对象是否相等
	 *
	 * \param	strOther 另外一个要比较的对象
	 *
	 * \retval true equal
	 * \retval false not equal	
	 */
	bool operator==(const std::string &strOther) const; 

	/*!
	 * \brief	比较两个CStringSlice的大小
	 *
	 * \param	strOther
	 *
	 * \retval	true 小于strOhter
	 * \retval  false 不小于strOther
	 */
	bool operator<(const CStringSlice &strOther) const ;
	
	private:
	const char *m_pcStart, *m_pcEnd;

};

/*!
 * \brief	the infomation unit to be used by  ConfigHashTable
 * \note	comprehend the usage of m_iNext ,as a back pointer to 
 * 			compose a dense storage hash table.
 * \as testConfigHashNode.cpp
 */
class ConfigHashNode {
	public:
	
	/*!
	 *\brief constructor 
	 *\note this constructor do nothing,it 
	 * does not initiallize any class members.
	 * so be careful to use it
	 * 
	 */
	ConfigHashNode(); 
	
	/*! constructor */
	ConfigHashNode(const unsigned int iHashnum, const CStringSlice & sec, const CStringSlice &key, const CStringSlice &val, int iNext);
	
	
	/*!
	 * \brief get m_iHashnum
	 *
	 * \return m_iHashnum	
	 * \as testConfigHashNode.cpp
	 */
	inline unsigned GetHashnum();
	
	/*!
	 * \brief 获取类成员m_iNext
	 *
	 * \return	m_iNext
	 * \as testConfigHashNode.cpp
	 */
	inline int GetNext();
	
	/*!
	 * \brief	获取类成员m_sec
	 *
	 * \return	m_sec
	 * \as testConfigHashNode.cpp
	 */
	inline CStringSlice & GetSec();
	
	/*!
	 * \brief	获取类成员m_key
	 *
	 * \return	m_key
	 * \as testConfigHashNode.cpp
	 */
	inline CStringSlice & GetKey();
	
	/*!
	 * \brief	获取类成员m_val
	 *
	 * \return	m_val
	 * \as testConfigHashNode.cpp
	 */
	inline CStringSlice & GetVal();

	private:

	unsigned int m_iHashnum;
	CStringSlice m_sec, m_key, m_val;
	int m_iNext;

};



/*!
 * \brief	A simple vector for ConfigHashTable, each unit is a ConfigHashNode.
 * 			the memory size is auto-increased by Add if necessary.
 * \note    only used for ConfigHashTable.
 * as testConfigFakeVector.cpp
 */
/* A simple Vector for ConfigHashTable */
class ConfigFakeVector {
	
public:
	
	/*!
	 * \brief constructor
	 */
	ConfigFakeVector();

	/*!
	 *\brief copy  constructor
	 */
	ConfigFakeVector(ConfigFakeVector & vOther);
	
	/*!
	 * \brief destructor
	 */
	~ConfigFakeVector();


	/*!
	 * \brief 
	 * \note do not free ConfigHashNode
	 */
	void Clear(); 

	inline unsigned int GetSize();

	void Add(ConfigHashNode node); 

	inline ConfigHashNode &operator[](int num) const;
private:

	ConfigHashNode * m_aNode;

	static const unsigned int STEP_SIZE = 128;

	unsigned int iSize, iMaxSize;
	
};


/*!
 * \brief	a dense storage hash table, using FNV1 hash function, CStringSlice section 
 * 			and CStringSlice key as	hash key, CStringSlice val as hash record.
 * \as	testIConfigHashConfigHashTable.cpp
 */
/* The Hash Table */
class ConfigHashTable {

public:

	ConfigHashTable();
	
	/*!
	 * \brief	construct ConfigHashTable, initialized HASH_SIZE according to iHashSize.
	 * 			HASH_SIZE = 2^n && HASH_SIZE >= iHashSize
	 * \note	bugfixed if ( iHashSize & (-iHashSize ) == iHashSize ) into 
	 * 					 if ( (iHashSize & (-iHashSize )) == iHashSize ), test whether iHashSize
	 * 			is 2^k. 
	 * \param	iHashSize int size to initialize HASH_SIZE.
	 */
	ConfigHashTable(int iHashSize);

	/*!
	 * \brief	constructor
	 * \param	other ConfigHashTable
	 */
	ConfigHashTable(ConfigHashTable &other);

	~ConfigHashTable();


	/*!
	 * \brief	allocate index array, initialized -1.
	 */
	void Init();

	/*!
	 * \brief	set each unit in index array to -1; release ConfigFakeVector
	 */
	void Clear();


	/*!
	 * \brief	add into ConfigHashTable
	 * \param	sec  CStringSlice, INI file section part
	 * \param	key	 CStringSlice, INI file key part
	 * \param	val  CStringSlice, INI file value part
	 */
	void Add(const CStringSlice &sec,const CStringSlice &key, const CStringSlice &val);
	
	/*!
	 * \brief	get the CStringSlice record specified by two parameters.
	 * \param	sec CStringSlice, INI file section part
	 * \param	key CStringSlice, INI file key part
	 * \return	CStringSlice, INI file value part if hit; else, null CStringSlice 
	 */
	CStringSlice Get(const CStringSlice &sec, const CStringSlice &key);

	/*!
	 * \brief	test whether ConfigHashTable is empty.
	 * \return true or false.
	 */
	bool Empty();

private:

	//using FNV1 hash function
	inline unsigned int HashFun(const CStringSlice &sec, const CStringSlice &key);


	int * m_aIndex;
	ConfigFakeVector m_table;

	//hash_size *must* be power of 2, ie: 2, 4, 8, 16, 32...
	static const unsigned int DEFAULT_HASH_SIZE = 1024;

	static unsigned int HASH_SIZE;
};



}
