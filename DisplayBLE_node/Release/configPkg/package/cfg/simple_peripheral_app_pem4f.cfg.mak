# invoke SourceDir generated makefile for simple_peripheral_app.pem4f
simple_peripheral_app.pem4f: .libraries,simple_peripheral_app.pem4f
.libraries,simple_peripheral_app.pem4f: package/cfg/simple_peripheral_app_pem4f.xdl
	$(MAKE) -f C:\Users\sujaj\workspace\DisplayBLE_node/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\sujaj\workspace\DisplayBLE_node/src/makefile.libs clean

