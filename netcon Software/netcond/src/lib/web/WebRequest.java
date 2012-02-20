package lib.web;

import lib.module.Module;
import lib.module.ModuleStock;

public class WebRequest {

	public static String get(String request, ModuleStock list) {
		
		String answer = null;
		
		if(request.equals("list")) {
			
			if(!(list.getList().isEmpty())) {
				
				answer = "";
				int i;
				
				for (Module mod : list.getList()) {
					
					answer += "*" + mod.getHostname() + ", Standort: " + mod.getStandort() + ", online seit " + 
					mod.getUptime() / 100 + " s"+ " (" + mod.getIp() + ":" + mod.getPort() + ") #";
					
					for(i = 0; i<mod.getDevicecount(); i++) {
						
						answer += "Device " + i  + ": "+ mod.getValue()[i] + " V #";
					}

				}

				
			}
			else
				answer = "Kein Modul gefunden";
			
		}
		else {
			
			answer = null;
		}
		
		return answer;
		
	}

	public static String set(String request, ModuleStock list) {


		return new String("SET nicht implementiert");

	}

}
