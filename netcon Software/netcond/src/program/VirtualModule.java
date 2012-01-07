package program;

// VirtualModule
// simuliert (Hardware)Module

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.util.Random;

import lib.Network;

public class VirtualModule {
	
	public static byte[] stringToByteArray(String input, int padUp) {
		if(padUp == 0)
			return input.getBytes();
		
		byte[] inputBytes = input.getBytes();
		byte[] output = new byte[padUp];
		
		for(int i = 0; i < padUp; i++) {
			if(i < input.length())
				output[i] = inputBytes[i];
			else
				output[i] = 0;
		}
		
		return output;
	}
	
	public static void main(String[] args) {
		
		long starttime = System.currentTimeMillis();
		
		DatagramPacket recv = null;
		Random random = new Random();
		
		int moduleNr = random.nextInt(100);

		byte[] mac_addr = new byte[6];
		random.nextBytes(mac_addr);
		
		String hostname = "Modul" + moduleNr;
		String standort = "Kueche" + moduleNr;
		int type = 0;
		

		System.out.println("Hostname: " + hostname + "\nOrt: " + standort);
		
		System.out.printf("Mac Addr: %X %X %X %X %X %X\n", mac_addr[0], mac_addr[1], mac_addr[2],
														   mac_addr[3], mac_addr[4], mac_addr[5]);
	
		while (true) {

			try {
				recv = Network.receiveBroadcast(50000);
			} catch (IOException e) {
				System.out.println(e.toString());
			}

			byte data[] = recv.getData();
			
			// Don't check Type and MAC-Addr for now
			if((new String(data, 0, 7)).equals("netfind") && data[7] == 0x00) {	
				
				System.out.print("Broadcast received - Sending response to ");
				System.out.println("(" + recv.getAddress().toString() + ":"
						+ recv.getPort() + ")");
	
				long startTime = System.currentTimeMillis();
	
				int rand = random.nextInt(1800);
				while (System.currentTimeMillis() - startTime < (rand));
	
				try {
					// You like streams? I do :D
					ByteArrayOutputStream responseBytes = new ByteArrayOutputStream();
					DataOutputStream response = new DataOutputStream(responseBytes);
					
					response.writeBytes("netdiscover");
					response.write(mac_addr);
					response.writeByte(type);
					response.writeInt((int)((System.currentTimeMillis() - starttime) / 10));
					response.write(stringToByteArray(hostname, 32));
					response.write(stringToByteArray(standort, 32));
					response.flush();
					
					Network.sendBroadcast(responseBytes.toByteArray(), 50001, 50000);
					
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

	}	
	
}
