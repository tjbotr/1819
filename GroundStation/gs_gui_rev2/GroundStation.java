package com.digi.xbee.example;

//import com.digi.xbee.api.XBeeAddress;

import com.digi.xbee.api.*;
import com.digi.xbee.api.XBeeDevice;
import com.digi.xbee.api.models.XBeeMessage;
import com.digi.xbee.api.exceptions.XBeeException;
import com.digi.xbee.api.utils.HexUtils;
import javax.swing.JOptionPane;
import java.util.concurrent.TimeUnit;
import java.time.*;
import java.nio.*;
import java.awt.event.*;
import java.util.LinkedList;
import java.util.List;
import javax.swing.SwingWorker;




public class GroundStation extends SwingWorker
{
	static  String PORT = "COM5";

    
        static  int BAUD_RATE = 9600;
        
        static short packetCount = 0;
	static float altitude = 0;
	static float totalAccel = 0; //from accelerometer
	static float totalSpeed = 0; //from accelerometer
	static short pitotSpeed = 0; //from pitot tube
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
        
        static RealTimeAltitude altitudeRealTime = null;

        static RealTimePitot pitotSpeedRealTime = null;
        
        static TotalAccel totalAccelRealTime = null;
        
        static GpsPosition gpsPositionRealTime = null;
        
        List<Double> altitudeList = new LinkedList<Double>();
        List<Double> pitotSpeedList = new LinkedList<Double>();
        List<Double> totalAccelList = new LinkedList<Double>();
        List<Double> longitudeList = new LinkedList<Double>();
        List<Double> latitudeList = new LinkedList<Double>();

        XBeeDevice myXBeeDevice = null;
        
        public  void setPORT(String PORT) {
            GroundStation.PORT = PORT;
        }

        public  void setBAUD_RATE(int BAUD_RATE) {
            GroundStation.BAUD_RATE = BAUD_RATE;
        }
       
	public void configure()
	{
		myXBeeDevice = new XBeeDevice(PORT, BAUD_RATE);
                
                //Initialize Altitude
                altitudeList.add(0.0);
                
                //Initialize PitotAccel
                pitotSpeedList.add(0.0);
                
                 //Initialize Total Accel
                totalAccelList.add(0.0);
                
                //Initialize PitotAccel
                longitudeList.add(0.0);
                
                 //Initialize Total Accel
                latitudeList.add(0.0);
                		
		try 
		{
			myXBeeDevice.open();
                        System.out.println("Device opened");
                        
                        byte[] toSend = {1}; //0000000000000000
//                        //RemoteXBeeDevice remoteDevice = new RemoteXbeeDevice(myXBeeDevice, new XBee16BitAddress("0x0"));
//                        //myXBeeDevice.send_data(remoteDevice, toSend); //value to arm
                        String REMOTE_NODE_IDENTIFIER = "remoteXbee";
                        /**try {         
                            // Obtain the remote XBee device from the XBee network.
                            XBeeNetwork xbeeNetwork = myXBeeDevice.getNetwork();
                            RemoteXBeeDevice remoteDevice = xbeeNetwork.discoverDevice(REMOTE_NODE_IDENTIFIER);
                            if (remoteDevice == null) {
                                System.out.println("Couldn't find the remote XBee device with '" + REMOTE_NODE_IDENTIFIER + "' Node Identifier.");
                                System.exit(1);
                            }
                            System.out.format("Sending data to %s >> %s | %s... ", remoteDevice.get64BitAddress(), 
                            HexUtils.prettyHexString(HexUtils.byteArrayToHexString(toSend)), 
                            new String(toSend));
         
                            myXBeeDevice.sendData(remoteDevice, toSend);
         
                            System.out.println("Success");
         
                        } catch (XBeeException e) {
                            System.out.println("Error");
                            e.printStackTrace();
                            System.exit(1);
                        }**/
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
                   
                    processArray(data);
                    // 
                    /**packetCount = new Byte(data[0]).floatValue();
                    altitude = new Byte(data[1]).floatValue();**/
                    
                    //Add to the altitude list
                    altitudeList.add(new Float(altitude).doubleValue());
                    
                    /**totalAccel = data[2];
                    totalSpeed = data[3]; //Float.intBitsToFloatInteger.parseInt(data[3].toBinaryString())
                    pitotSpeed = data[4];**/
                    
                    //Add to the pitotSpeed list
                    pitotSpeedList.add(new Float(pitotSpeed).doubleValue());
                    
                    //Add to the totalAccel list
                    totalAccelList.add(new Float(totalAccel).doubleValue());
                    
                    //Add to the totalAccel list
                    longitudeList.add(new Float(longitude).doubleValue());
                    
                    //Add to the totalAccel list
                    latitudeList.add(new Float(latitude).doubleValue());
                    
                    /**latitude = data[5];
                    longitude = data[6];
                    //cameraOn = data[7];
                    //cameraAllowedToBeEnabled = data[8];
                    cameraDelay = data[9];
                    pitotAccel = data[10];**/
                    
                                        
                    timeStamp = ""+java.time.LocalTime.now();
                    /**System.out.println("packetCount "+packetCount);
                    System.out.println("altitude "+altitude);
                    System.out.println(totalAccel);
                    System.out.println(totalSpeed);
                    System.out.println(pitotSpeed);
                    System.out.println(latitude);
                    System.out.println(longitude);
                    //System.out.println(cameraOn);
                    //System.out.println(cameraAllowedToBeEnabled);
                    System.out.println(cameraDelay);
                    System.out.println(pitotAccel);**/
                    System.out.println(timeStamp);
                   
                    publish();
                    
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
        System.out.println("processing data from array"+array.length);
	byte[] packetCountData = {array[0], array[1]};
        System.out.println("one"+ array[0]+"two"+array[1]);
	packetCount = ByteBuffer.wrap(packetCountData).order(ByteOrder.BIG_ENDIAN).getShort();
        System.out.println("packetCount "+packetCount);

	byte[] altmData = {array[2], array[3], array[4], array[5]};
	altitude = ByteBuffer.wrap(altmData).order(ByteOrder.BIG_ENDIAN).getFloat();
        System.out.println("altitude "+altitude);
        
	byte[] totalAccelData = {array[6], array[7], array[8], array[9]};
	totalAccel = ByteBuffer.wrap(totalAccelData).order(ByteOrder.BIG_ENDIAN).getFloat();
        System.out.println(totalAccel);

	byte[] pitotSpeedData = {array[10], array[11]};
	pitotSpeed = ByteBuffer.wrap(pitotSpeedData).order(ByteOrder.BIG_ENDIAN).getShort();
        System.out.println(pitotSpeed);

	byte[] latitudeData = {array[12], array[13], array[14], array[15]};
	latitude= ByteBuffer.wrap(latitudeData).order(ByteOrder.BIG_ENDIAN).getFloat();
        System.out.println(latitude);

	byte[] longitudeData = {array[16], array[17], array[18], array[19]};
	longitude  = ByteBuffer.wrap(longitudeData).order(ByteOrder.BIG_ENDIAN).getFloat();
        System.out.println(longitude);
        System.out.println("jeffter");
    }

    @Override
    protected Object doInBackground() throws Exception {
        altitudeRealTime = new RealTimeAltitude();
        
        altitudeRealTime.go();
        
        pitotSpeedRealTime = new RealTimePitot();
        
        pitotSpeedRealTime.go();
        
        totalAccelRealTime = new TotalAccel();
        
        totalAccelRealTime.go();
        
        gpsPositionRealTime = new GpsPosition();
                
        gpsPositionRealTime.go();
        
        configure();
        //Altitude Frame
        
        return null;
        //throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    protected void done() {
        //super.done(); //To change body of generated methods, choose Tools | Templates.
        myXBeeDevice.close();
        System.out.println("overall done");
    }

    @Override
    protected void process(List chunks) {
        //super.process(chunks); //To change body of generated methods, choose Tools | Templates.
        displayUI();
        
        //Display Altitude Chart
        System.out.println("** Displaying Altitude");
        double[] mostRecentAltitudeSet = new double[altitudeList.size()];
        for (int i = 0; i < altitudeList.size(); i++) {
          mostRecentAltitudeSet[i] = altitudeList.get(i);
        }
 
        altitudeRealTime.refresh(mostRecentAltitudeSet);   
        
        //Display Pitot Speed Chart
        System.out.println("** Displaying Pitot Speed");
        double[] mostRecentPitotSpeedSet = new double[pitotSpeedList.size()];
        for (int i = 0; i < pitotSpeedList.size(); i++) {
          mostRecentPitotSpeedSet[i] = this.pitotSpeedList.get(i);
        }
 
        pitotSpeedRealTime.refresh(mostRecentPitotSpeedSet);  
        
        //Display Total Accel Chart
        System.out.println("** Displaying Accelerometer Acceleration");
        double[] mostRecentTotalAccelSet = new double[totalAccelList.size()];
        for (int i = 0; i < totalAccelList.size(); i++) {
          mostRecentTotalAccelSet[i] = this.totalAccelList.get(i);
        }
 
        totalAccelRealTime.refresh(mostRecentTotalAccelSet); 
        
        //Display Longitude Chart
        System.out.println("** Displaying Longitude and Latitude");
        double[] mostRecentLongitudeSet = new double[longitudeList.size()];
        for (int i = 0; i < longitudeList.size(); i++) {
          mostRecentLongitudeSet[i] = this.longitudeList.get(i);
        }
         
        //Display Latitude Chart
        double[] mostRecentLatitudeSet = new double[latitudeList.size()];
        for (int i = 0; i < latitudeList.size(); i++) {
          mostRecentLatitudeSet[i] = this.latitudeList.get(i);
        }
        
        gpsPositionRealTime.refresh(mostRecentLongitudeSet, mostRecentLatitudeSet);

    }

    
      
}