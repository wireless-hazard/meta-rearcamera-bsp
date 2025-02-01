all: main.o framebuffer_ssd1306.o
	$(CXX) -o mpu_test main.o framebuffer_ssd1306.o

main.o: main.cpp framebuffer_ssd1306.h
	$(CXX) main.cpp -o main.o -c

framebuffer_ssd1306.o: framebuffer_ssd1306.cpp framebuffer_ssd1306.h font_8x8_definitions.h
	$(CXX) framebuffer_ssd1306.cpp -o framebuffer_ssd1306.o -c

clean:
	rm -rf *.o *~ mpu_test
