// Header files
#include <cstddef>
#include <cstdint>
#include <cstring>

// Check if using Emscripten
#ifdef __EMSCRIPTEN__

	// Header files
	#include <emscripten.h>
	#include "smaz.h"

// Otherwise
#else

	// Header files
	extern "C" {
		#include "smaz.h"
	}
#endif

using namespace std;


// Definitions

// Check if using Emscripten
#ifdef __EMSCRIPTEN__

	// Export
	#define EXPORT extern "C"

// Otherwise
#else

	// Export
	#define EXPORT static

	// Emscripten keepalive
	#define EMSCRIPTEN_KEEPALIVE
#endif


// Constants

// Invalid size
static const size_t INVALID_SIZE = SIZE_MAX;

// Size scaling factor
static const uint8_t SIZE_SCALING_FACTOR = 2;


// Function prototypes

// Invalid size
EXPORT size_t EMSCRIPTEN_KEEPALIVE invalidSize();

// Get compress size
EXPORT size_t EMSCRIPTEN_KEEPALIVE compressSize(const uint8_t *input, size_t inputSize);

// Compress
EXPORT bool EMSCRIPTEN_KEEPALIVE compress(uint8_t *result, size_t resultSize, const uint8_t *input, size_t inputSize);

// Get decompress size
EXPORT size_t EMSCRIPTEN_KEEPALIVE decompressSize(const uint8_t *input, size_t inputSize);

// Deompress
EXPORT bool EMSCRIPTEN_KEEPALIVE decompress(uint8_t *result, size_t resultSize, const uint8_t *input, size_t inputSize);


// Supporting function implementation

// Invalid size
size_t invalidSize() {

	// Return invalid size
	return INVALID_SIZE;
}

// Compress size
size_t compressSize(const uint8_t *input, size_t inputSize) {

	// Go through incrementing result sizes
	for(size_t i = 0; inputSize + i * SIZE_SCALING_FACTOR >= inputSize + i && inputSize + i != SIZE_MAX;) {
	
		// Initialize result
		uint8_t result[inputSize + i];
		
		// Get result size
		size_t resultSize = smaz_compress(const_cast<char *>(reinterpret_cast<const char *>(input)), inputSize, reinterpret_cast<char *>(result), sizeof(result));
		
		// Clear memory
		explicit_bzero(result, sizeof(result));
		
		// Check if the result size was large enough
		if(resultSize != sizeof(result) + 1) {
			
			// Return result size
			return resultSize;
		}
		
		// Update size
		i = i ? i * SIZE_SCALING_FACTOR : 1;
	}
	
	// Return invalid size
	return INVALID_SIZE;
}

// Compress
bool compress(uint8_t *result, size_t resultSize, const uint8_t *input, size_t inputSize) {

	// Return if compressing the input was successful
	return static_cast<unsigned int>(smaz_compress(const_cast<char *>(reinterpret_cast<const char *>(input)), inputSize, reinterpret_cast<char *>(result), resultSize)) == resultSize;
}

// Decompress size
size_t decompressSize(const uint8_t *input, size_t inputSize) {

	// Go through incrementing result sizes
	for(size_t i = 0; inputSize + i * SIZE_SCALING_FACTOR >= inputSize + i && inputSize + i != SIZE_MAX;) {
	
		// Initialize result
		uint8_t result[inputSize + i];
		
		// Get result size
		size_t resultSize = smaz_decompress(const_cast<char *>(reinterpret_cast<const char *>(input)), inputSize, reinterpret_cast<char *>(result), sizeof(result));
		
		// Clear memory
		explicit_bzero(result, sizeof(result));
		
		// Check if the result size was large enough
		if(resultSize != sizeof(result) + 1) {
			
			// Return result size
			return resultSize;
		}
		
		// Update size
		i = i ? i * SIZE_SCALING_FACTOR : 1;
	}
	
	// Return invalid size
	return INVALID_SIZE;
}

// Decompress
bool decompress(uint8_t *result, size_t resultSize, const uint8_t *input, size_t inputSize) {

	// Return if compressing the input was successful
	return static_cast<unsigned int>(smaz_decompress(const_cast<char *>(reinterpret_cast<const char *>(input)), inputSize, reinterpret_cast<char *>(result), resultSize)) == resultSize;
}
