<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
 <head>
  <title>netcon web</title>
  
  <link href="index.css" rel="stylesheet" type="text/css">
  
  <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.min.js"></script>
    
  <script type="text/javascript">
  
  function refreshContent() {
			setRequest();
		}
		
		$(document).ready(function(){
			window.setInterval("refreshContent()", 7000);
			refreshContent();
		});	
  
  
	var request = false;

	// Request senden
	function setRequest() {
		
		// Request erzeugen
		if (window.XMLHttpRequest) {
			request = new XMLHttpRequest(); 
		} 

		// überprüfen, ob Request erzeugt wurde
		if (!request) {
			alert("Kann keine XMLHTTP-Instanz erzeugen");
			return false;
		} else {
			var url = "connector.php";
			// Request öffnen
			request.open('post', url, true);
			// Request senden
			request.send(null);
			// Request auswerten
			request.onreadystatechange = interpretRequest;
		}
	}

	// Request auswerten
	function interpretRequest() {
		switch (request.readyState) {
			// wenn der readyState 4 und der request.status 200 ist, dann ist alles korrekt gelaufen
			case 4:
				if (request.status != 200) {
					alert("Der Request wurde abgeschlossen, ist aber nicht OK\nFehler:"+request.status);
				} else {
					var content = request.responseText;
					// den Inhalt des Requests in das <div> schreiben
					document.getElementById('content').innerHTML = content;
				}
				break;
			default:
				break;
		}
	}
  
  </script>
 </head>
 <body>
 
 <center>
<div> 
	
	<br>
	<img src="./pictures/logo.png" alt="error"></img>
	<h3> Module im Netzwerk</h3>

</div>
	
  	<div id="Content">
  		<div id="content">test</div>
  	</div>
</center>
 
  
 </body>
</html>