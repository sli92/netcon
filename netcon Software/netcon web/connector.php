<div class="txt_left">
	
<?php
            $out = "";
			$host="localhost";
			$port=6789;
			$timeout=30;
			
			$sk=fsockopen($host,$port,$errnum,$errstr,$timeout);
			
			if(!is_resource($sk)) {
				
				echo("<img src=\"./pictures/error.gif\" alt=\"error\"><br>");
				exit("Fehler: Keine Verbindung");
				
				
			}
			else {
			
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
					
						echo("<img src=\"./pictures/ok.gif\" alt=\"ok\"> Verbindung hergestellt...");
						echo("Keine Module gefunden");
					
					}
				
				echo($out);	
			}
			
			fclose($sk);
		
?>
</div> 	
