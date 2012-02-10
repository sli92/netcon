<?php
header('Content-Type: text/html; charset=utf-8'); // sorgt für die korrekte Kodierung
header('Cache-Control: must-revalidate, pre-check=0, no-store, no-cache, max-age=0, post-check=0'); // ist mal wieder wichtig wegen IE


            $out = "";
			$host="localhost";
			$port=5004;
			$timeout=30;
			
			
			$sk=fsockopen($host,$port,$errnum,$errstr,$timeout);
			
			if(!is_resource($sk)) {
				
				exit("Server nicht erreichbar");
		
			}
			else {
					
					fputs($sk, "GET\n", 4);
					fputs($sk, "list\n", 5);
					
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
						
								
					echo($out);	
				
			}
			
			fclose($sk);	


?>

