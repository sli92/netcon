package lib.module;

import java.net.InetAddress;

import program.threads.ModuleConnector;

public class Module {
	
	private String hostname;
	private String location;
	private int uptime;
	private int devicecount;		// how many devices the module contains
	private int type[];				// devices' types  
	private String value[];			// devices' values 
	private String dtype[];			// values' types
	
	private String ip;
	private int port;
	private String mac;

	private int timeToLive;			// how long the module will be held in memory if it won't be detected
	private boolean found;			// marker wheter the module was found in round
	private int connectiontries;	// counter for TCP-connection tries
	
	private ModuleConnector thread = null;	// TCP-connection thread to collect values

	public Module(String hostname, String standort, int uptime, InetAddress ip, int port, String mac) {
		
		setHostname(hostname);
		setStandort(standort);
		setUptime(uptime);
		
		setDevicecount(0);
		setType(null);
		setValue(null);
		setDtype(null);
		
		setIp(ip.toString().replace("/", ""));
		setPort(port);
		setMac(mac);
		
		setTimeToLive(3);
		setFound(false);
		setConnectiontries(3);
		
	}
	
	public String getHostname() {
		return hostname;
	}
	private void setHostname(String hostname) {
		this.hostname = hostname;
	}
	
	public String getStandort() {
		return location;
	}
	private void setStandort(String standort) {
		this.location = standort;
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
	
	public int[] getType() {
		return type;
	}
	public void setType(int type[]) {
		this.type = type;
	}

	public String[] getValue() {
		return value;
	}
	public void setValue(String value[]) {
		this.value = value;
	}
	
	public String[] getDtype() {
		return dtype;
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
	

	public int getTimeToLive() {
		return timeToLive;
	}
	private void setTimeToLive(int timeToLive) {
		this.timeToLive = timeToLive;
	}
	public void decTimeToLive() {
		timeToLive--;
	}
	public void incTimeToLive() {
		timeToLive++;
	}
	public boolean isFound() {
		return found;
	}
	public void setFound(boolean found) {
		this.found = found;
	}
	public int getConnectiontries() {
		return connectiontries;
	}

	public void setConnectiontries(int connectiontries) {
		this.connectiontries = connectiontries;
	}
	
	
	public ModuleConnector getThread() {
		return thread;
	}
	public void startThread() {
		
		this.thread = new ModuleConnector(this);
	}
	public void restartThread() {
		
		this.thread = null;
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
