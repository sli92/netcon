package program.main;

import java.io.IOException;
import java.net.DatagramPacket;
import java.util.ArrayList;
import java.util.List;

import program.threads.WebConnector;

import lib.module.Module;
import lib.module.ModuleStock;
import lib.network.UDP;
import lib.protocol.Netfind;

public class Netcond {
	
	public static void main(String[] args) {
		
		ModuleStock list = new ModuleStock();				// module list
		List<Module> tempList = new ArrayList<Module>();	// temporary module list
		
		// starting web thread for web communication
		new WebConnector(list);
		
		// gathering modules 
		while (true) {
			
			tempList.clear();
			
			// UDP-broadcast to find modules every 2 seconds
			try {
				UDP.sendBroadcast(Netfind.netfind(), 50000, 50001);
			} catch (IOException e) {
				System.out.println("error by sending udp-broadcast");
				e.printStackTrace();
			}

			DatagramPacket recv = null;
			Module module = null;
			
			// collecting module answers for 2 seconds
			
			long startTime = System.currentTimeMillis();
			
			while (true) {

				module = null;
				
				// module response
				try {
					recv = UDP.receivePacket(50001, 2000);
				} catch (IOException e) {
					e.printStackTrace();
				}
				
				// process module data
				if (recv != null) {
					module = Netfind.netdiscover(recv);
					
					if(module != null) 
						// add to temporary list
						tempList.add(module);

				} 
				
				// check if time is expired
				if((System.currentTimeMillis() - startTime) > 2000)
					break;
				
			}
			
			// match module list
			list.equalize(tempList);

		}

	}

}
