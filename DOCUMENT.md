
# Documentaion

# Programing

## Tools:
- [FTDI Basic Breakout - 5V/3.3V (USB to Serial TTL) - Blue Board](http://ram-e-shop.com/oscmax/catalog/product_info.php?products_id=1797)
- [FTDI Basic Breakout - 5V/3.3V (USB to Serial TTL) - Blue Board](http://ram-e-shop.com/oscmax/catalog/product_info.php?products_id=3707)
- [USBasp AVR programmer](http://ram-e-shop.com/oscmax/catalog/product_info.php?products_id=2850)

# API
- When Power on : Soft Start takes 30 second to make sure everything is stable
- When motion detected All awake and wifi connected, The data sent to Server is, [name] => *Nest12*, [msg] => alarm,[v] => *2964*,[mac] => *5C:CF:7F:A4:2C:13* ==> All data sent as A GET request with [User-Agent: ratnest] in headers :next_track_button: If the request reached the server then an internal alarm flag is activated so no other alarm will occure elese when **Reset** button is presed
- When **Test** Button pressed The data sent to Server is, [name] => Nest12, [msg] => test,[v] => 2964,[mac] => 5C:CF:7F:A4:2C:13
- When **Update** Button pressed The data sent to Server is, [name] => Nest12, [msg] => config,[v] => 2964,[mac] => 5C:CF:7F:A4:2C:13 and the data from the server should be [Link](http://7elol.com//update/config.php?name=Nest1&msg=config&v=2955&mac=5C:CF:7F:A4:2C:13)
```
{"ssid":"airlive",
"ssid_pwd":"password",
"update_server":"7elol.com",
"update_server_page":"\/update\/config.php",
"server":"7elol.com",
"server_page":"\/update\/rfid.php",
"place":"Nest12",
"ip":"192.168.0.15",
"gateway":"192.168.0.1",
"subnet":"255.255.255.0"}
```

- When **Config** Button pressed an access paint named **Rat_nest** with password **password** then the operator need to access the following [page](192.168.4.1) and follow the instructions
```
'ssid' => The Wifi SSID,
'ssid_pwd' => The Wifi Password
'update_server'=> server name like [google.com]
'update_server_page'=> The route to the page [/conf/config.aspx]
'server'=>Backend server [google.com]
'server_page'=> The route to the page [/request/index.aspx]
'place'=> The trap name 
'ip'=> Trap Ip in the lan
'gateway'=> Gateway 
'subnet'=> Subnet
```

- for all operations the buzzer will give one buzzer on operation done and twice on error 


## Programming

ESP [Programming](http://www.instructables.com/id/ESP8266-a-Complete-Beginners-Guide-IOT/) <br>
-U lfuse:w:0x62:m -U hfuse:w:0xd6:m -U efuse:w:0xfe:m <br>
(https://tosiek.pl/usbasp-v2-0-warning-cannot-set-sck-period/)
(https://jlcpcb.com)
(https://www.smart-prototyping.com)
(https://www.pcbway.com/orderonline.aspx)
