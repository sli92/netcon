package program;

/*
 *  NetFinder(Bundle) Version 0.02
 *  NetFinder Version 0.02 Build 111111
 */

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
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
		byte[] mac_filter = {(byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF};
		ByteArrayOutputStream requestBytes = new ByteArrayOutputStream();
		DataOutputStream request = new DataOutputStream(requestBytes);
		
		ModuleStock list = new ModuleStock();
		
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
			
			request.writeBytes("netfind");
			request.writeByte(0x00);			// Version
			request.writeByte(0xFF);			// Type

			request.write(mac_filter);
			request.flush();
			
			} catch (Exception e) {
				e.printStackTrace();
			}

		JFrame frame = new NetFFrame(list);
		frame.setVisible(true);
		
		List<Module> tempList = null;

		while (true) {

			try {
				Network.sendBroadcast(requestBytes.toByteArray(), 50000, 50001);
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
