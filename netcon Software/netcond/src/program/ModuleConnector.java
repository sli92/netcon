package program;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.net.UnknownHostException;

import lib.Module;
import lib.Netcon;
import enums.GET;

public class ModuleConnector implements Runnable{

	Thread t;
	private Module module;	// Thread fuer Modul
	private String commandList; // Befehl vom WebConnector

	public ModuleConnector(Module module) {
		
		this.module = module;
		commandList = null;
		
		t = new Thread(this, "ModuleConnector");
		t.start();

	}

	public void run() {
			
		System.out.println("Modulthread für " + module.getHostname());
		
		Socket clientSocket = null;
		DataOutputStream outToServer = null; 
		
		// Verbindung herstellen
		try {
			clientSocket = new Socket(module.getIp(), 50003);
			
			outToServer = new DataOutputStream(clientSocket.getOutputStream());
			
			outToServer.write(Netcon.netcon(GET.devicecount, ""));
			
			BufferedReader inFromServer = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
			
			System.out.println(inFromServer.readLine());
			
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
			
	}

	public String getShared() {
		return commandList;
	}

}
