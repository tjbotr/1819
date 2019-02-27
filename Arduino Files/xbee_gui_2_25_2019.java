package com.digi.xbee.example;

import java.util.*;
import java.awt.*;
import javax.swing.*;
import com.digi.xbee.api.*;

public class xbee_gui_2_25_2019 {
   AbstractXBeeDevice xbee;
   
   public static void main(String[] args) {
      //String portString = "";
      
      JFrame overallFrame = new JFrame("Groundstation");
      overallFrame.setSize(1100, 700);
      
      JPanel instPanel = new JPanel();
      instPanel.setVisible(true);
      instPanel.setSize(750, 700);
      
      String textFieldText = "Connect to an XBee";
      JTextField textFieldText = new JTextField(textFieldText);
      instPanel.add(textFieldText);
      overallFrame.add(instPanel);
      
      ActionListener connectXbee =
               new ActionListener() {
                  @Override
                  public void actionPerformed(ActionEvent e) {
                     xbee = new AbstractXBeeDevice(textFieldText.paramString(), 9600);
                  }
               };
               
      textFieldText.addActionListener(connectXbee);
      
      overallFrame.setVisible(true);
   }
}