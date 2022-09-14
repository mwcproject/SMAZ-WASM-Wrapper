# Library parameters
NAME = "SMAZ"
VERSION = "0.0.23"
CC = "em++"
CFLAGS = -Wall -Wno-writable-strings -D NDEBUG -Oz -finput-charset=UTF-8 -fexec-charset=UTF-8 -funsigned-char -ffunction-sections -fdata-sections -D VERSION=$(VERSION) -I "./smaz-master/" -s MODULARIZE=1 --memory-init-file=0 -s ABORTING_MALLOC=0 -s ALLOW_MEMORY_GROWTH=1 --closure 1 -flto -fno-rtti -fno-exceptions -s NO_FILESYSTEM=1 -s DISABLE_EXCEPTION_CATCHING=1 -s EXPORTED_FUNCTIONS="['_malloc', '_free']" -s EXPORT_NAME="smaz"
LIBS =
SRCS = "./main.cpp" "./smaz-master/smaz.c"
PROGRAM_NAME = $(subst $\",,$(NAME))

# Make WASM
wasm:
	$(CC) $(CFLAGS) -s WASM=1 -s ENVIRONMENT=web -o "./$(PROGRAM_NAME).js" $(SRCS) $(LIBS)
	cat "./main.js" >> "./$(PROGRAM_NAME).js"
	rm -rf "./dist"
	mkdir "./dist"
	mv "./$(PROGRAM_NAME).js" "./$(PROGRAM_NAME).wasm" "./dist/"

# Make asm.js
asmjs:
	$(CC) $(CFLAGS) -s WASM=0 -s ENVIRONMENT=web -o "./$(PROGRAM_NAME).js" $(SRCS) $(LIBS)
	cat "./main.js" >> "./$(PROGRAM_NAME).js"
	rm -rf "./dist"
	mkdir "./dist"
	mv "./$(PROGRAM_NAME).js" "./dist/"

# Make npm
npm:
	$(CC) $(CFLAGS) -s WASM=1 -s BINARYEN_ASYNC_COMPILATION=0 -s SINGLE_FILE=1 -o "./wasm.js" $(SRCS) $(LIBS)
	$(CC) $(CFLAGS) -s WASM=0 -s BINARYEN_ASYNC_COMPILATION=0 -s SINGLE_FILE=1 -o "./asm.js" $(SRCS) $(LIBS)
	echo "\"use strict\"; try { module[\"exports\"] = require(\"@nicolasflamel/smaz-react\");} catch(error) { try { module[\"exports\"] = require(\"@nicolasflamel/smaz-native\");} catch(error) { const smaz = (typeof WebAssembly !== \"undefined\") ? require(\"./wasm.js\") : require(\"./asm.js\");" > "./index.js"
	cat "./main.js" >> "./index.js"
	echo "}}" >> "./index.js"
	rm -rf "./dist"
	mkdir "./dist"
	mv "./index.js" "./wasm.js" "./asm.js" "./dist/"

# Make clean
clean:
	rm -rf "./$(PROGRAM_NAME).js" "./$(PROGRAM_NAME).wasm" "./index.js" "./wasm.js" "./asm.js" "./dist" "./smaz-master" "./master.zip"

# Make dependencies
dependencies:
	wget "https://github.com/antirez/smaz/archive/master.zip"
	unzip "./master.zip"
	rm "./master.zip"
