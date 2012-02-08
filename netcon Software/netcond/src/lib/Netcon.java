package lib;

import enums.GET;


public class Netcon {

	// Anzahl der Geraete
	public static byte[] netcon(GET c, String device) {
		
		String message = null;
		
		try {
			
			message = "GET ";
			
			switch(c) {
			
			case devicecount: 
				message += "devicecount";
				break;
				
			case devicetype:
				message += "type " + device;
				break;
				
			case value:
				message += "value "+ device;
				break;
				
			case dtype:
				message += "dtype "+ device;
				break;
			
			}
			
			message += "\r\n";
			
			} 
		
		catch (Exception e) {
				e.printStackTrace();
		}
	
		return message.getBytes();	
		
	}
	
}
