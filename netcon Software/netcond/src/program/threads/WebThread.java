package program.threads;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.Socket;

import lib.module.ModuleStock;
import lib.web.WebRequest;

public class WebThread implements Runnable {
	
	Thread t;
	private Socket connectionSocket;
	private ModuleStock list;
	
	public WebThread(Socket connectionSocket, ModuleStock list) {
		
		setList(list);
		setConnectionSocket(connectionSocket);
		t = new Thread(this, "WebThread");
		t.start();
	}

	@Override
	public void run() {

		try {
			
			BufferedReader inFromClient = new BufferedReader(new InputStreamReader(connectionSocket.getInputStream()));
			DataOutputStream outToClient = null;
			
			String client = inFromClient.readLine();
			
			if(client.equals("GET")) {
				
				String answer = WebRequest.get(inFromClient.readLine(), list);
			
				outToClient = new DataOutputStream(connectionSocket.getOutputStream());
				
				if(answer != null)
					outToClient.writeBytes(answer);
				else
					outToClient.writeBytes("error");
				
			}
			else if(client.equals("SET")) {
				
				String answer = WebRequest.set(inFromClient.readLine(), list);

				outToClient = new DataOutputStream(connectionSocket.getOutputStream());
				
				if(answer != null)
					outToClient.writeBytes("OK " + answer);
				else
					outToClient.writeBytes("error");
	
			}
			else {
				
				outToClient = new DataOutputStream(connectionSocket.getOutputStream());
				
				outToClient.writeBytes("error");
				
			}
			
			outToClient.writeBytes("\n");
			
			connectionSocket.close();

			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
	}
		

	public Socket getConnectionSocket() {
		return connectionSocket;
	}

	public void setConnectionSocket(Socket connectionSocket) {
		this.connectionSocket = connectionSocket;
	}
	
	public ModuleStock getList() {
		return list;
	}

	public void setList(ModuleStock list) {
		this.list = list;
	}

}
