package lib;

/*
 * ModuleStock(lib) Version 0.01 Build 111111
 */

import java.util.ArrayList;
import java.util.List;

import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.TableModel;


public class ModuleStock implements TableModel {
	
	private List<Module> list;
	
	private List<TableModelListener> tableListener = new ArrayList<TableModelListener>();
	
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
						
					this.list.get(this.list.indexOf(mod)).setFound(true);
						
				}
						
				else {
					mod.setFound(true);
					this.list.add(mod);
	
					for(TableModelListener l : tableListener) {
						l.tableChanged(new TableModelEvent(this));
					}
				}
		
			}
			
			for(Module mod : this.list) {
				
				if(mod.isFound() == false) {
					
					mod.decTimeToLive();
					
					if(mod.getTimeToLive() == 0)
						delList.add(mod);
				}
				
				mod.setFound(false);
			}
			
			if(delList.size() > 0) {
				this.list.removeAll(delList);
				
				for(TableModelListener l : tableListener) {
					l.tableChanged(new TableModelEvent(this));
				}
			}
		
	}

	@Override
	public void addTableModelListener(TableModelListener l) {
		
		tableListener.add(l);
	}

	@Override
	public Class<?> getColumnClass(int columnIndex) {
		switch(columnIndex) {
		case 0: return String.class;
		case 1: return String.class;
		case 2: return Integer.class;
		default: return null;
		}
	}

	@Override
	public int getColumnCount() {
		// TODO Auto-generated method stub
		return 3;
	}

	@Override
	public String getColumnName(int columnIndex) {
		
		switch(columnIndex) {
		case 0: return "Hostname";
		case 1: return "IP";
		case 2: return "Port";
		default: return null;
		}
	}

	@Override
	public int getRowCount() {
		return list.size();
	}

	@Override
	public Object getValueAt(int rowIndex, int columnIndex) {
		
		Module mod = this.list.get(rowIndex);
		switch(columnIndex) {
		case 0: return mod.getHostname();
		case 1: return mod.getIp();
		case 2: return mod.getPort();
		default: return null;
		}
		
	}

	@Override
	public boolean isCellEditable(int arg0, int arg1) {
		return false;
	}

	@Override
	public void removeTableModelListener(TableModelListener l) {
		tableListener.remove(l);
		
	}

	@Override
	public void setValueAt(Object arg0, int arg1, int arg2) {
		
	}
	
}
