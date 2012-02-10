<?php
            $out = "";
			$host="localhost";
			$port=5004;
			$timeout=30;
			
			
			$sk=fsockopen($host,$port,$errnum,$errstr,$timeout);
			
			if(!is_resource($sk)) {
				
				echo("<img src=\"./pictures/error.gif\" alt=\"error\">");
				exit(" <b> Fehler </b>: Verbindung zu Daemon kann nicht hergestellt werden");
		
			}
			else {
					
					//fputs($sk, "test\n", 5);
					
					$zeichen = fgetc($sk);
					
					while($zeichen != "\n") {
					
						if($zeichen == '#') {
							
							$out .= "<br>";
							
						}
						elseif($zeichen == '*'){
							
							$out .= "<img src=\"./pictures/module.gif\" alt=\"module\">";
						}
						else{
							
							$out .= $zeichen;
						}
						
						
						$zeichen = fgetc($sk);
					}
					
					if($out == "") {
					
						echo("<img src=\"./pictures/ok.gif\" alt=\"ok\"> Daemon laeuft.Verbindung hergestellt...");
						echo("<br>Keine Module gefunden");
					
					}
				
				echo($out);	
			}
			
			fclose($sk);	
?>
