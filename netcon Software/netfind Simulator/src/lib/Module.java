package lib;

/*
 * Module(lib) Version 0.01 Build 111111
 */

import java.net.InetAddress;

public class Module {
	
	private String hostname;
	private String ip;
	private int port;
	private int timeToLive;
	private boolean found;
	
	public Module(String hostname, InetAddress ip, int port) {
		
		setHostname(hostname);
		setIp(ip.toString().replace("/", ""));
		setPort(port);
		setTimeToLive(3);
		setFound(false);
		
	}
	
	public Module(byte data[], InetAddress ip, int port) {
		setHostname("DUMMY");
		setIp(ip.toString().replace("/", ""));
		setPort(port);
		setTimeToLive(3);
		setFound(false);
	}
	
	public String getHostname() {
		return hostname;
	}
	private void setHostname(String hostname) {
		this.hostname = hostname;
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
	
	public boolean equals(Object o) {
		
		Module mod = (Module) o;
		
		if (getHostname().matches(mod.getHostname()))
			return true;
		
		return false;
	}
	
	public int hashCode() {
		
		return port * super.hashCode();
	}
	
	
}
