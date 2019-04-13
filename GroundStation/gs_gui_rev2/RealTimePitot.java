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
public class RealTimePitot {
 
  SwingWrapper<XYChart> sw;
  XYChart chart;
 
 
   void go() {
 
    // Create Chart
    chart = QuickChart.getChart("Pitot Tube Speed vs Time Real Time Graph", "Time (seconds)", "Speed (mph)", "randomWalk", new double[] { 0 }, new double[] { 0 });
    chart.getStyler().setLegendVisible(false);
    chart.getStyler().setXAxisTicksVisible(true);
 
    // Show it
    sw = new SwingWrapper<XYChart>(chart);
    sw.displayChart().setLocation(1000, 200);
   //sw.displayChart();
 
  }
   
   void refresh( double[] mostRecentPitotSet){
        chart.updateXYSeries("randomWalk", null, mostRecentPitotSet, null);
        sw.repaintChart();
    }
 
}

