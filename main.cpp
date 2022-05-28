// Header files
#include <emscripten.h>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include "smaz.h"

using namespace std;


// Constants

// Invalid size
static const size_t INVALID_SIZE = SIZE_MAX;

// Size scaling factor
static const uint8_t SIZE_SCALING_FACTOR = 2;


// Function prototypes

// Invalid size
extern "C" size_t EMSCRIPTEN_KEEPALIVE invalidSize();

// Get compress size
extern "C" size_t EMSCRIPTEN_KEEPALIVE compressSize(const uint8_t *input, size_t inputSize);

// Compress
extern "C" bool EMSCRIPTEN_KEEPALIVE compress(uint8_t *result, size_t resultSize, const uint8_t *input, size_t inputSize);

// Get decompress size
extern "C" size_t EMSCRIPTEN_KEEPALIVE decompressSize(const uint8_t *input, size_t inputSize);

// Deompress
extern "C" bool EMSCRIPTEN_KEEPALIVE decompress(uint8_t *result, size_t resultSize, const uint8_t *input, size_t inputSize);


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
