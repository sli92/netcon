package program.threads;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.Socket;

import lib.web.WebRequest;

import org.json.simple.JSONObject;

public class WebThread implements Runnable {
	
	Thread t;
	private Socket connectionSocket;
	
	public WebThread(Socket connectionSocket) {
		
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
				
				JSONObject answer = WebRequest.get(inFromClient.readLine());
			
				outToClient = new DataOutputStream(connectionSocket.getOutputStream());
				
				if(answer != null)
					outToClient.writeBytes(answer.toJSONString());
				else 
					outToClient.writeBytes("error");

			}
			else if(client.equals("SET")) {
				
				String answer = WebRequest.set(inFromClient.readLine());

				outToClient = new DataOutputStream(connectionSocket.getOutputStream());
				
				if(answer != null)
					outToClient.writeBytes("OK " + answer);
				else
					outToClient.writeBytes("error");
	
			}
			else {
				
				outToClient = new DataOutputStream(connectionSocket.getOutputStream());
				
				outToClient.writeBytes("error");
				
				System.out.println("E: Incomprehensible web request (" + client + ")");
				
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
	

}
