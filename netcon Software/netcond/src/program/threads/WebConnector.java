package program.threads;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.List;

import lib.module.Module;

public class WebConnector implements Runnable {

	Thread t;
	private List<Module> moduleList;

	public WebConnector(List<Module> moduleList) {

		this.moduleList = moduleList;
		t = new Thread(this, "WebConnector");
		t.start();

	}

	public void run() {
		
		System.out.println("Webconnector started!");

		ServerSocket welcomeSocket = null;
		

		try {
			welcomeSocket = new ServerSocket(5004);

			while (true) {

				Socket connectionSocket = welcomeSocket.accept();

				new WebThread(connectionSocket, moduleList);
				

			}

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

}
