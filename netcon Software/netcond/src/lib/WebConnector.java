package lib;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

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
			welcomeSocket = new ServerSocket(6789);

			while (true) {

				Socket connectionSocket = welcomeSocket.accept();

				DataOutputStream outToClient = new DataOutputStream(
						connectionSocket.getOutputStream());

				for (Module mod : list.getList()) {

					outToClient.writeBytes("*" + mod.getHostname()
							+ ", Standort: " + mod.getStandort()
							+ ", online seit " + mod.getUptime() / 100 + " s"
							+ " (" + mod.getIp() + ":" + mod.getPort() + ")"
							+ "#");

				}

				outToClient.writeBytes("\n");

			}

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

}
