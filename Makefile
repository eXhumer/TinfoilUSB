TinfoilUSB-MinGW64-Static:
	x86_64-w64-mingw32-g++ -I/mingw64/include/ -L/mingw64/lib/ -o $@ main.cpp types.cpp -lusb-1.0 -static

TinfoilUSB-MinGW64-Shared:
	x86_64-w64-mingw32-g++ -I/mingw64/include/ -L/mingw64/lib/ -o $@ main.cpp types.cpp -lusb-1.0
