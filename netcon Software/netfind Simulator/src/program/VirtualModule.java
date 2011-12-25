package program;

/*
 * VirtualModule Version 0.01 Build 111026
 */

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Random;

import lib.Network;

public class VirtualModule {
	
	public static void main(String[] args) {
		
		long starttime = System.currentTimeMillis();
		
		DatagramPacket recv = null;
		Random random = new Random();
		
		int moduleNr = random.nextInt(100);

		byte[] mac_addr = new byte[6];
		random.nextBytes(mac_addr);
		
		String hostname = "Modul" + moduleNr;
		
		String standort = "Kueche";

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
					byte[] response = new byte[86];
					
					response[0] = 'n';
					response[1] = 'e';
					response[2] = 't';
					response[3] = 'd';
					response[4] = 'i';
					response[5] = 's';
					response[6] = 'c';
					response[7] = 'o';
					response[8] = 'v';
					response[9] = 'e';
					response[10] = 'r';
					
					response[11] = mac_addr[0];
					response[12] = mac_addr[1];
					response[13] = mac_addr[2];
					response[14] = mac_addr[3];
					response[15] = mac_addr[4];
					response[16] = mac_addr[5];
					
					response[17] = 0x00; // Type
					
					// Set uptime to 0
					long uptime = (System.currentTimeMillis() - starttime) / 10;
					ByteArrayOutputStream bos = new ByteArrayOutputStream();  
					DataOutputStream dos = new DataOutputStream(bos); 
					dos.writeInt((int)uptime);  
					dos.flush();  
					byte[] buptime = bos.toByteArray();
					
					response[18] = buptime[0];
					response[19] = buptime[1];
					response[20] = buptime[2];
					response[21] = buptime[3];
					
					byte[] temp = hostname.getBytes();
					
					for(int i = 0; i < 32; i++) {
						if(i < temp.length)
							response[22 + i] = temp[i];
						else
							response[22 + i] = 0x00;
					}
					
					temp = standort.getBytes();
					
					for(int i = 0; i < 32; i++) {
						if(i < temp.length)
							response[54 + i] = temp[i];
						else
							response[54 + i] = 0x00;
					}
					
					Network.sendBroadcast(response, 50001, 50000);
	
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

	}	
	
}
