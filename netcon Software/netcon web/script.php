<div class="txt_left">
	
<?php

			$host="localhost";
			$port=6789;
			$timeout=30;
			
			$sk=fsockopen($host,$port,$errnum,$errstr,$timeout);
			
			if(!is_resource($sk)) {
				
				echo("<img src=\"kreuz.gif\" alt=\"test2\"><br>");
				exit("Fehler: Keine Verbindung");
				
				
			}
			else {
			
					$zeichen = fgetc($sk);
				
					while($zeichen != "\n") {
					
						if($zeichen == '#') {
							
							$out .= "<br>";
						}
						elseif($zeichen == '*'){
							
							$out .= "<img src=\"haken.gif\" alt=\"test\">";
						}
						else{
							
							$out .= $zeichen;
						}
						
						
						$zeichen = fgetc($sk);
					}
					
					if($out == "") {
					
						echo("<img src=\"haken.gif\" alt=\"test2\"> Verbindung hergestellt...");
						echo("Keine Module gefunden");
					
					}
				
				echo($out);
				
				
				
				
			}
			
			fclose($sk);
			
		
		
?>
</div> 	
