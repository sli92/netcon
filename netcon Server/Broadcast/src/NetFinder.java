
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class NetFinder {
	
	public static void main(String[] args) {
		
		
		System.out.println("Press return to get module list");
		
		InputStreamReader isr = new InputStreamReader(System.in);
		BufferedReader br = new BufferedReader(isr);
		
		try{
			br.readLine();
		} catch(Exception e) {
			
		}
		
		// send broadcast
		try{
			Broadcast.sendBroadcast("bcnetcon", 6799);
		} catch (IOException e) {
			e.printStackTrace();
			}
		
		String message = null;
		
		while(true) {
			
			message = null;
				try {
					message = Broadcast.receiveBroadcast(6800);
				}
				catch (IOException e) {
					System.out.println(e.toString());
				}
				
				System.out.println(message);
				
		}
				
	}

}
