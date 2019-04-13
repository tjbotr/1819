/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.digi.xbee.example;

import org.knowm.xchart.QuickChart;
import org.knowm.xchart.SwingWrapper;
import org.knowm.xchart.XYChart;
import org.knowm.xchart.XYSeries.XYSeriesRenderStyle;
import org.knowm.xchart.style.Styler;

/**
 *
 * @author bala
 */
public class GpsPosition {
    SwingWrapper<XYChart> sw;
    XYChart chart;
 
 
   void go() {
 
    // Create Chart
    chart = QuickChart.getChart("GPS Position vs Time Real Time Graph", "Time (seconds)", "Position", "GPS Legend", new double[] { 0 }, new double[] { 0 });
    chart.getStyler().setLegendVisible(true);
    chart.getStyler().setDefaultSeriesRenderStyle(XYSeriesRenderStyle.Line);
    chart.getStyler().setYAxisLabelAlignment(Styler.TextAlignment.Right);
    chart.getStyler().setXAxisTicksVisible(true);
 
    // Show it
    sw = new SwingWrapper<XYChart>(chart);
    sw.displayChart().setLocation(1100, 800);
   //sw.displayChart();
 
  }
   
   void refresh( double[] mostRecentLongSet, double[] mostRecentLatSet){
        //chart.updateXYSeries("randomWalk", mostRecentLatSet, mostRecentLongSet, null);
        chart.removeSeries("Longitude");
        chart.removeSeries("Latitude");
        chart.addSeries("Longitude", null, mostRecentLongSet);
        chart.addSeries("Latitude", null, mostRecentLatSet);
        //chart.updateXYSeries("randomWalk", mostRecentLongSet, mostRecentLatSet, null);
        sw.repaintChart();
    }
}
