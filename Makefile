CFLAGS = -Wall -g `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`

grid_test: bifidGUI.c
	$(CC) -o bifidGUI bifidGUI.c $(CFLAGS) $(LDFLAGS)


