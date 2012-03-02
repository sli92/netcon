package lib.module;

import java.net.InetAddress;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import program.threads.ModuleConnector;

public class Module {
	
	private String hostname;
	private String location;
	private int uptime;
	private int devicecount;		// how many devices the module contains
	private int type[];				// devices' types  
	private String value[];			// devices' values 
	private String minValue[];		// devices' min values
	private String maxValue[];		// devices' max values
	private String dtype[];			// values' types
	
	private String ip;
	private int port;
	private String mac;		
	
	private ModuleConnector thread = null;	// TCP-connection thread to collect values

	public Module(String hostname, String location, InetAddress ip, int port, String mac) {
		
		setHostname(hostname);
		setLocation(location);
		setUptime(0);
		
		setDevicecount(0);
		setType(null);
		setValue(null);
		setMaxValue(null);
		setMinValue(null);
		setDtype(null);
		
		setIp(ip.toString().replace("/", ""));
		setPort(port);
		setMac(mac);
		
	}
	
	public String getHostname() {
		return hostname;
	}
	private void setHostname(String hostname) {
		this.hostname = hostname;
	}
	
	public String getLocation() {
		return location;
	}
	private void setLocation(String location) {
		this.location = location;
	}
	
	public int getUptime() {
		return uptime;
	}
	public void setUptime(int uptime) {
		this.uptime = uptime;
	}
	
	public int getDevicecount() {
		return devicecount;
	}
	public void setDevicecount(int count) {
		this.devicecount = count;
	}
	
	public int getType(int i) {
		return type[i];
	}
	public void setType(int type[]) {
		this.type = type;
	}

	public String getValue(int i) {
		return value[i];
	}
	public void setValue(String value[]) {
		this.value = value;
	}
	
	public String getMinValue(int i) {
		return minValue[i];
	}

	public void setMinValue(String minValue[]) {
		this.minValue = minValue;
	}

	public String getMaxValue(int i) {
		return maxValue[i];
	}

	public void setMaxValue(String maxValue[]) {
		this.maxValue = maxValue;
	}
	
	public String getDtype(int i) {
		return dtype[i];
	}
	public void setDtype(String dtype[]) {
		this.dtype = dtype;
	}
	

	public String getIp() {
		return ip;
	}
	private void setIp(String ip) {
		this.ip = ip;
	}
	public int getPort() {
		return port;
	}
	private void setPort(int port) {
		this.port = port;
	}
	public String getMac() {
		return mac;
	}
	private void setMac(String mac) {
		this.mac = mac;
	}
	
	
	public ModuleConnector getThread() {
		return thread;
	}
	public void startThread() {
		
		this.thread = new ModuleConnector(this);
	}
	
	@SuppressWarnings("unchecked")
	public JSONObject getJSON() {
		
		JSONObject obj = new JSONObject();
		JSONArray deviceList = new JSONArray();
		
		obj.put("name", getHostname());
		obj.put("ip", getIp());
		obj.put("port", getPort());
		obj.put("location", getLocation());
		obj.put("uptime", getUptime());
		
		// pseudocode time !!!
		for(int i = 0; i < getDevicecount(); i++)
		{
			JSONObject dev = new JSONObject();
			dev.put("id", i);
			dev.put("min", getMinValue(i));
			dev.put("max", getMaxValue(i));
			dev.put("value", getValue(i));
			dev.put("type", getType(i));
			dev.put("dtype", getDtype(i));
			deviceList.add(dev);
		}
		
		obj.put("devicelist", deviceList);
		
		return obj;
		
	}
	
	@Override
	public boolean equals(Object o) {
		
		Module mod = (Module) o;
		
		if (getHostname().matches(mod.getHostname()))
			return true;
		
		return false;
	}
	
	@Override
	public int hashCode() {
		
		return port * super.hashCode();
	}
	
}
