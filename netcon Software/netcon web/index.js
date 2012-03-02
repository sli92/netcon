$(document).ready(function(){
	
    window.setInterval(refreshContent, 1000);
    refreshContent();

});

function typeToString(typeNum) {
    switch (typeNum) {
        case 1:
            return "Spannung";
            break;
    }
}

function dtypeToString(typeChar) {
    switch (typeChar) {
        case 'f':
            return "Float";
            break;
    }
}

function shortUnitForType(typeNum) {
    switch (typeNum) {
        case 1:
            return 'V';
            break;
    }
}

function rectime(seconds) {
    var numdays = Math.floor(seconds / 86400);
    var numhours = Math.floor((seconds % 86400) / 3600);
    var numminutes = Math.floor(((seconds % 86400) % 3600) / 60);
    var numseconds = ((seconds % 86400) % 3600) % 60;

    return numdays + "d " + numhours + ":" + numminutes + ":" + numseconds;

}

function refreshContent() {
    $.get('connector.php', function(result){
        if(result.indexOf("error") >= 0) {
        	
            var tableHTML = '';
            if(result == "error") 
            	tableHTML += 'Keine Verbindung';
            else
            	tableHTML += 'Keine Module gefunden';
            var tab = $("#moduletable");
            tab.html(tableHTML);
            return;
        }
        var json = $.parseJSON(result);
        
        var tableHTML = '';

        tableHTML += '<tr><th>Name</th><th>Standort</th><th>Uptime</th><th>Devices</th></tr></thead>';
        
        
        for(var i = 0; i < json.modulliste.length; i++) {
            var modul = json.modulliste[i];
            

            tableHTML += '<tr><td style="width: 180px; text-align: center;">'
				+ modul.name
				+ '</td><td style="width: 120px; text-align: center;">'
				+ modul.location
				+ '</td><td style="width: 90px; text-align: center;">'
				+ rectime(parseInt(modul.uptime / 100))
				+ '</td>';
            
            tableHTML += '<td><table><tr><th>ID</th><th>Typ</th><th>Datentyp</th><th>Min</th><th>Max</th><th>Aktuell</th><th></th></tr>';
            for(var z = 0; z < modul.devicelist.length; z++) {
                var device = modul.devicelist[z];
                var unit = shortUnitForType(device.type);
                var proz = parseInt(device.value / (device.max - device.min) * 100);
                

	            tableHTML += '<tr><td style="width: 30px; text-align: right;">'
					+ device.id
					+ '</td><td style="width: 90px; text-align: center;">'
					+ typeToString(device.type)
					+ '</td><td style="width: 80px; text-align: center;">'
					+ dtypeToString(device.dtype)
					+ '</td><td style="width: 60px; text-align: right;">'
					+ device.min
					+ unit
					+ '</td><td style="width: 60px; text-align: right;">'
					+ device.max
					+ unit
					+ '</td><td style="width: 60px; text-align: right;">'
					+ device.value
					+ unit
					+ '</td><td style="width: 100px; border: 1px solid blue;"><div style="background-color: red; border: 1px solid blue; width: '
					+ proz
					+ 'px; height: 1em;"></div></td></tr>';
	                
            }
            
            tableHTML += '</table></td>';
        }
        
        
        
        var tab = $("#moduletable");
        tab.html(tableHTML);
    });
}