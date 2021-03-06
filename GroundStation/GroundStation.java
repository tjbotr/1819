package com.digi.xbee.example;

//import com.digi.xbee.api.XBeeAddress;
import com.digi.xbee.api.XBeeDevice;
import com.digi.xbee.api.models.XBeeMessage;
import com.digi.xbee.api.exceptions.XBeeException;



public class GroundStation 
{
	private static final String PORT = "COM5";
	private static final int BAUD_RATE = 9600;
	
	public static void main(String[] args) 
	{
		XBeeDevice myXBeeDevice = new XBeeDevice(PORT, BAUD_RATE);
		// Read data.
		try 
		{
			myXBeeDevice.open();
		} 
		catch (XBeeException e) 
		{
			e.printStackTrace();
			System.exit(1);
		}
		XBeeMessage xbeeMessage = null;
		while (xbeeMessage == null)
			xbeeMessage = myXBeeDevice.readData();
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
	}
}
