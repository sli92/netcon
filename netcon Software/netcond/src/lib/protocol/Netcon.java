package lib.protocol;

public class Netcon {

	public static byte[] netcon(NetconGET c, String device) {
		
		String message = null;
		
		try {
			
			message = "GET ";
			
			switch(c) {
			
			case uptime:
				message += "uptime";
				break;
			
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
				
			case min:
				message += "min "+ device;
				break;
				
			case max:
				message += "max "+ device;
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
