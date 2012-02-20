package program.threads;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import lib.module.ModuleStock;

public class WebConnector implements Runnable {

	Thread t;
	private ModuleStock list;

	public WebConnector(ModuleStock list) {

		this.list = list;
		t = new Thread(this, "WebConnector");
		t.start();

	}

	public void run() {
		
		System.out.println("Webconnector gestartet!");

		ServerSocket welcomeSocket = null;
		

		try {
			welcomeSocket = new ServerSocket(5004);

			while (true) {

				Socket connectionSocket = welcomeSocket.accept();

				new WebThread(connectionSocket, list);
				

			}

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

}
