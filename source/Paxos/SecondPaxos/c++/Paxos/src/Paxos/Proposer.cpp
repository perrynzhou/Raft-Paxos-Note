#include "Proposer.h"

namespace paxos
{

Proposer::Proposer()
{
	SetPlayerCount(0, 0);
}

Proposer::Proposer(short proposerCount, short acceptorCount)
{
	SetPlayerCount(proposerCount, acceptorCount);
}

Proposer::~Proposer()
{
}

void Proposer::SetPlayerCount(short proposerCount, short acceptorCount)
{
	m_proposerCount = proposerCount;
	m_acceptorCount = acceptorCount;

	return;
}

void Proposer::StartPropose(PROPOSAL &value)
{
	m_value = value;
	m_proposeFinished = false;
	m_isAgree = false;
	m_maxAcceptedSerialNum = 0;
	m_okCount = 0;
	m_refuseCount = 0;
	m_start = time(NULL);

	return;
}

PROPOSAL& Proposer::GetProposal()
{
	return m_value;
}

bool Proposer::Proposed(bool ok, PROPOSAL &lastAcceptValue)
{
	if ( m_proposeFinished ) return true;//������һ�׶γٵ��Ļ�Ӧ��ֱ�Ӻ�����Ϣ

	if ( !ok ) 
	{
		m_refuseCount++;
		//���а����ܾ�������Ҫ�ȴ�����acceptorͶƱ�ˣ����¿�ʼPropose�׶�
		if ( m_refuseCount > m_acceptorCount / 2 ) 
		{
			m_value.serialNum += m_proposerCount;
			StartPropose(m_value);
			return false;
		}
		return true;
	}

	m_okCount++;
	/*
		û�б�Ҫ����֧��serialNumΪnull
		��ΪserialNum>m_maxAcceptedSerialNum����serialNum��0��Ϊ��Ҫ����
	*/
	//��¼�����յ��������У�����������飬���Լ��������Ȩʱ�����
	if ( lastAcceptValue.serialNum > m_maxAcceptedSerialNum ) 
	{
		m_maxAcceptedSerialNum = lastAcceptValue.serialNum;
		m_value.value = lastAcceptValue.value;
	}
	if ( m_okCount > m_acceptorCount / 2 ) 
	{
		m_okCount = 0;
		m_proposeFinished = true;
	}
	return true;
}

bool Proposer::StartAccept()
{
	return m_proposeFinished;
}

bool Proposer::Accepted(bool ok)
{
	if ( !m_proposeFinished ) return true;//�������ϴεڶ��׶γٵ��Ļ�Ӧ��ֱ�Ӻ�����Ϣ

	if ( !ok ) 
	{
		m_refuseCount++;
		//���а����ܾ�������Ҫ�ȴ�����acceptorͶƱ�ˣ����¿�ʼPropose�׶�
		if ( m_refuseCount > m_acceptorCount / 2 ) 
		{
			m_value.serialNum += m_proposerCount;
			StartPropose(m_value);
			return false;
		}
		return true;
	}

	m_okCount++;
	if ( m_okCount > m_acceptorCount / 2 ) m_isAgree = true;

	return true;
}

bool Proposer::IsAgree()
{
	return m_isAgree;
}

}
