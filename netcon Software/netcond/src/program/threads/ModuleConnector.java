package program.threads;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;


import lib.module.Module;
import lib.protocol.Netcon;
import lib.protocol.NetconGET;

public class ModuleConnector implements Runnable {

	Thread t;
	private Module module; // module thread

	public ModuleConnector(Module module) {

		this.module = module;

		t = new Thread(this, "ModuleConnector");
		t.start();

	}

	public void run() {

		System.out.println("Modulthread für " + module.getHostname() + " ID: "
				+ t.getId());

		Socket clientSocket = null;
		DataOutputStream outToServer = null;
		BufferedReader inFromServer = null;

		// connect to the module
		try {
			clientSocket = new Socket(module.getIp(), 50003);

			clientSocket.setSoTimeout(5000);

			outToServer = new DataOutputStream(clientSocket.getOutputStream());

			outToServer.write(Netcon.netcon(NetconGET.devicecount, ""));

			inFromServer = new BufferedReader(new InputStreamReader(
					clientSocket.getInputStream()));

			if (inFromServer.readLine().equals("OK")) {

				module.setDevicecount(Integer.parseInt(inFromServer.readLine()));

				module.setType(new int[module.getDevicecount()]);
				module.setValue(new String[module.getDevicecount()]);
				module.setDtype(new String[module.getDevicecount()]);
			} else {

				System.out.println("Modul nicht netcon kompatibel: "
						+ module.getHostname() + "  " + t.getId()
						+ " Thread beendet");
				return;
			}

		} catch (Exception e) {
			System.out.println(module.getHostname()
					+ " nicht erreichbar. Modulthread " + t.getId()
					+ " beendet. Neustart eingeleitet. Noch "
					+ module.getConnectiontries() + " Versuche!");
			if (module.getConnectiontries() != 0) {
				module.setConnectiontries(module.getConnectiontries() - 1);
				module.restartThread();
			}

			return;
		}

		int i;
		int type[] = new int[module.getDevicecount()];
		String value[] = new String[module.getDevicecount()];
		String dtype[] = new String[module.getDevicecount()];
		
		module.setConnectiontries(4);

		while (true) {

			long startTime = System.currentTimeMillis();

			for (i = 0; i < module.getDevicecount(); i++) {

				try {
					outToServer.write(Netcon.netcon(NetconGET.devicetype,
							String.valueOf(i)));

					inFromServer.readLine();

					type[i] = Integer.parseInt(inFromServer.readLine());
					
					try {
						Thread.sleep(10);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}

					outToServer.write(Netcon.netcon(NetconGET.value,
							String.valueOf(i)));

					inFromServer.readLine();

					value[i] = inFromServer.readLine();
					
					try {
						Thread.sleep(10);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}

					outToServer.write(Netcon.netcon(NetconGET.dtype,
							String.valueOf(i)));

					inFromServer.readLine();

					dtype[i] = inFromServer.readLine();
					
					try {
						Thread.sleep(10);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					

				} catch (IOException e) {
<<<<<<< HEAD:netcon Software/netcond/src/program/threads/ModuleConnector.java
					System.out
							.println("Verbindung verloren zu "
									+ module.getHostname()
									+ ". Modulthread ID: "
									+ t.getId()
									+ "beendet. Erneuter Verbindungsversuch eingeleitet. Noch "
									+ module.getConnectiontries()
									+ " Versuche!");

					if (module.getConnectiontries() != 0) {
						module.setConnectiontries(module.getConnectiontries() - 1);
						module.restartThread();
					}

=======
					System.out.println("Verbindung verloren zu "
							+ module.getHostname() + ". Modulthread ID: " + t.getId() + " beendet.");
>>>>>>> blubblub:netcon Software/netcond/src/program/ModuleConnector.java
					return;
				}

			}

			module.setType(type);
			module.setValue(value);
			module.setDtype(dtype);

			while ((System.currentTimeMillis() - startTime) < 2000) {
				try {
					Thread.sleep(1);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}

		}

	}

}
