TARGET = execas
OBJS = execas.o
CFLAGS = -g -O2 -Wall
LDFLAGS =

execas: $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS) $(TARGET)
