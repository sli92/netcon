
import java.io.IOException;

public class Module {
	
	public static void main(String[] args) {
		
		String message = null;
		
		while(true) {
			message = null;
				try {
					message = Broadcast.receiveBroadcast(6799);
				}
				catch (IOException e) {
					System.out.println(e.toString());
				}
				
				if(message.indexOf("bcnetcon") != -1)
				{
					
					System.out.println("Broadcast erhalten - Sende Antwort...");
					
					try{
						
						Broadcast.sendBroadcast("Messmodul meldet sich", 6800 );
					} catch (IOException e) {
						e.printStackTrace();
						}	
				}
				
				
				
			
		}	
	}
}
