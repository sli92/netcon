package program;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

import lib.Module;
import lib.Netcon;
import enums.GET;

public class ModuleConnector implements Runnable{

	Thread t;
	private Module module;	// Thread fuer Modul

	public ModuleConnector(Module module) {
		
		this.module = module;
		
		t = new Thread(this, "ModuleConnector");
		t.start();

	}

	public void run() {
			
		// System.out.println("Modulthread f�r " + module.getHostname());
		
		Socket clientSocket = null;
		DataOutputStream outToServer = null; 
		BufferedReader inFromServer = null;
		
		// Verbindung herstellen
		try {
			clientSocket = new Socket(module.getIp(), 50003);
			
			outToServer = new DataOutputStream(clientSocket.getOutputStream());
			
			outToServer.write(Netcon.netcon(GET.devicecount, ""));
			
			inFromServer = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
			
			if(inFromServer.readLine().equals("OK")) {
				
				module.setDevicecount(Integer.parseInt(inFromServer.readLine()));
				
				module.setType(new int[module.getDevicecount()]);
				module.setValue(new String[module.getDevicecount()]);
				module.setDtype(new String[module.getDevicecount()]);
			} else {
				
				System.out.println("Fehler bei der Kommunikation mit : " + module.getHostname());
				return;
			}

			
		} catch (Exception e) {
			System.out.println(module.getHostname() + " nicht erreichbar. Modulthread beendet!");
			// e.printStackTrace();
		} 
		
		int i;
		int type[] = new int[module.getDevicecount()];
		String value[] = new String[module.getDevicecount()];
		String dtype[] = new String[module.getDevicecount()];
		
		while(true) {
			
			long startTime = System.currentTimeMillis();
		
			for(i = 0; i<module.getDevicecount(); i++) {
				
				try {
					outToServer.write(Netcon.netcon(GET.devicetype, String.valueOf(i)));
					
					inFromServer.readLine();
					
					type[i] = Integer.parseInt(inFromServer.readLine());
					
					outToServer.write(Netcon.netcon(GET.value, String.valueOf(i)));
					
					inFromServer.readLine();
					
					value[i] = inFromServer.readLine();
					
					outToServer.write(Netcon.netcon(GET.dtype, String.valueOf(i)));
					
					inFromServer.readLine();
					
					dtype[i] = inFromServer.readLine();
					
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
				
			}
			
			module.setType(type);
			module.setValue(value);
			module.setDtype(dtype);
			
			while((System.currentTimeMillis() - startTime) < 3000);
			
			
			
		}
		
	}

}