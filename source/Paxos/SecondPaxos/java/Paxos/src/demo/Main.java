/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package demo;

import java.io.IOException;
import java.util.Date;
import java.util.concurrent.locks.ReentrantLock;
import java.util.logging.FileHandler;
import java.util.logging.Level;
import java.util.logging.Logger;

import paxos.Acceptor;
import paxos.ProposeData;
import paxos.Proposer;
import tool.LogFormatter;
/**
 *
 * @author Administrator
 */
public class Main {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        msLogger = Logger.getLogger(Main.class.getName());
        try {
            String fs = String.format("./Main.log");
            FileHandler fileHandler = new FileHandler(fs);
            msLogger.addHandler(fileHandler);
            fileHandler.setFormatter(new LogFormatter());
        } catch (IOException ex) {
            Logger.getLogger(Proposer.class.getName()).log(Level.SEVERE, null, ex);
        } catch (SecurityException ex) {
            Logger.getLogger(Proposer.class.getName()).log(Level.SEVERE, null, ex);
        }
        String logText = "11��Proposer, 11��Acceptor׼������Paxos\r\n"
                + "ÿ��Proposer�����̣߳�Acceptor����Ҫ�߳�\r\n"
		+ "Proposer�߳��еȴ����ʱ��:��ʾ��Acceptor��ͨ��ʱ��\r\n"
		+ "Proposer�߳��е���Acceptor.Proposed()��ʾ�õ���Propose������\r\n"
		+ "Proposer�߳��е���Acceptor.Accepted()��ʾ�õ���Accept������\r\n"
		+ "Proposer����׼������߳�,�����̼߳���ͶƱ���գ�ȫ����׼��ͬ��ֵ�����һ�¡�\r\n\r\n";
        msLogger.info( logText );
        msLogger.info( "Paxos��ʼ" );


        Proposer []p = new Proposer[11];
        Acceptor []a = new Acceptor[11];
        int i = 0;
        for ( i = 0; i < 11; i++ ) 
        {
            a[i] = new Acceptor();
        }
        
        TestPaxos.msLockAcceptors = new ReentrantLock[11];
        for ( i = 0; i < 11; i++ )
        {
        	TestPaxos.msLockAcceptors[i] = new ReentrantLock();
        }
        ProposeData value;
        Thread []t = new Thread[11];
        Date dt= new Date();
        TestPaxos.msStartPaxos = dt.getTime();
        TestPaxos []proposerRun = new TestPaxos[11];
        for ( i = 0; i < 11; i++ )
        {
            p[i] = new Proposer((short)i, (short)i);
            p[i].setPlayerCount((short)11, (short)11);
            value = new ProposeData();
            value.setSerialNum(i);
            value.setValue(i);
            p[i].startPropose(value);
            proposerRun[i] = new TestPaxos(i, p[i], a);
            t[i] = new Thread(proposerRun[i]);
            t[i].start();
        }
        while ( true ) try {
            Thread.sleep(1);
        } catch (InterruptedException ex) {
            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    public static Logger msLogger;

}
