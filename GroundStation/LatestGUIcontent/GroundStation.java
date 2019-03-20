package com.digi.xbee.example;

//import com.digi.xbee.api.XBeeAddress;
import com.digi.xbee.api.XBeeDevice;
import com.digi.xbee.api.models.XBeeMessage;
import com.digi.xbee.api.exceptions.XBeeException;
import javax.swing.JOptionPane;



public class GroundStation 
{
	private static final String PORT = "COM5";
	private static final int BAUD_RATE = 9600;
	
	public void configure(GroundStationDriverUI displayUI,String newPort, int rate)
	{
                System.out.println("newPort "+newPort);
                System.out.println("Rate "+rate);
		XBeeDevice myXBeeDevice = new XBeeDevice(newPort, rate);
		
		try 
		{
			myXBeeDevice.open();
                        System.out.println("Device opened");
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
			xbeeMessage = myXBeeDevice.readData();
                }
                System.out.println("Data is read");
		myXBeeDevice.close();
		byte[] data = xbeeMessage.getData();
		System.out.println(data);
		float pressure = data[0];
		float altitude = data[1];
		float temperature = data[2];
		float xPosition = data[3];
		float yPosition = data[4];
		float zPosition = data[5];
		float xAcceleration = data[6];
		float yAcceleration = data[7];
		float zAcceleration = data[8];
		float coordinates = data[9];
                System.out.println(pressure);
                System.out.println(altitude);
                System.out.println(temperature);
                System.out.println(xPosition);
                System.out.println(yPosition);
                System.out.println(zPosition);
                System.out.println(xAcceleration);
                System.out.println(yAcceleration);
                System.out.println(zAcceleration);
                System.out.println(coordinates);
                
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
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                DisplayDataFrame displayUI = new DisplayDataFrame();
                //displayUI.setPort(PORT);
                //displayUI.setBaudRate(BAUD_RATE);
                displayUI.setVisible(true);
            }
        });
    }
      
}