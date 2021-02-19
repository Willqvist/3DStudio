Antingen kan du öppna programmet i Clion och sekevera det därifrån eller följa följande steg.

steg 1. Kör följande kommando i en cmd.
cmake . -B ./build  -G "Visual Studio 16 2019"

PS. Byt ut "16 2019" mot vilken version du har.

steg 2. kör följande kommando i en cmd från nuvarande katalog.
cmake --build ./build --target GLProject --config Release

steg 3. Dra in ./res katalogen in i ./build/Release/

steg 4. Kör programmet GLProject.exe

Då du skriver in sökvägar till obj filer/bilder så skriv res/namn_på_fil.obj så att programmet vet att den ska leta i res katalogen.

