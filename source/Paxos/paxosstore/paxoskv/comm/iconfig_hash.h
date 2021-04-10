
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
	 * \brief	constructor ��ʼ��һ�����ַ���
	 */
	CStringSlice(); 

	/*!
	 * \brief	����һ���ַ�������βָ�빹�����
	 *
	 * \param	pcStart �ַ�����ͷָ��
	 * \param	pcEnd �ַ�����βָ��
	 */
	CStringSlice(const char *pcStart, const char *pcEnd);

	/*!
	 * \brief	������string������CSTringSlice����
	 *
	 * \param	strOther
	 */
	CStringSlice(const std::string & strOther) ;	

	/*!
	 * \brief	�� string ���ͷ��� CStringSlice� е�����
	 *
	 * \return  string �ַ���
	 */
	std::string ToStr();

	/*!
	 * \brief ������β�� sDelimiter �ַ�
	 *
	 * \param	sDelimiter Ҫȥ�����ַ�
	 */
	void StrTrim(const char *sDelimiter);

	/*!
	 * \brief	�õ��ַ�����ͷָ��
	 *
	 * \return �ַ�����ͷָ��	
	 */
	inline const char *GetStart() const { return m_pcStart; }
	/*!
	 * \brief	�õ��ַ�����βָ��
	 *
	 * \return	�ַ�����βָ��
	 */
	inline const char *GetEnd() const { return m_pcEnd; }

	/*!
	 * \brief	�Ƚ����� CStringSlice �����Ƿ񲻵�
	 *
	 * \param	strOther ��һ��Ҫ�ȽϵĶ���
	 *
	 * \retval true not equal
	 * \retval false  equal
	 */
	bool operator!=(const CStringSlice &strOther ) const; 

	/*!
	 * \brief	�Ƚ����� CStringSlice �����Ƿ����
	 *
	 * \param	strOther
	 *
	 * \retval true equal
	 * \retval false not equal	
	 */
	bool operator==(const CStringSlice &strOther ) const ; 

	/*!
	 * \brief	string �� CStringSlice �����Ƿ����
	 *
	 * \param	strOther ����һ��Ҫ�ȽϵĶ���
	 *
	 * \retval true equal
	 * \retval false not equal	
	 */
	bool operator==(const std::string &strOther) const; 

	/*!
	 * \brief	�Ƚ�����CStringSlice�Ĵ�С
	 *
	 * \param	strOther
	 *
	 * \retval	true С��strOhter
	 * \retval  false ��С��strOther
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
	 * \brief ��ȡ���Աm_iNext
	 *
	 * \return	m_iNext
	 * \as testConfigHashNode.cpp
	 */
	inline int GetNext();
	
	/*!
	 * \brief	��ȡ���Աm_sec
	 *
	 * \return	m_sec
	 * \as testConfigHashNode.cpp
	 */
	inline CStringSlice & GetSec();
	
	/*!
	 * \brief	��ȡ���Աm_key
	 *
	 * \return	m_key
	 * \as testConfigHashNode.cpp
	 */
	inline CStringSlice & GetKey();
	
	/*!
	 * \brief	��ȡ���Աm_val
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
