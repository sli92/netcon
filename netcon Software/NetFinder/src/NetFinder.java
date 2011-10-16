/*
	NetFinder Version 0.01 Build 111016
	Bug to fix: whitespace in received message on Windows PCs

*/


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.UnknownHostException;

public class NetFinder {
	
	public static void main(String[] args) {
		
		try {
			System.out.println("netcon netfinder an " +  InetAddress.getLocalHost().toString() + "\n");
		} catch (UnknownHostException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		
		while(true) {
			
			System.out.println("Press RETURN to find modules in the network");
			
			InputStreamReader isr = new InputStreamReader(System.in);
			BufferedReader br = new BufferedReader(isr);
			
			try{
				br.readLine();
			} catch(Exception e) {
				e.printStackTrace();
				}
			
			try{
				Network.sendPacket("bcnetcon", InetAddress.getByName("255.255.255.255"), 6799);
			} catch (IOException e) {
				e.printStackTrace();
				}
			
			DatagramPacket recv = null;
			String message = null;
			
			System.out.println("Search started - found modules: ");
			
			long startTime = System.currentTimeMillis();
			
			boolean found = false;
			
			while(true) {
				
				if(System.currentTimeMillis() - startTime > 15000)
					break;
				
					try {
						recv = Network.receivePacket(6800, 5000);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					if( recv != null) {
						found = true;
						message = new String(recv.getData());
						System.out.println(message + " (" + recv.getAddress().toString() + ":" + recv.getPort() + ")");
					}
					
			}
			
			System.out.println("Search complete");
			
			if(found == false) 
				System.out.print("...No modules found!");
			
			System.out.println("\n");
					
		}
	}		
}
