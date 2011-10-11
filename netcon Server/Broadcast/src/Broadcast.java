
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;


public class Broadcast {
	
	private static String group = "255.255.255.255";
	
	public static void sendBroadcast(String msg, int port) throws IOException {
		
		// TODO IllegalArguments
		
		MulticastSocket socket = new MulticastSocket();
		
		DatagramPacket msgPacket = new DatagramPacket(msg.getBytes(), msg.length(), InetAddress.getByName(group), port);
		
		socket.send(msgPacket);
		
		socket.close();
	}
			
	
	public static String receiveBroadcast(int port) throws IOException {
		
		MulticastSocket socket = new MulticastSocket(port);
	
		// receive broadcast
		byte[] buf = new byte[1024];
		
		DatagramPacket recv = new DatagramPacket(buf, buf.length);
		socket.receive(recv);
		
		String recvMessage = new String(buf);
	
		socket.close();
		
		return recvMessage;			
		
	}

}
