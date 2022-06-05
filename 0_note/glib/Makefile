mainloop-1:
	gcc -g `pkg-config --cflags --libs glib-2.0 gthread-2.0` mainloop-1.c -o mainloop-1
mainloop-2:
	gcc -g `pkg-config --cflags --libs glib-2.0 gthread-2.0` mainloop-2.c -o mainloop-2
mainloop-3:
	gcc -g `pkg-config --cflags --libs glib-2.0 gthread-2.0` mainloop-3.c -o mainloop-3
mainloop-4:
	gcc -g `pkg-config --cflags --libs glib-2.0 gthread-2.0` mainloop-4.c -o mainloop-4
socket:
	gcc -g `pkg-config --cflags --libs glib-2.0 gthread-2.0 gio-2.0` socket-server.c -o socket-server
	gcc -g `pkg-config --cflags --libs glib-2.0 gthread-2.0 gio-2.0` socket-client.c -o socket-client
gobject:
	gcc -g `pkg-config --cflags --libs glib-2.0 gthread-2.0 gobject-2.0` gobject.c -o gobject
gmodule:
	gcc -o libmodule.so -fPIC -shared `pkg-config --libs --cflags glib-2.0 gmodule-2.0` gmodule_module.c
	gcc -o gmodule `pkg-config --libs --cflags glib-2.0 gmodule-2.0` gmodule.c
