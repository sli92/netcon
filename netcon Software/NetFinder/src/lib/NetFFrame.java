package lib;

/*
 * NetFFrame(lib) Version 0.01 Build 111026
 */

import java.awt.BorderLayout;

import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTable;

public class NetFFrame extends JFrame {

	private static final long serialVersionUID = 1L;
	private JScrollPane scrollTable;
	private JTable table;
	private ModuleStock list;

	public NetFFrame(ModuleStock list) {

		this.list = list;

		setTitle("NetFinder");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		createWidgets();
		addWidgets();

		pack();

		setLocationRelativeTo(null);
	}

	private void createWidgets() {

		table = new JTable(list);
		table.setAutoCreateRowSorter(true);
		table.setRowSelectionAllowed(false);
		table.setFocusable(false);

		scrollTable = new JScrollPane(table);
	

	}

	private void addWidgets() {

		getContentPane().setLayout(new BorderLayout(5, 5));
		getContentPane().add(BorderLayout.CENTER, scrollTable);
	}

}


