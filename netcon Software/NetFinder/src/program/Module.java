package program;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Random;

import lib.Network;

public class Module {
	
	public static void main(String[] args) {
		
		DatagramPacket recv = null;
		Random random = new Random();
		int moduleNr = random.nextInt(100);
		
		try {
			System.out.println("netcon module " + moduleNr + " an " + InetAddress.getLocalHost().toString() );
		} catch (UnknownHostException e1) {
			e1.printStackTrace();
		}
		
		while(true) {
			
				try {
					recv = Network.receiveBroadcast(6799);
				}
				catch (IOException e) {
					System.out.println(e.toString());
				}
				
					
				System.out.print("Broadcast received - Sending response to ");
				System.out.println("("+recv.getAddress().toString()+ ":" + recv.getPort() + ")");

				long startTime = System.currentTimeMillis();
				
				int rand = random.nextInt(2000);
				while(System.currentTimeMillis() - startTime < (rand+500));
				
				try{
					
					Network.sendBroadcast("Module " + moduleNr + " present", 6800, 6799);
					
				} catch (IOException e) {
					e.printStackTrace();
					}	
		}

	}	
	
}