package lib;

import enums.GET;


public class Netcon {

	// Anzahl der Geraete
	public static byte[] netcon(GET c, String device) {
		
		String message = null;
		
		try {
			
			message = "GET ";
			
			switch(c) {
			
			case name:
				message += "name";
				break;
			
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
			
			//System.out.println(message);
			
			} 
		
			
		
		catch (Exception e) {
				e.printStackTrace();
		}
	
		return message.getBytes();	
		
	}
	
}
