package lib.protocol;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.net.DatagramPacket;
import java.util.Arrays;

import lib.module.Module;

public class Netfind {
	
	protected static final byte[] Hexhars = {

		'0', '1', '2', '3', '4', '5',
		'6', '7', '8', '9', 'A', 'B',
		'C', 'D', 'E', 'F' 
	};
	

	public static String encode(byte[] b) {

		StringBuilder s = new StringBuilder(2 * b.length);

		for (int i = 0; i < b.length; i++) {

		int v = b[i] & 0xff;

		s.append((char)Hexhars[v >> 4]);
		s.append((char)Hexhars[v & 0xf]);
		}

		return s.toString();
	}
	
	
	public static int byteArrayToInt(byte[] b, int offset) {
        int value = 0;
        for (int i = 0; i < 4; i++) {
            int shift = (4 - 1 - i) * 8;
            value += (b[i + offset] & 0x000000FF) << shift;
        }
        return value;
    }
	
	public static byte[] netfind() {
		
		byte[] mac_filter = {(byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF};
		ByteArrayOutputStream requestBytes = new ByteArrayOutputStream();
		DataOutputStream request = new DataOutputStream(requestBytes);
		
		try {
			
				request.writeBytes("netfind");
				request.writeByte(0x00);			// Version
	
				request.write(mac_filter);
				request.flush();
				
				} catch (Exception e) {
					e.printStackTrace();
			}
		
		return requestBytes.toByteArray();
		
	}
	
	
	public static Module netdiscover(DatagramPacket packet) {
		
		byte[] data = packet.getData();	
		
		if((new String(data, 0, 11)).equals("netdiscover")) {
			
			String hostname = new String(Arrays.copyOfRange(data, 21, 53)).trim();
			String standort = new String(Arrays.copyOfRange(data, 53, 85)).trim();
			int uptime = byteArrayToInt(Arrays.copyOfRange(data, 17, 21),0);
			String mac = encode((Arrays.copyOfRange(data, 11, 17)));
			
			
			return new Module(hostname, standort, uptime, packet.getAddress(), packet.getPort(), mac);
				
		}
		else {
			return null;
		}
			

	}

}
