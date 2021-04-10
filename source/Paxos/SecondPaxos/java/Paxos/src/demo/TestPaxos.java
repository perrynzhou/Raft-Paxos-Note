package demo;

import java.util.Date;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.logging.Level;
import java.util.logging.Logger;

import paxos.Acceptor;
import paxos.ProposeData;
import paxos.Proposer;
import tool.IOFile;

public class TestPaxos implements Runnable{
	public TestPaxos(int id, Proposer proposer, Acceptor[] acceptors) {
        mId = id;
        mAcceptors = acceptors;
        mProposer = proposer;
	}
	
    //�����̣߳�ģ����Acceptorͨ��
    public void run() {
        ProposeData lastValue = new ProposeData();
        int[] acceptorId = new int[11];
        int count = 0;
        String fs = String.format("./Proposer%d.log", mId);
        IOFile logger = new IOFile(fs);
        Date dt = new Date();
        long startTime = dt.getTime();//��ʼPaxos��ʱ
        ProposeData value = null;
        while (true) {
            value = mProposer.getProposal();//�õ�����
            String logText = String.format("Proposer%d�ſ�ʼ(Propose�׶�):����=[���:%d������:%d]\r\n", 
            		mId, value.serialNum(), value.value());
            logger.write(logText);
            count = 0;
            int i = 0;
            for (i = 0; i < 11; i++) {
            	/*
            	 * ������Ϣ����i��acceptor
            	 * ����һ��ʱ��ﵽacceptor��sleep(���ʱ��)ģ��
            	 * acceptor������Ϣ��mAcceptors[i].Propose()
            	 * ��Ӧproposer
            	 * ����һ��ʱ��proposer�յ���Ӧ��sleep(���ʱ��)ģ��
            	 * proposer�����ӦmProposer.proposed(ok, lastValue)
            	 * fix��demo������������ʽʹ�ò���Ҫ����ʾ��λ�Ӧ��û�е���proposer�޸�����
            	 */
                sleep(rand(2000));//�������ʱ�䣬��Ϣ������mAcceptors[i]
                //������Ϣ
                TestPaxos.msLockAcceptors[mId].lock();
                boolean ok = mAcceptors[i].Propose(value.serialNum(), lastValue);
                TestPaxos.msLockAcceptors[mId].unlock();
                sleep(rand(2000));//�������ʱ��,��Ϣ����Proposer
                //����Propose��Ӧ
                if (!mProposer.proposed(ok, lastValue)) //���¿�ʼPropose�׶�
                {
                    sleep(1000);//Ϊ�˽��ͻ��������һ���ñ��proposer�л�������Լ���2�׶���׼
                    break;
                }
                ProposeData newValue = mProposer.getProposal();//�õ�����
                if (newValue.value() != value.value()) {//acceptor���λ�Ӧ�����Ƽ���һ������
                    logText = String.format("Proposer%d���޸�������:����=[���:%d������:%d]\r\n", 
                    		mId, newValue.serialNum(), newValue.value());
                    logger.write(logText);
                }
                acceptorId[count++] = i;//��¼Ը��ͶƱ��acceptor
                if (mProposer.startAccept()) {
                    if (0 == rand(100) % 2) 
                    {
                    	break;
                    }
                }
            }
        	//�����û�дﵽAccept��ʼ���������û�б�ʾҪ���¿�ʼPropose�׶�
            if (!mProposer.startAccept()) continue;

            //��ʼAccept�׶�
        	//����Accept��Ϣ������Ը��ͶƱ��acceptor
            value = mProposer.getProposal();//�õ�����
            logText = String.format("Proposer%d�ſ�ʼ(Accept�׶�):����=[���:%d������:%d]\r\n", mId, value.serialNum(), value.value());
            logger.write(logText);
            for (i = 0; i < count; i++) {
            	//����accept��Ϣ��acceptor
                sleep(rand(2000));//�������ʱ��,accept��Ϣ����acceptor
                //����accept��Ϣ
                TestPaxos.msLockAcceptors[mId].lock();
                boolean ok = mAcceptors[acceptorId[i]].Accept(value);
                TestPaxos.msLockAcceptors[mId].unlock();
                sleep(rand(2000));//�������ʱ��,accept��Ӧ����proposer
                //����accept��Ӧ
                if (!mProposer.accepted(ok)) //���¿�ʼPropose�׶�
                {
                    sleep(1000);//Ϊ�˽��ͻ��������һ���ñ��proposer�л�������Լ���2�׶���׼
                    break;
                }
                if (mProposer.isAgree()) {//�ɹ���׼������
                    dt = new Date();
                    long end = dt.getTime();
                    startTime = end - startTime;
                    logText = String.format("Proposer%d����׼������,��ʱ%dMS:�������� = [���:%d������:%d]\r\n", mId, 
                    		(int)startTime, value.serialNum(), value.value());
                    logger.write(logText);
                    Main.msLogger.info(logText);

                    msLockFinished.lock();
                    msFinishedCount += 1;
                    if (11 == msFinishedCount) {
                        msStartPaxos = dt.getTime() - msStartPaxos;
                        logText = String.format("Paxos��ɣ���ʱ%dMS\r\n", (int) msStartPaxos);
                        Main.msLogger.info(logText);
                    }
                    msLockFinished.unlock();
                    return;
                }
            }
        }
    }

    private void sleep(long ms) {
        try {
            Thread.sleep(ms);
        } catch (InterruptedException ex) {
            Logger.getLogger(TestPaxos.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    private int rand(int max){
        int r = (int) (Math.random() * max);
        return r;
    }

    private int mId;//������id
    private Proposer mProposer;//�̲߳�����proposer���
    private Acceptor[] mAcceptors;//����acceptor��㣬�û�ģ����acceptor������ֱ�ӵ���acceptor������ʾͨ�����
    public static Lock[] msLockAcceptors = null;
    public static long msStartPaxos;//��ʼʱ��
    public static int msFinishedCount;//��ɵ�proposer�������
    public static Lock msLockFinished = new ReentrantLock();//msFinishedCount��

}
