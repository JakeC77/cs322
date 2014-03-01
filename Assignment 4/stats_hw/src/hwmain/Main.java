package hwmain;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;


//Program to tally up the results of my tests and calculate averages. It then writes them to a csv.
//Jake Christensen CS 322
public class Main {

	/**
	 * @param args
	 */
	static ArrayList<Result> results;
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		String path = System.getProperty("user.dir");

//		String[] files = { "Results.csv", "Results2.csv", "Results1.csv" };
		String file = "Results.csv";
		
		results = new ArrayList<Result>();
//		for (String s : files) {
			try (BufferedReader br = new BufferedReader(new FileReader(path
					+ "/" + file))) {
				StringBuilder sb = new StringBuilder();
				String line = br.readLine();

				while (line != null) {
					Result res = new Result();
					String[] vals = line.split(",");
					res.nValue = Integer.valueOf(vals[0]);
					res.cores = Integer.valueOf(vals[1]);
					res.time = Double.valueOf(vals[2]);
					res.loop = Integer.valueOf(vals[3]);
					results.add(res);
					sb.append(line);
					sb.append(System.lineSeparator());
					line = br.readLine();
				}
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	//	}
		String test = path;
		getAverageTimeByCoreNum(5,1);
		writeAllAvgsToFile();
	}

	public static double getAverageTimeByCoreNum(int CoreNum, int typeNum) {
		int count = 0;
		double total = 0;
		for(Result r : results){
			if(r.cores == CoreNum){
				total = total + r.time;
				count++;
			}
			
		}
		System.out.println("Average is " + (total/count));
		return (total/count);
	}

	public static double[] getAverageTimeByCoreAndN(int coreNum, int NValue){
		int count = 0;
		ArrayList<Result> selected = new ArrayList<Result>();
		
		double total = 0;
		for(Result r : results){
			if(r.cores == coreNum && r.nValue == NValue){
				total = total + r.time;
				count++;
				selected.add(r);
			}
			
		}
		System.out.println("Average is " + (total/count));
		double avg = total/count;
		double tot = 0;
		for(Result r :selected){
		tot += (r.time - avg)*(r.time - avg);
		}
		double stdev;
			try{
				stdev = Math.sqrt((1/count)*tot);
			}catch(Exception e){
				 stdev = 0;
			}
		double[] result ={avg,stdev}; 
		return result ;
		
		
		
	}
	
	
	public static double getAverageTimeByNValue(int NValue) {
		int count = 0;
		double total = 0;
		for(Result r : results){
			if( r.nValue == NValue){
				total = total + r.time;
				count++;
			}
			
		}
		
		System.out.println("Average is " + (total/count));
		return (total/count);
	}
	
	
	
	public static double getAverageTimeByLoop(int loopNum, int typeNum) {
		int count = 0;
		double total = 0;
		for(Result r : results){
			if( r.loop == loopNum){
				total = total + r.time;
				count++;
			}
			
		}
		System.out.println("Average is " + (total/count));
		return (total/count);
	}
	
	
	
	public static void writeAllAvgsToFile(){
		int[] cores = {5,10,20,40};
		int[] Ns = {5000,1200,120};
		PrintWriter writer = null;
		try {
			writer = new PrintWriter("AVGResults_A4.csv", "UTF-8");
			writer.println("Average for Cores");
			
//Here we get the average and standard deviations for a variety of different configurations, and write them to a csv file.
			for(int i : cores){
				writer.println("# Cores, N Value,Avg,Stdev");
				for(int j : Ns){
					double[] res = getAverageTimeByCoreAndN(i,j);
					writer.println(i + "," +j+ "," + res[0] +"," + res[1]);
					
				}
			}			
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		writer.close();
		
	}
	
	
}
