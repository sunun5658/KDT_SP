# gcc 옵션
CFLAGS = -Wall -O -g
CC = gcc


# Target 지정
TARGET_NAME = toy_project
TARGET_DIR = ./bin/

#소스파일 위치
DIRS = ./ ./system ./ui/ ./web_server/

TARGET = $(addprefix $(TARGET_DIR),$(TARGET_NAME))
SRCS = $(foreach dir, $(DIRS), $(wildcard $(dir)*.c))
OBJS = $(SRCS: .c = .o)

#헤더파일 -I 인클루드
INC_DIR = $(foreach dir, $(DIRS),-I$(dir))

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(INC_DIR) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) $(INC_DIR) -o $@ -c $<

clean :
	@ rm -f $(TARGET) $(OBJS)
