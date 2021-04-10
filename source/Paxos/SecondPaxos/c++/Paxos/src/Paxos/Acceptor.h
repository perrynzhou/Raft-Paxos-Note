#ifndef ACCTPTOR_H
#define ACCTPTOR_H

#include "PaxosData.h"
namespace paxos
{

//ͶƱ��
class Acceptor
{
public:
	Acceptor(void);
	virtual ~Acceptor(void);

	//ͬ��ͶƱ
	bool Propose(unsigned int serialNum, PROPOSAL &lastAcceptValue);
	//��������
	bool Accept(PROPOSAL &value);

private:
	PROPOSAL		m_lastAcceptValue;//�����ܵ�����
	unsigned int	m_maxSerialNum;//Propose�ύ�������ˮ��
};

}

#endif //ACCTPTOR_H