package lib;

import java.util.ArrayList;
import java.util.List;


public class ModuleStock {
	
	private List<Module> list;
	
	public ModuleStock() {
		setList(new ArrayList<Module>());
	}
	
	public List<Module> getList() {
		return list;
	}
	private void setList(List<Module> list) {
		this.list = list;
	}
	

	public void equalize(List<Module> compList) {
		
		List<Module> delList = new ArrayList<Module>();
			
			for(Module mod : compList) {
				
				if(this.list.contains(mod)) {
						
					if(this.list.get(this.list.indexOf(mod)).getTimeToLive() < 3)
						this.list.get(this.list.indexOf(mod)).incTimeToLive();
					
					this.list.get(this.list.indexOf(mod)).setUptime(mod.getUptime());
					this.list.get(this.list.indexOf(mod)).setFound(true);
					
					if(this.list.get(this.list.indexOf(mod)).getThread() == null)
						this.list.get(this.list.indexOf(mod)).startThread();
						
				}
						
				else {
					
					mod.setFound(true);
					mod.startThread();
					this.list.add(mod);
	
				}
		
			}
			
			for(Module mod : this.list) {
				
				if(mod.isFound() == false) {

					mod.decTimeToLive();
					
					if(mod.getTimeToLive() == 0) {
						delList.add(mod);
					}
				}
				
				mod.setFound(false);
			}
			
			if(delList.size() > 0) {
				
				this.list.removeAll(delList);
				
			}
		
	}

}
