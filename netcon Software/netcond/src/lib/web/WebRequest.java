package lib.web;

import java.util.List;

import lib.module.Module;

public class WebRequest {

	public static String get(String request, List<Module> moduleList) {
		
		String answer = null;
		
		if(request.equals("list")) {
			
			if(!(moduleList.isEmpty())) {
				
				answer = "";
				int i;
				
				for (Module mod : moduleList) {
					
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

	public static String set(String request, List<Module> moduleList) {


		return new String("SET nicht implementiert");

	}

}
