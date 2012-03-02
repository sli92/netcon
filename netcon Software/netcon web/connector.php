<?php
header('Content-Type: text/html; charset=utf-8'); 


            $out = "";
			$host="localhost";
			$port=5004;
			$timeout=30;

			$sk=fsockopen($host,$port,$errnum,$errstr,$timeout);
			
			if(!is_resource($sk)) {
				
				exit("error");
		
			}
			else {
					
					fputs($sk, "GET\n", 4);
					fputs($sk, "list\n", 5);
					
					$zeichen = fgetc($sk);
						
						while($zeichen != "\n") {

							$out .= $zeichen;
							$zeichen = fgetc($sk);
						}
						

					echo $out;
					
			}
			
			fclose($sk);	


?>

