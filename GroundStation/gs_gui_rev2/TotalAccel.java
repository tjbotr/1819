/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.digi.xbee.example;

import org.knowm.xchart.QuickChart;
import org.knowm.xchart.SwingWrapper;
import org.knowm.xchart.XYChart;

/**
 *
 * @author bala
 */
public class TotalAccel {
 
  SwingWrapper<XYChart> sw;
  XYChart chart;
 
 
   void go() {
 
    // Create Chart
    chart = QuickChart.getChart("Accelerometer Accleration vs Time Real Time Graph", "Time (seconds)", "Acceleration (m/s^2)", "randomWalk", new double[] { 0 }, new double[] { 0 });
    chart.getStyler().setLegendVisible(false);
    chart.getStyler().setXAxisTicksVisible(true);
 
    // Show it
    sw = new SwingWrapper<XYChart>(chart);
    sw.displayChart().setLocation(450, 600);
   //sw.displayChart();
 
  }
   
   void refresh( double[] mostRecentTotalAccelSet){
        chart.updateXYSeries("randomWalk", null, mostRecentTotalAccelSet, null);
        sw.repaintChart();
    }
 
}