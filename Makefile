CFLAGS = -Wall -Wextra -Werror #-g3 -fsanitize=address
SOURCES = my_zsh.c 
TARGET = my_zsh
CC = gcc

$(TARGET) : $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^
.PHONY:
	fclean

fclean:
	@rm -f $(OBJECTS) $(TARGET)