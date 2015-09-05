# another virtual host using mix of IP-, name-, and port-based configuration
#
server {
    listen       8081;
 
    location / {
        root   html;
		autoindex on;
    }
		
	#### firmware for esp8266
	location /update/fw {
	#TODO: builden aus settings!!
		alias C:/dev/ESP8266/sming/DobbyESP/out/firmware;
		autoindex on;
		index none;
	}
	
	location /update/web {
		#TODO: builden aus settings!!
		alias C:/dev/ESP8266/sming/DobbyESP/web/build;
		autoindex on;
		index none;
	}
}