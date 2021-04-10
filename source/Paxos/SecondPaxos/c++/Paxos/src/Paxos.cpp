// speak.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "Paxos/Acceptor.h"
#include "Paxos/Proposer.h"
#include "lib/Thread.h"
#include "lib/Lock.h"
#include "lib/mapi.h"
#include "lib/atom.h"
#include "lib/Logger.h"

paxos::Proposer p[11];
paxos::Acceptor a[11];
mdk::Mutex l[11];
int finishedCount = 0;
mdk::uint64 g_start;
mdk::Logger g_log;

void* Proposer(void *id)
{
	mdk::Logger log;
	char logName[256];
	sprintf( logName, "Proposer%d", (int)id );
	log.SetLogName(logName);
	log.SetMaxLogSize(10);
	log.SetMaxExistDay(30);
	log.SetPrintLog(false);

	paxos::Proposer &proposer = p[(int)id];
	paxos::PROPOSAL value = proposer.GetProposal();
	paxos::PROPOSAL lastValue;


	int acceptorId[11];
	int count = 0;

	mdk::uint64 start = mdk::MillTime();
	while ( true )
	{
		value = proposer.GetProposal();//�õ�����
		log.Info("Info", "Proposer%d�ſ�ʼ(Propose�׶�):����=[���:%d������:%d]\n", 
			(int)id, value.serialNum, value.value);
		count = 0;
		int i = 0;
		for (i = 0; i < 11; i++ )
		{
			/*
				* ������Ϣ����i��acceptor
				* ����һ��ʱ��ﵽacceptor��sleep(���ʱ��)ģ��
				* acceptor������Ϣ��mAcceptors[i].Propose()
				* ��Ӧproposer
				* ����һ��ʱ��proposer�յ���Ӧ��sleep(���ʱ��)ģ��
				* proposer�����ӦmProposer.proposed(ok, lastValue)
			*/
            mdk::m_sleep(rand()%2000);//�������ʱ�䣬��Ϣ������mAcceptors[i]
			//������Ϣ
			l[i].Lock();
			bool ok = a[i].Propose(value.serialNum, lastValue);
			l[i].Unlock();
			mdk::m_sleep(rand()%2000);//�������ʱ��,��Ϣ����Proposer
			//����Propose��Ӧ
			if ( !proposer.Proposed(ok, lastValue) ) //���¿�ʼPropose�׶�
			{
				mdk::m_sleep(1000);//Ϊ�˽��ͻ��������һ���ñ��proposer�л�������Լ���2�׶���׼
				break;
			}
			paxos::PROPOSAL curValue = proposer.GetProposal();//�õ�����
			if ( curValue.value != value.value )//acceptor���λ�Ӧ�����Ƽ���һ������
			{
				log.Info("Info", "Proposer%d���޸�������:����=[���:%d������:%d]\n", 
					(int)id, curValue.serialNum, curValue.value);
			}
			acceptorId[count++] = i;//��¼Ը��ͶƱ��acceptor
			if ( proposer.StartAccept() )
			{
				if ( 0 == rand() % 2 ) break;
			}
		}
		//�����û�дﵽAccept��ʼ���������û�б�ʾҪ���¿�ʼPropose�׶�
		if ( !proposer.StartAccept() ) continue;

		//��ʼAccept�׶�
		//����Accept��Ϣ������Ը��ͶƱ��acceptor
		value = proposer.GetProposal();
		log.Info("Info", "Proposer%d�ſ�ʼ(Accept�׶�):����=[���:%d������:%d]\n", 
			(int)id, value.serialNum, value.value);
		for (i = 0; i < count; i++ )
		{
			//����accept��Ϣ��acceptor
			mdk::m_sleep(rand()%2000);//�������ʱ��,accept��Ϣ����acceptor
			//����accept��Ϣ
			l[acceptorId[i]].Lock();
			bool ok = a[acceptorId[i]].Accept(value);
			l[acceptorId[i]].Unlock();
			mdk::m_sleep(rand()%2000);//�������ʱ��,accept��Ӧ����proposer
			//����accept��Ӧ
			if ( !proposer.Accepted(ok) ) //���¿�ʼPropose�׶�
			{
				mdk::m_sleep(1000);//Ϊ�˽��ͻ��������һ���ñ��proposer�л�������Լ���2�׶���׼
				break;
			}
			if ( proposer.IsAgree() )//�ɹ���׼������
			{
				start = mdk::MillTime() - start;
				log.Info("Info", "Proposer%d����׼������,��ʱ%lluMS:�������� = [���:%d������:%d]\n", 
					(int)id, start, value.serialNum, value.value);
				g_log.Info("Info", "Proposer%d����׼������,��ʱ%lluMS:�������� = [���:%d������:%d]\n", 
					(int)id, start, value.serialNum, value.value);
				if ( 10 == mdk::AtomAdd(&finishedCount, 1) )
				{
					g_start = mdk::MillTime() - g_start;
					g_log.Info("Info", "Paxos��ɣ���ʱ%lluMS\n", g_start);
				}
				return NULL;
			}
		}
	}
	return NULL;
}

//Paxos����ģ����ʾ����
int main(int argc, char* argv[])
{
	int i = 0;
	g_log.SetLogName("Paxos");
	g_log.SetMaxLogSize(10);
	g_log.SetMaxExistDay(30);
	g_log.SetPrintLog(true);
	g_log.Info("Info", "11��Proposer, 11��Acceptor׼������Paxos\n"
		"ÿ��Proposer�����̣߳�Acceptor����Ҫ�߳�\n"
		"Proposer�߳��еȴ����ʱ��:��ʾ��Acceptor��ͨ��ʱ��\n"
		"Proposer�߳��е���Acceptor.Proposed()��ʾ�õ���Propose������\n"
		"Proposer�߳��е���Acceptor.Accepted()��ʾ�õ���Accept������\n"
		"Proposer����׼������߳�,�����̼߳���ͶƱ���գ�ȫ����׼��ͬ��ֵ�����һ�¡�\n");
	g_start = mdk::MillTime();
	g_log.Info("Info", "Paxos��ʼ\n" );
	paxos::PROPOSAL value;

	for ( i = 0; i < 11; i++ ) 
	{
		p[i].SetPlayerCount(11, 11);
		value.serialNum = value.value = i + 1;
		p[i].StartPropose(value);
	}

	mdk::Thread t[11];
	for ( i = 0; i < 11; i++ ) t[i].Run(Proposer, (void*)i);
	for ( i = 0; i < 11; i++ ) t[i].WaitStop();
	while ( true ) mdk::m_sleep(1000);
	return 0;
}

