
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.MulticastSocket;
import java.net.SocketAddress;
import java.net.SocketException;


public class Network {
	
private static String group = "255.255.255.255";

	public static void sendBroadcast(String msg, int dstPort, int srcPort) throws IOException {
		
		SocketAddress inet = new InetSocketAddress(InetAddress.getLocalHost(), srcPort);
	
		MulticastSocket socket = new MulticastSocket(inet);
		
		DatagramPacket msgPacket = new DatagramPacket(msg.getBytes(), msg.length(), InetAddress.getByName(group), dstPort);
		
		socket.send(msgPacket);
		
		socket.close();
	}
			
	public static DatagramPacket receiveBroadcast(int lstPort) throws IOException {
		
		MulticastSocket socket = new MulticastSocket(lstPort);
	
		// receive broadcast
		byte[] buf = new byte[100];
		
		DatagramPacket recv = new DatagramPacket(buf, buf.length);
		socket.receive(recv);
	
		socket.close();
		
		return recv;		
		
	}
	
	public static void sendPacket(String msg, InetAddress dstAdr, int dstPort, int srcPort) throws IOException {
		
		DatagramSocket socket = new DatagramSocket(srcPort, InetAddress.getLocalHost());
		
		DatagramPacket packet = new DatagramPacket(msg.getBytes(), msg.length(), dstAdr, dstPort);
		
		socket.send(packet);
		
		socket.close();
		
	}
	
	public static DatagramPacket receivePacket(int lstPort, int timeout) throws SocketException {
		
		DatagramSocket socket = new DatagramSocket(lstPort);
		
		byte[] buf = new byte[100];
		
		DatagramPacket packet = new DatagramPacket(buf, buf.length);

		socket.setSoTimeout(timeout);
		
		try {
			socket.receive(packet);
		} catch (IOException e) {
			packet = null;
			socket.close();
			return packet;
		}
		
		socket.close();
		
		return packet;
	}

}
