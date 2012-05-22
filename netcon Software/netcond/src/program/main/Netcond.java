package program.main;

import java.io.IOException;
import java.net.DatagramPacket;
import java.util.ArrayList;
import java.util.List;

import lib.module.Module;
import lib.network.UDP;
import lib.protocol.Netfind;
import program.threads.WebConnector;

public class Netcond {
	
	public static List<Module> moduleList = new ArrayList<Module>();	
	
	public static void main(String[] args) {
		
		// starting web thread for web communication
		new WebConnector();
		
		// search for new modules
		while (true) {
			
			// UDP-broadcast 
			try {
				UDP.sendBroadcast(Netfind.netfind(), 50000, 50001);
			} catch (IOException e) {
				System.out.println("error by sending udp-broadcast");
				e.printStackTrace();
			}

			DatagramPacket recv = null;
			Module module = null;
			
			long startTime = System.currentTimeMillis();
			
			while (true) {

				module = null;
				
				// module response
				try {
					recv = UDP.receivePacket(50001, 2000); 		// port, timeout
				} catch (IOException e) {
					e.printStackTrace();
				}
				
				// process module data
				if (recv != null) {
					module = Netfind.netdiscover(recv);
					
					if(module != null) {
						
						// check if "list" already contains module
						if(!moduleList.contains(module)) {
							
							// add to temporary list
							module.startThread();
							
							synchronized ( moduleList ) 
							{ 
								moduleList.add(module); 
							}
							
							
						}
							
							
					}

				} 
				
				// check if time is expired
				if((System.currentTimeMillis() - startTime) > 2000)
					break;
				
			}

		}

	}

}
