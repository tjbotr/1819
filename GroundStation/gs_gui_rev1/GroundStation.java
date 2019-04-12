package com.digi.xbee.example;

//import com.digi.xbee.api.XBeeAddress;

import com.digi.xbee.api.*;
import com.digi.xbee.api.XBeeDevice;
import com.digi.xbee.api.models.XBeeMessage;
import com.digi.xbee.api.exceptions.XBeeException;
import javax.swing.JOptionPane;
import java.util.concurrent.TimeUnit;
import java.time.*;
import java.nio.*;
import java.awt.event.*;
import java.util.List;
import javax.swing.SwingWorker;




public class GroundStation extends SwingWorker
{
	static  String PORT = "COM5";

    
        static  int BAUD_RATE = 9600;
        
        static float packetCount = 0;
	static float altitude = 0;
	static float totalAccel = 0; //from accelerometer
	static float totalSpeed = 0; //from accelerometer
	static float pitotSpeed = 0; //from pitot tube
	static float latitude = 0;
	static float longitude = 0;
	static boolean cameraOn = false;
	static boolean cameraAllowedToBeEnabled = false;
	static int cameraDelay = 0;
        static int pitotAccel = 0;
        static String timeStamp = ""+Instant.now();
        static DisplayDataFrame displayUI = null;
        static GroundStationDriverUI mainFrame = null;
        static int armingAddress = 0x0;

        XBeeDevice myXBeeDevice = null;
           
        public  void setMainFrame(GroundStationDriverUI mainFrame) {
            GroundStation.mainFrame = mainFrame;
        }
        
        public  void setPORT(String PORT) {
            GroundStation.PORT = PORT;
        }

        public  void setBAUD_RATE(int BAUD_RATE) {
            GroundStation.BAUD_RATE = BAUD_RATE;
        }
        
	public void configure()
	{
		myXBeeDevice = new XBeeDevice(PORT, BAUD_RATE);
                		
		try 
		{
			myXBeeDevice.open();
                        System.out.println("Device opened");
                        
                        byte[] toSend = {1}; //0000000000000000
                        //RemoteXBeeDevice remoteDevice = new RemoteXbeeDevice(myXBeeDevice, new XBee16BitAddress("0x0"));
                        //myXBeeDevice.send_data(remoteDevice, toSend); //value to arm
		} 
		catch (XBeeException e) 
		{
			e.printStackTrace();
			//System.exit(1);
                        JOptionPane.showMessageDialog(displayUI, "Port can not be found"
        ,"Finding Port Error", JOptionPane.ERROR_MESSAGE);
		}
		XBeeMessage xbeeMessage = null;
		while (xbeeMessage == null) {
                        System.out.println("Data is being read");
                        System.out.println(xbeeMessage);
			xbeeMessage = myXBeeDevice.readData();
                }
                while (xbeeMessage!=null) {
                    System.out.println("Data is read");
                    //myXBeeDevice.close();
                    byte[] data = xbeeMessage.getData();
                    System.out.println(data);
                   // 
                    packetCount = new Byte(data[0]).floatValue();
                    altitude = new Byte(data[1]).floatValue();
                    totalAccel = data[2];
                    totalSpeed = data[3]; //Float.intBitsToFloatInteger.parseInt(data[3].toBinaryString())
                    pitotSpeed = data[4];
                    latitude = data[5];
                    longitude = data[6];
                    //cameraOn = data[7];
                    //cameraAllowedToBeEnabled = data[8];
                    cameraDelay = data[9];
                    pitotAccel = data[10];
                    //
                    
                    //processArray(data);
                    
                    timeStamp = ""+java.time.LocalTime.now();
                    System.out.println("packetCount "+packetCount);
                    System.out.println("altitude "+altitude);
                    System.out.println(totalAccel);
                    System.out.println(totalSpeed);
                    System.out.println(pitotSpeed);
                    System.out.println(latitude);
                    System.out.println(longitude);
                    //System.out.println(cameraOn);
                    //System.out.println(cameraAllowedToBeEnabled);
                    System.out.println(cameraDelay);
                    System.out.println(pitotAccel);
                    System.out.println(timeStamp);
                   
                    publish();
                    int delay = 10000; //milliseconds
                    ActionListener taskPerformer = new ActionListener() {
                        public void actionPerformed(ActionEvent evt) {
                            System.out.println("** BEfore calling In display UI **");
                            
                        }
                    };
                    javax.swing.Timer timer = new javax.swing.Timer(delay, taskPerformer);
                    timer.setRepeats(false);
                    timer.start();
                    System.out.println("** Timer started **");
                    
                    
                    xbeeMessage = myXBeeDevice.readData();
                    System.out.println("new data:" + xbeeMessage);
                    //delay
                    try
                    {
                        
                        Thread.sleep(1000);
                    }
                    catch(InterruptedException ex)
                    {
                        Thread.currentThread().interrupt();
                    }  
                    
                    
                }
                
	}
        
        static void displayUI() {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(GroundStationDriverUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(GroundStationDriverUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(GroundStationDriverUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(GroundStationDriverUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        
        //java.awt.EventQueue.invokeLater(new Runnable() {
       //     public void run() {
                System.out.println("** In display UI **");
                if (displayUI == null)
                    displayUI = new DisplayDataFrame();
                displayUI.pack();
                displayUI.setTimeStamp(timeStamp);
                displayUI.setAltitude(altitude);
                displayUI.setPacketCount(packetCount);
                displayUI.setTotalSpeed(totalSpeed);
                displayUI.setTotalAcc(totalAccel);
                displayUI.setPitotSpeed(pitotSpeed);
                displayUI.setPitotAcc(pitotAccel);
                displayUI.setPosition(latitude, longitude);
               
                
                //displayUI.setPort(PORT);
                //displayUI.setBaudRate(BAUD_RATE);
                
                displayUI.setVisible(true);
                displayUI.repaint();
               
       //     }
       // });
        
        
    }
    
    public static void processArray(byte[] array) {
	byte[] packetCountData = {array[0], array[1]};
	packetCount = ByteBuffer.wrap(packetCountData).getInt();
        System.out.println("packetCount "+packetCount);

	byte[] altmData = {array[2], array[3], array[4], array[5]};
	altitude = ByteBuffer.wrap(altmData).order(ByteOrder.BIG_ENDIAN).getFloat();
        System.out.println("altitude "+altitude);
        
	byte[] totalAccelData = {array[6], array[7], array[8], array[9]};
	totalAccel = ByteBuffer.wrap(totalAccelData).order(ByteOrder.BIG_ENDIAN).getFloat();
        System.out.println(totalAccel);

	byte[] pitotSpeedData = {array[10], array[11]};
	pitotSpeed = ByteBuffer.wrap(pitotSpeedData).getInt();
        System.out.println(pitotSpeed);

	byte[] latitudeData = {array[12], array[13], array[14], array[15]};
	latitude= ByteBuffer.wrap(latitudeData).order(ByteOrder.BIG_ENDIAN).getFloat();
        System.out.println(latitude);

	byte[] longitudeData = {array[16], array[17], array[18], array[19]};
	longitude  = ByteBuffer.wrap(longitudeData).order(ByteOrder.BIG_ENDIAN).getFloat();
        System.out.println(longitude);
    }

    @Override
    protected Object doInBackground() throws Exception {
        configure();
        return null;
        //throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    protected void done() {
        //super.done(); //To change body of generated methods, choose Tools | Templates.
        myXBeeDevice.close();
        System.out.println("done");
    }

    @Override
    protected void process(List chunks) {
        //super.process(chunks); //To change body of generated methods, choose Tools | Templates.
        displayUI();
    }

    
      
}