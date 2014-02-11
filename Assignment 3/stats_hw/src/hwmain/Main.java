package hwmain;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;


//Program to tally up the results of my tests and calculate averages. It then writes them to a csv.

public class Main {

	/**
	 * @param args
	 */
	static ArrayList<Result> results;
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		String path = System.getProperty("user.dir");

		String[] files = { "Results.csv", "Results2.csv", "Results1.csv" };
		results = new ArrayList<Result>();
		for (String s : files) {
			try (BufferedReader br = new BufferedReader(new FileReader(path
					+ "/" + s))) {
				StringBuilder sb = new StringBuilder();
				String line = br.readLine();

				while (line != null) {
					Result res = new Result();
					String[] vals = line.split(",");
					res.type = Integer.valueOf(vals[0]);
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
		}
		String test = path;
		getAverageTimeByCoreNum(5,1);
		writeAllAvgsToFile();
	}

	public static double getAverageTimeByCoreNum(int CoreNum, int typeNum) {
		int count = 0;
		double total = 0;
		for(Result r : results){
			if(r.type == typeNum && r.cores == CoreNum){
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
			if(r.type == typeNum && r.loop == loopNum){
				total = total + r.time;
				count++;
			}
			
		}
		System.out.println("Average is " + (total/count));
		return (total/count);
	}
	
	public static void writeAllAvgsToFile(){
		int[] cores = {5,10,15,20};
		int[] types = {1,2,3};
		PrintWriter writer = null;
		try {
			writer = new PrintWriter("AVGResults.csv", "UTF-8");
			writer.println("Average for Cores");
			

			for(int i : types){
				writer.println("Type Number " + i);
				for(int j : cores){
					writer.println(","+j+ "," + getAverageTimeByCoreNum(j,i));
					
				}
				
				
			}
			
			writer.println("Average for Loops");
			
			for(int i : types){
				writer.println("Type Number " + i);
				for(int j = 0; j < 31 ; j++){
					writer.println(","+j+ "," + getAverageTimeByLoop(j,i));
					
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
