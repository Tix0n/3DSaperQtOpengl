CC = g++ -Wall -Werror -Wextra
GCOV = -fprofile-arcs -ftest-coverage
INC_CHECK = $(shell pkg-config --cflags --libs check)
LFLAGS = -fprofile-arcs -ftest-coverage

OS = $(shell uname)

QMAKE = qmake
INSTALL_DIR = bin

ifeq ($(OS), Linux)
	BUILD_DIR = build-ex-Desktop_Qt_5_15_2_GCC_64bit
	APP_DIR=
	LIBS = -lm -lpthread -lsubunit -lrt -lGL -lGLU -lglut
else ifeq ($(OS), Darwin)
	BUILD_DIR = build-ex-Desktop_Qt_5_15_2_GCC_64bit
	APP_DIR=3DSaper.app/Contents/MacOS
	LIBS = -framework OpenGL -framework GLUT
endif

all: clean build install

build:
	$(QMAKE) -o $(BUILD_DIR)/Makefile ./3DSaper/3dSaperQtOpengl.pro
	make -C $(BUILD_DIR) LFLAGS="$(LIBS)"

rebuild: clean all

install: build
	mkdir -p $(INSTALL_DIR)
	install $(BUILD_DIR)$(APP_DIR)/3dSaperQtOpengl $(INSTALL_DIR)/3dSaper

uninstall:
	rm -rf $(INSTALL_DIR)

dist:
	tar -cf 3DSaper.tar 3DSaper Makefile

linter:
	clang-format -i --style=Google ./3DSaper/*.h ./3DSaper/*.cpp
	clang-format -n --style=Google ./3DSaper/*.h ./3DSaper/*.cpp

clean:
	rm -rf build* $(INSTALL_DIR) 
	rm -f *.info  *.tar gcov_report *.o
