package lib;

import java.net.InetAddress;

public class Module {
	
	private String hostname;
	private String standort;
	private int uptime;
	
	private String ip;
	private int port;
	private String mac;

	private int timeToLive;
	private boolean found;
	
	public Module(String hostname, String standort, int uptime, InetAddress ip, int port, String mac) {
		
		setHostname(hostname);
		setStandort(standort);
		setUptime(uptime);
		
		setIp(ip.toString().replace("/", ""));
		setPort(port);
		setMac(mac);
		
		
		setTimeToLive(3);
		setFound(false);
		
	}
	
	public String getHostname() {
		return hostname;
	}
	private void setHostname(String hostname) {
		this.hostname = hostname;
	}
	public String getStandort() {
		return standort;
	}
	private void setStandort(String standort) {
		this.standort = standort;
	}
	public int getUptime() {
		return uptime;
	}
	public void setUptime(int uptime) {
		this.uptime = uptime;
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
