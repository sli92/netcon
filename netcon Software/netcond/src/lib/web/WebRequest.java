package lib.web;

import java.util.List;

import lib.module.Module;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

public class WebRequest {

	@SuppressWarnings("unchecked")
	public static JSONObject get(String request, List<Module> moduleList) {
		
		JSONObject modulJSON = new JSONObject();
		JSONArray modullisteJSON = new JSONArray();
		
		if(request.equals("list")) {
			
			if(!(moduleList.isEmpty())) {
				
				for(Module mod : moduleList) {
					
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

	public static String set(String request, List<Module> moduleList) {


		return new String("SET nicht implementiert");

	}

}
