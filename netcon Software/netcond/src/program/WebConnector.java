package program;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import lib.Module;
import lib.ModuleStock;

public class WebConnector implements Runnable {

	Thread t;
	private ModuleStock list;

	public WebConnector(ModuleStock list) {

		this.list = list;
		t = new Thread(this, "WebConnector");
		t.start();

	}

	public void run() {

		ServerSocket welcomeSocket = null;
		

		try {
//			String clientSentence = null;
			welcomeSocket = new ServerSocket(5004);

			while (true) {

				Socket connectionSocket = welcomeSocket.accept();
			
				
//				BufferedReader inFromClient =
//			               new BufferedReader(new InputStreamReader(connectionSocket.getInputStream()));
//			
//				clientSentence = inFromClient.readLine();
				
				DataOutputStream outToClient = new DataOutputStream(
						connectionSocket.getOutputStream());

				for (Module mod : list.getList()) {

					outToClient.writeBytes("*" + mod.getHostname()
							+ ", Standort: " + mod.getStandort()
							+ ", online seit " + mod.getUptime() / 100 + " s"
							+ " (" + mod.getIp() + ":" + mod.getPort() + ")" + "<br>");
					
					int i;
					
					for(i = 0; i<mod.getDevicecount(); i++) {
						
						outToClient.writeBytes("Device " + i  + ": "+ mod.getValue()[i] + " V<br>");
					}

				}

				outToClient.writeBytes("#\n");
				
				

			}

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

}
