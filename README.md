## Basic BMP image filters made in C for Linux
Box blur and swiss cheese filter to be applied 

For usage, run the following in Linux CLI:</br>
`gcc MaFilters.c BmpProcessor.h BmpProcessor.o PixelProcessor.h -lm -lpthread -o ImageFilter`</br>
`./ImageFilter -i in.bmp -o out.bmp -f c`</br>
Where `c` would indicate swiss cheese filter or `b` for box filter
