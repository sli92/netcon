package lib.web;

import lib.module.Module;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import program.main.Netcond;

public class WebRequest {

	@SuppressWarnings("unchecked")
	public static JSONObject get(String request) {
		
		JSONObject modulJSON = new JSONObject();
		JSONArray modullisteJSON = new JSONArray();
		
		if(request.equals("list")) {
			
			if(!(Netcond.moduleList.isEmpty())) {
				
				for(Module mod : Netcond.moduleList) {
					
					modullisteJSON.add(mod.getJSON());
				}
				
				
				modulJSON.put("modulliste", modullisteJSON);
				return modulJSON;
				
			}
			else
				return null;
			
		}
		else {
			
			return null;
		}
		
	
		
	}

	public static String set(String request) {


		return new String("SET nicht implementiert");

	}

}
