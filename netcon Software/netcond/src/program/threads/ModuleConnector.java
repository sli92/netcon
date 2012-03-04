package program.threads;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

import program.main.Netcond;


import lib.module.Module;
import lib.protocol.Netcon;
import lib.protocol.NetconGET;

public class ModuleConnector implements Runnable {

	Thread t;
	private Module module; 
	private int connectionTries;
	private Socket clientSocket = null;

	public ModuleConnector(Module module) {

		this.module = module;
		setConnectionTries(1);

		t = new Thread(this, "ModuleConnector");
		t.start();

	}

	public void run() {

		System.out.println("Modulthread for " + module.getHostname() + " ID: "
				+ t.getId());

		DataOutputStream outToServer = null;
		BufferedReader inFromServer = null;

		// connection to the module
		while(true){
			
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
					
					terminate(" ID: "+ t.getId() + " connection loss. Module deleted");
					return;
				}
				
				break;
			
			// connection not possible
			} catch (Exception e) {
				
				if(connectionTries > 0) {
					
					timeout();
					
					continue;
				}
				
				else{
					
					terminate(t.getId() +": read time out. Thread beendet!");
					
					return;
					
				}
				
			}
		}

		int i;
		int type[] = new int[module.getDevicecount()];
		String value[] = new String[module.getDevicecount()];
		String dtype[] = new String[module.getDevicecount()];
		String minValue[] = new String[module.getDevicecount()];
		String maxValue[] = new String[module.getDevicecount()];
		
		setConnectionTries(1);
		
		// get devicetype and value type for each device
		for (i = 0; i < module.getDevicecount(); i++) {

			try {
				
				outToServer.write(Netcon.netcon(NetconGET.devicetype,
						String.valueOf(i)));
				
				if(inFromServer.readLine().equals("OK")) 
					type[i] = Integer.parseInt(inFromServer.readLine());
				else{
					inFromServer.readLine();
					continue;
				}	

				threadSleep(10);
				
				outToServer.write(Netcon.netcon(NetconGET.min,
						String.valueOf(i)));
				
				if(inFromServer.readLine().equals("OK"))
					
					minValue[i] = inFromServer.readLine();
				else{
					inFromServer.readLine();
					continue;
				}					
				
				threadSleep(10);
				
				outToServer.write(Netcon.netcon(NetconGET.max,
						String.valueOf(i)));
				
				if(inFromServer.readLine().equals("OK"))
					
					maxValue[i] = inFromServer.readLine();
				else{
					inFromServer.readLine();
					continue;
				}	
				
				
				threadSleep(10);

				outToServer.write(Netcon.netcon(NetconGET.dtype,
						String.valueOf(i)));

				if(inFromServer.readLine().equals("OK"))
					
					dtype[i] = inFromServer.readLine();
				else{
					inFromServer.readLine();
					continue;
				}	

				threadSleep(10);

			} catch (IOException e) {
				
				if(connectionTries > 0) {
					
					timeout();
;
					i = 0;
					continue;
				}
				
				else{
					
					terminate(t.getId() +": read time out. Thread beendet!");
					
					return;
				}
			}

				module.setType(type);
				module.setDtype(dtype);
				module.setMinValue(minValue);
				module.setMaxValue(maxValue);

		}
		
		setConnectionTries(1);

		while (true) {

			long startTime = System.currentTimeMillis();
			
			try {
				outToServer.write(Netcon.netcon(NetconGET.uptime,
						String.valueOf(i)));
				
				if(inFromServer.readLine().equals("OK"))
					module.setUptime(inFromServer.readLine());
				else{
					inFromServer.readLine();
					continue;
				}
			} catch (IOException e2) {
				
				if(connectionTries > 0) {
					
					timeout();
	
					i = 0;
					continue;
				}
				
				else{
					
					System.out.println(t.getId() +": read time out. Thread beendet!");
					
					closeSocket();
					
					synchronized ( Netcond.moduleList ) {
						Netcond.moduleList.remove(module);
					}
					return;
				}	
			}

			threadSleep(10);

			for (i = 0; i < module.getDevicecount(); i++) {

				try {
						
					
					outToServer.write(Netcon.netcon(NetconGET.value,
							String.valueOf(i)));
					
					if(inFromServer.readLine().equals("OK")){
						
						value[i] = inFromServer.readLine();

					}
					else{
						inFromServer.readLine();
						continue;
					}		
					
					threadSleep(10);
					
					setConnectionTries(1);

				} catch (IOException e) {
					
					if(connectionTries > 0) {
						
						timeout();

						i = 0;
						continue;
					}
					
					else{
						
						terminate(t.getId() +": read time out. Thread beendet!");
						return;
					}
					
				}

			}
			
				module.setValue(value);

			while ((System.currentTimeMillis() - startTime) < 1000) {
				threadSleep(1);
			}

		}

	}


	private void setConnectionTries(int connectionTries) {
		this.connectionTries = connectionTries;
	}
	
	private void closeSocket() {
		
		try {
			clientSocket.close();
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
	}
	
	private void timeout(){
		
		System.out.println(t.getId() +": read time out.");
		connectionTries--;
		
	}
	
	private void terminate(String msg) {
		
		System.out.println(msg);
		
		closeSocket();
		
		synchronized ( Netcond.moduleList ) {
			Netcond.moduleList.remove(module);
		}
	}
	
	private void threadSleep(int time){
		
		try {
			Thread.sleep(time);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}	
	}


}
