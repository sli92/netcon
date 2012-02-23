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
	private Socket clientSocket;

	public ModuleConnector(Module module) {

		this.module = module;
		setClientSocket(null);

		t = new Thread(this, "ModuleConnector");
		t.start();

	}

	public void run() {

		System.out.println("Modulthread für " + module.getHostname() + " ID: "
				+ t.getId());

		Socket clientSocket = null;
		DataOutputStream outToServer = null;
		BufferedReader inFromServer = null;

		// connection to the module
		try {
			clientSocket = new Socket(module.getIp(), 50003);

			clientSocket.setSoTimeout(5000);

			outToServer = new DataOutputStream(clientSocket.getOutputStream());
			
			// get number of devices
			outToServer.write(Netcon.netcon(NetconGET.devicecount, ""));

			inFromServer = new BufferedReader(new InputStreamReader(
					clientSocket.getInputStream()));
			
			// check wheter module is netcon compatible
			if (inFromServer.readLine().equals("OK")) {

				module.setDevicecount(Integer.parseInt(inFromServer.readLine()));

				module.setType(new int[module.getDevicecount()]);
				module.setValue(new String[module.getDevicecount()]);
				module.setDtype(new String[module.getDevicecount()]);
			} else {

				System.out.println("Modul nicht netcon kompatibel: "
						+ module.getHostname() + "  " + t.getId()
						+ " Thread beendet");
				clientSocket.close();
				return;
			}
		
		// connection not possible
		} catch (Exception e) {
			System.out.println(module.getHostname()
					+ " nicht erreichbar. Modulthread " + t.getId()
					+ " beendet. Neustart eingeleitet. Noch "
					+ module.getConnectiontries() + " Versuche!");
			if (module.getConnectiontries() != 0) {
				module.setConnectiontries(module.getConnectiontries() - 1);
				module.restartThread();
			}	
			
			try {
				clientSocket.close();
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			return;
		}

		int i;
		int type[] = new int[module.getDevicecount()];
		String value[] = new String[module.getDevicecount()];
		String dtype[] = new String[module.getDevicecount()];
		
		// get devicetype and value type for each device
		for (i = 0; i < module.getDevicecount(); i++) {

			try {
				outToServer.write(Netcon.netcon(NetconGET.devicetype,
						String.valueOf(i)));

				inFromServer.readLine();

				type[i] = Integer.parseInt(inFromServer.readLine());
				
				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}

				outToServer.write(Netcon.netcon(NetconGET.dtype,
						String.valueOf(i)));

				inFromServer.readLine();

				dtype[i] = inFromServer.readLine();
				
				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				
				

			} catch (IOException e) {
				
				e.printStackTrace();
				i=0;
				continue;
			}
			
			module.setType(type);
			module.setDtype(dtype);

		}
		
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
						e.printStackTrace();
					}

					outToServer.write(Netcon.netcon(NetconGET.value,
							String.valueOf(i)));

					inFromServer.readLine();

					value[i] = inFromServer.readLine();
					
					try {
						Thread.sleep(10);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}

					outToServer.write(Netcon.netcon(NetconGET.dtype,
							String.valueOf(i)));

					inFromServer.readLine();

					dtype[i] = inFromServer.readLine();
					
					try {
						Thread.sleep(10);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					
					

				} catch (IOException e) {
					
					System.out.println(e.toString() + " " + t.getId() + " beendet");
					
					try {
						clientSocket.close();
					} catch (IOException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					
					module.restartThread();
					return;
					

				}

			}

			module.setValue(value);

			while ((System.currentTimeMillis() - startTime) < 1000) {
				try {
					Thread.sleep(1);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}

		}

	}

	public Socket getClientSocket() {
		return clientSocket;
	}

	public void setClientSocket(Socket clientSocket) {
		this.clientSocket = clientSocket;
	}


}
