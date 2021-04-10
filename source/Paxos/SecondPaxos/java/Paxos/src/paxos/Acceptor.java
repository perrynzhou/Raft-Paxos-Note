/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package paxos;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 *
 * @author Administrator
 * Acceptor�ࣺ��Ӧһ�׶����飬���׶�����
 */
public class Acceptor {

    public Acceptor() {
        mMaxSerialNum = 0;
        mLastAcceptValue = new ProposeData();
        mLastAcceptValue.setSerialNum(0);
        mLastAcceptValue.setValue(0);
    }

    //ͬ��/�ܾ��½׶λ��������
    //ͬ��ʱ����ŵ����ͬ����С��mMaxSerialNum�����飬Ҳ���ٽ��ܱ��С��mMaxSerialNum������
    public boolean Propose(int serialNum, ProposeData lastAcceptValue) {
        mLock.lock();
        if (0 >= serialNum) {
            mLock.unlock();
            return false;
        }
        if (mMaxSerialNum > serialNum) {
            mLock.unlock();
            return false;
        }
        mMaxSerialNum = serialNum;
        lastAcceptValue.setSerialNum(mLastAcceptValue.serialNum());
        lastAcceptValue.setValue(mLastAcceptValue.value());
        mLock.unlock();

        return true;
    }

    //����/�ܾ�����
    //ֻ���ܱ��>=mMaxSerialNum�����飬����¼
    public boolean Accept(ProposeData value) {
        mLock.lock();
        if (0 >= value.serialNum()) {
            mLock.unlock();
            return false;
        }
        if (mMaxSerialNum > value.serialNum()) {
            mLock.unlock();
            return false;
        }
        mLastAcceptValue.setSerialNum(value.serialNum());
        mLastAcceptValue.setValue(value.value());
        mLock.unlock();
        return true;
    }
    private ProposeData mLastAcceptValue;//�����ܵ�����
    private int mMaxSerialNum;//Propose�ύ�������ˮ��
    private Lock mLock = new ReentrantLock();
}
