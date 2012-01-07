package program;

import java.io.IOException;
import java.net.DatagramPacket;
import java.util.ArrayList;
import java.util.List;

import lib.Module;
import lib.ModuleStock;
import lib.Netfind;
import lib.Network;
import lib.WebConnector;

public class Netcond {
	
	public static void main(String[] args) {
		
		ModuleStock list = new ModuleStock();
		List<Module> tempList = new ArrayList<Module>();
		
		new WebConnector(list);

		while (true) {
			
			tempList.clear();

			try {
				Network.sendBroadcast(Netfind.netfind(), 50000, 50001);
			} catch (IOException e) {
				e.printStackTrace();
			}

			DatagramPacket recv = null;
			Module module = null;
			
			long startTime = System.currentTimeMillis();
			
			while (true) {

				module = null;

				try {
					recv = Network.receivePacket(50001, 2000);
				} catch (IOException e) {
					e.printStackTrace();
				}

				if (recv != null) {
					module = Netfind.netdiscover(recv);
					if(module != null) 
						tempList.add(module);

				} 
				
				if((System.currentTimeMillis() - startTime) > 3500)
					break;

			}
			
			// Abgleich mit ModulStock
			list.equalize(tempList);

		}

	}

}
