$(document).ready(function(){ 
		
		initTable();
		updateTable();
		window.setInterval(updateTable, 1000);
}); 

function changeTimeformat(seconds) {
    var days = Math.floor(seconds / 86400);
    var hours = Math.floor((seconds % 86400) / 3600);
    var minutes = Math.floor(((seconds % 86400) % 3600) / 60);
    var seconds = ((seconds % 86400) % 3600) % 60;

    return days + "d " + hours + ":" + minutes + ":" + seconds;

}

function getTypeunit(typeNum) {
    switch (typeNum) {
        case 1:
            return 'V';
            break;
    }
}

function initTable() {
	
	return $('#example').dataTable( {

    		"bRetrieve": true,
			"bJQueryUI": true,
			"bProcessing": true,
        	
			"oLanguage": {
                "sUrl": "./DataTables/media/dataTables.german.txt"
            }
		});   
}

function updateTable() {
	
	var oTable = initTable();
	
	$.get('connector.php', function(result){
		
        if(result.indexOf("error") >= 0) {

            if(result == "error") { 
            	oTable.fnClearTable();
            	oTable.fnAddData ( [
        	
        			"Keine Verbindung",
        			"","","","","",""
        			
        			 ]
        		);
            }
            else {
            	oTable.fnClearTable();
            }

            return;
        }
        
        oTable.fnClearTable();
        
        var json = $.parseJSON(result);    
        
        for(var i = 0; i < json.modulliste.length; i++) {
        	
        	var modul = json.modulliste[i];
        	
        	for(var z = 0; z < modul.devicelist.length; z++) {
        		
        		var device = modul.devicelist[z];
        		var percentage = parseInt(device.value / (device.max - device.min) * 100);
        		var bar = '<table><td style="width: 100px; border: 1px solid red;padding: 0px 0px;">' 
        					+ '<div style="background-color:red;width:' + percentage + 'px; height: 1em;"></div></td></table>';
        		var unit = getTypeunit(device.type);
        		
        		oTable.fnAddData ( [
        	
        			modul.name+"#"+device.id,
        			modul.location,
        			changeTimeformat(parseInt(modul.uptime / 100)),
        			"["+unit+"]",
        			device.min,
        			bar + device.value,
        			device.max,
        			
        			 ]
        		);
        		
        		
        	}
        	
        	
        }           
   
    });
}


