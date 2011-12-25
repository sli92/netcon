package program;

/*
 *  NetFinder(Bundle) Version 0.02
 *  NetFinder Version 0.02 Build 111111
 */

import java.io.IOException;
import java.net.DatagramPacket;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JFrame;
import javax.swing.UIManager;

import lib.Module;
import lib.ModuleStock;
import lib.NetFFrame;
import lib.Network;

public class NetFinder {
	
	public static void main(String[] args) {
		byte[] request = new byte[15];
		
		request[0] = 'n';
		request[1] = 'e';
		request[2] = 't';
		request[3] = 'f';
		request[4] = 'i';
		request[5] = 'n';
		request[6] = 'd';
		request[7] = 0x00;				// Version
		request[8] = (byte)0xFF;		// Type
		
		// Mac-Addr
		request[9] = (byte)0xFF;
		request[10] = (byte)0xFF;
		request[11] = (byte)0xFF;
		request[12] = (byte)0xFF;
		request[13] = (byte)0xFF;
		request[14] = (byte)0xFF;
		
		
		ModuleStock list = new ModuleStock();
		
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
			} catch (Exception e) {
				e.printStackTrace();
			}

		JFrame frame = new NetFFrame(list);
		frame.setVisible(true);
		
		List<Module> tempList = null;

		while (true) {

			try {
				Network.sendBroadcast(request, 50000, 50001);
			} catch (IOException e) {
				e.printStackTrace();
			}

			DatagramPacket recv = null;
			Module module = null;
			tempList = new ArrayList<Module>();

			long startTime = System.currentTimeMillis();
			
			while (true) {

				module = null;

				try {
					recv = Network.receivePacket(50001, 2000);
				} catch (IOException e) {
					e.printStackTrace();
				}

				if (recv != null) {
					module = new Module(recv.getData(), recv.getAddress(), recv.getPort());
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
