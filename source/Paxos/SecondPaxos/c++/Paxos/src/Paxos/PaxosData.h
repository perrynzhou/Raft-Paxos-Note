#ifndef PAXOS_DATA_H
#define PAXOS_DATA_H

namespace paxos
{
	//�������ݽṹ
	typedef struct PROPOSAL
	{
		unsigned int	serialNum;//��ˮ��,1��ʼ��������֤ȫ��Ψһ
		unsigned int	value;//��������
	}PROPOSAL;
}

#endif //PAXOS_DATA_H