package program;
/*
	NetFinder Version 0.01 Build 111020

*/

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.UnknownHostException;

import lib.Network;

public class NetFinder {
	
	public static void main(String[] args) {
		
		try {
			System.out.println("netcon netfinder an " +  InetAddress.getLocalHost().toString() + "\n");
		} catch (UnknownHostException e1) {
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
				Network.sendPacket("bcnetcon", InetAddress.getByName("255.255.255.255"), 6799, 6800);
			} catch (IOException e) {
				e.printStackTrace();
				}
			
			DatagramPacket recv = null;
			String message = null;
			
			System.out.println("Search started - found modules: ");
			
			boolean found = false;
			
			while(true) {
								
					try {
						recv = Network.receivePacket(6800, 4000);
					} catch (IOException e) {
						e.printStackTrace();
					}
					
					if( recv != null) {
						found = true;
						message = new String(recv.getData());
						message = message.trim();
						System.out.println(message + " (" + recv.getAddress().toString() + ":" + recv.getPort() + ")");
					}
					else
						break;
					
			}
			
			System.out.println("Search complete");
			
			if(found == false) 
				System.out.print("...No modules found!");
			
			System.out.println("\n");
					
		}
	}		
}
