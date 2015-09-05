# another virtual host using mix of IP-, name-, and port-based configuration
#
server {
    listen       8081;
 
    location / {
        root   html;
        index  index.html index.htm;
    }
	
			#### firmware for esp8266
	location /update/fw {
		alias  "[FIRMWARE_DIR]";
		index  index.html index.htm;
		autoindex on;
	}
	
	location /update/web {
		alias  "[WEB_DIR]";
		index none;
		autoindex on;
	}

}