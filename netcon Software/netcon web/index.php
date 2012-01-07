<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
       "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
<title>Netcon</title>
<link href="netcon.css" rel="stylesheet" type:"text/css">



<script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.min.js"></script>
	<script type="text/javascript">

		function refreshContent() {
			$("[name=hallo]").load("script.php");
		}
		
		$(document).ready(function(){
			window.setInterval("refreshContent()", 7000);
			refreshContent();
		});

</script>
</head>
<body>

<div name="hallo" id="Hintergrund" >
	
		

	
</div>

</body>
</html>