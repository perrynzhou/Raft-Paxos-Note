#ifndef PROPOSER_H
#define PROPOSER_H

#include <ctime>
#include "PaxosData.h"

namespace paxos
{

//������
class Proposer
{
public:
	Proposer();
	Proposer(short proposerCount, short acceptorCount);
	virtual ~Proposer();
	//���ò���������
	void SetPlayerCount(short proposerCount, short acceptorCount);
	//��ʼPropose�׶�
	void StartPropose(PROPOSAL &value);
	//ȡ������
	PROPOSAL& GetProposal();
	//���鱻ͶƱ��Proposedʧ�������¿�ʼPropose�׶�
	bool Proposed(bool ok, PROPOSAL &lastAcceptValue);
	//��ʼAccept�׶�,���������ɹ���ʼaccept�׶η���ture�������㿪ʼ��������false
	bool StartAccept();
	//���鱻���ܣ�Acceptedʧ�������¿�ʼPropose�׶�
	bool Accepted(bool ok);
	//���鱻��׼
	bool IsAgree();

private:
	short			m_proposerCount;///proposer����
	short			m_acceptorCount;//acceptor����
	PROPOSAL		m_value;//Ԥ������
	bool			m_proposeFinished;//�����Ʊ��׼����ʼ���׶�
	bool			m_isAgree;//m_value����׼
	unsigned int	m_maxAcceptedSerialNum;//�ѱ����ܵ���������ˮ������
	time_t			m_start;//�׶ο�ʼʱ�䣬�׶�һ���׶ζ�����
	short			m_okCount;//ͶƱ�������׶�һ���׶ζ�����
	short			m_refuseCount;//�ܾ��������׶�һ���׶ζ�����
};

}

#endif //PROPOSER_H