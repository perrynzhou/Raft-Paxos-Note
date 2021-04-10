/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package paxos;

import java.util.Date;

/**	
 *
 * @author Administrator
 */
public class Proposer {

    public Proposer() {
        setPlayerCount((short) 0, (short) 0);
        mValue = new ProposeData();
    }

    public Proposer(short proposerCount, short acceptorCount) {
        setPlayerCount(proposerCount, acceptorCount);
        mValue = new ProposeData();
    }

    //���ò���������
    public void setPlayerCount(short proposerCount, short acceptorCount) {
        mProposerCount = proposerCount;///proposer����
        mAcceptorCount = acceptorCount;//acceptor����
    }

    //��ʼPropose�׶�
    public void startPropose(ProposeData value) {
        mValue.setSerialNum(value.serialNum());
        mValue.setValue(value.value());
        mProposeFinished = false;
        mIsAgree = false;
        mMaxAcceptedSerialNum = 0;
        mOkCount = 0;
        mRefuseCount = 0;
		Date curTime = new Date();
        mStart = curTime.getTime();//����Ǿ���1970��1��1��0��0��0��ĺ�����
    }

    /*
     * �׶γ�ʱ
     * millSecond����ʱ�ж�ʱ��
     */
    public boolean isTimeOut( int millSecond )
    {
		Date curTime = new Date();
        int waitTime = (int)(curTime.getTime() - mStart);//����Ǿ���1970��1��1��0��0��0��ĺ�����
        if ( waitTime > millSecond ) return true;
        
        return false;
    }
    
    //ȡ������
    public ProposeData getProposal() {
        return mValue;
    }

    //���鱻ͶƱ��Proposedʧ�������¿�ʼPropose�׶�
    public boolean proposed(boolean ok, ProposeData lastAcceptValue) {
        if (mProposeFinished) {
            return true;//������һ�׶γٵ��Ļ�Ӧ��ֱ�Ӻ�����Ϣ
        }
        if (!ok) {
            mRefuseCount++;
            //���а����ܾ�������Ҫ�ȴ�����acceptorͶƱ�ˣ����¿�ʼPropose�׶�
            if (mRefuseCount > mAcceptorCount / 2) {
                mValue.setSerialNum(mValue.serialNum() + mProposerCount);
                startPropose(mValue);
                return false;
            }
            return true;
        }

        mOkCount++;
        /*
		        û�б�Ҫ����֧��serialNumΪnull
		        ��ΪserialNum>m_maxAcceptedSerialNum����serialNum��0��Ϊ��Ҫ����
         */
        //��¼�����յ��������У�����������飬���Լ��������Ȩʱ�����
        if (lastAcceptValue.serialNum() > mMaxAcceptedSerialNum) {
            mMaxAcceptedSerialNum = lastAcceptValue.serialNum();
            if (mValue.value() != lastAcceptValue.value()) {
                mValue.setValue(lastAcceptValue.value());
            }
        }
        if (mOkCount > mAcceptorCount / 2) {
            mOkCount = 0;
            mProposeFinished = true;
        }
        return true;
    }

    //��ʼAccept�׶�,���������ɹ���ʼaccept�׶η���ture�������㿪ʼ��������false
    public boolean startAccept() {
        return mProposeFinished;
    }

    //���鱻���ܣ�Acceptedʧ�������¿�ʼPropose�׶�
    public boolean accepted(boolean ok) {
        if (!mProposeFinished) {
            return true;//�������ϴεڶ��׶γٵ��Ļ�Ӧ��ֱ�Ӻ�����Ϣ
        }
        if (!ok) {
            mRefuseCount++;
            //���а����ܾ�������Ҫ�ȴ�����acceptorͶƱ�ˣ����¿�ʼPropose�׶�
            if (mRefuseCount > mAcceptorCount / 2) {
                mValue.setSerialNum(mValue.serialNum() + mProposerCount);
                startPropose(mValue);
                return false;
            }
            return true;
        }

        mOkCount++;
        if (mOkCount > mAcceptorCount / 2) {
            mIsAgree = true;
        }

        return true;
    }

    //���鱻��׼
    public boolean isAgree() {
        return mIsAgree;
    }

    private short mProposerCount;///proposer����
    private short mAcceptorCount;//acceptor����
    private ProposeData mValue;//��������
    private boolean mProposeFinished;//�����Ʊ��׼����ʼ���׶�
    private boolean mIsAgree;//m_value����׼
    private int mMaxAcceptedSerialNum;//�ѱ����ܵ���������ˮ������
    private long mStart;//�׶ο�ʼʱ�䣬�׶�һ���׶ζ�����
    private short mOkCount;//ͶƱ�������׶�һ���׶ζ�����
    private short mRefuseCount;//�ܾ��������׶�һ���׶ζ�����
}
