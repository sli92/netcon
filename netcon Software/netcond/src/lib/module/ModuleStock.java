package lib.module;

import java.util.ArrayList;
import java.util.List;

public class ModuleStock {
	
	private List<Module> list;		// module list
	
	public ModuleStock() {
		setList(new ArrayList<Module>());
	}
	
	public List<Module> getList() {
		return list;
	}
	private void setList(List<Module> list) {
		this.list = list;
	}
	
	// function to match (temporary) "compList" and (long-term) "list"
	public void equalize(List<Module> compList) {
		
		List<Module> delList = new ArrayList<Module>();		// modules to delete
			
			// search "compList" for modules
			for(Module mod : compList) {
				
				// "list" already contains module
				if(this.list.contains(mod)) {
					
					// reset timetolive if it's smaller than 3
					if(this.list.get(this.list.indexOf(mod)).getTimeToLive() < 3)
						this.list.get(this.list.indexOf(mod)).incTimeToLive();
					
					// renew module's uptime
					this.list.get(this.list.indexOf(mod)).setUptime(mod.getUptime());
					
					// set module as found
					this.list.get(this.list.indexOf(mod)).setFound(true);
					
					// restart module thread if required
					if(this.list.get(this.list.indexOf(mod)).getThread() == null)
						this.list.get(this.list.indexOf(mod)).startThread();
						
				}
				
				// "list" doesn't contain module
				else {
					
					// set module as found
					mod.setFound(true);
					
					// start TCP-connection thread (ModuleConnector)
					mod.startThread();
					
					// add module to "list"
					this.list.add(mod);
	
				}
		
			}
			
			// search "list" for modules 
			for(Module mod : this.list) {
				
				// module unlocated? (found == false)
				if(mod.isFound() == false) {
					
					// decrement timetolive 
					mod.decTimeToLive();
					
					// mark module for deletion 
					if(mod.getTimeToLive() == 0) {
						delList.add(mod);
					}
				}
				
				// set module to be unlocated (found = false)
				mod.setFound(false);
			}
			
			// delete marked modules
			if(delList.size() > 0) {
				
				this.list.removeAll(delList);
				
			}
		
	}

}
