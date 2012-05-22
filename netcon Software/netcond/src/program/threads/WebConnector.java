package program.threads;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class WebConnector implements Runnable {

	Thread t;

	public WebConnector() {

		t = new Thread(this, "WebConnector");
		t.start();

	}

	public void run() {
		
		System.out.println("Webconnector started!");

		ServerSocket welcomeSocket = null;
		

		try {
			welcomeSocket = new ServerSocket(50004);

			while (true) {

				Socket connectionSocket = welcomeSocket.accept();

				new WebThread(connectionSocket);
				

			}

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

}
