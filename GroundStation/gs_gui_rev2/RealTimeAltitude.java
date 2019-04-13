/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.digi.xbee.example;

/**
 *
 * @author bala
 */
import java.util.LinkedList;
import java.util.List;
 
import javax.swing.SwingWorker;
 
import org.knowm.xchart.QuickChart;
import org.knowm.xchart.SwingWrapper;
import org.knowm.xchart.XYChart;
 
/**
 * Creates a real-time chart using SwingWorker
 */
public class RealTimeAltitude {
 
  SwingWrapper<XYChart> sw;
  XYChart chart;
 
 
   void go() {
 
    // Create Chart
    chart = QuickChart.getChart("Altitude vs Time Real Time Graph", "Time", "Altitude", "randomWalk", new double[] { 0 }, new double[] { 0 });
    chart.getStyler().setLegendVisible(false);
    chart.getStyler().setXAxisTicksVisible(true);
 
    // Show it
    sw = new SwingWrapper<XYChart>(chart);
    sw.displayChart().setLocation(450, 200);
 
  }
   
   void refresh( double[] mostRecentAltitudeSet){
        //System
        chart.updateXYSeries("randomWalk", null, mostRecentAltitudeSet, null);
        sw.repaintChart();
    }
 
}

