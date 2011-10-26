package program;

/*
 *  NetFinder(Bundle) Version 0.02
 *  NetFinder Version 0.02 Build 111026
 */

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
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
				Network.sendPacket("bcnetcon",
						InetAddress.getByName("255.255.255.255"), 6799, 6800);
			} catch (IOException e) {
				e.printStackTrace();
			}

			DatagramPacket recv = null;
			Module module = null;
			tempList = new ArrayList<Module>();

			while (true) {

				module = null;

				try {
					recv = Network.receivePacket(6800, 2000);
				} catch (IOException e) {
					e.printStackTrace();
				}

				if (recv != null) {
					module = new Module(new String(recv.getData()).toString(),
							recv.getAddress(), recv.getPort());
					tempList.add(module);

				} else
					break;

			}

			list.equalize(tempList);

		}

	}

}
