// Use strict
"use strict";


// Classes

// SMAZ class
class Smaz {

	// Public
	
		// Initialize
		static initialize() {
		
			// Set instance to invalid
			Smaz.instance = Smaz.INVALID;
		
			// Return promise
			return new Promise(function(resolve, reject) {
		
				// Set settings
				var settings = {
				
					// On abort
					"onAbort": function(error) {
					
						// Prevent on abort from being called again
						delete settings["onAbort"];
						
						// Reject error
						reject("Failed to download resource");
					}
				};
				
				// Create SMAZ instance
				smaz(settings).then(function(instance) {
				
					// Prevent on abort from being called
					delete settings["onAbort"];
				
					// Set instance
					Smaz.instance = instance;
					
					// Resolve
					resolve();
				});
			});
		}
		
		// Compress
		static compress(input) {
		
			// Check if instance doesn't exist
			if(typeof Smaz.instance === "undefined")
			
				// Set instance
				Smaz.instance = smaz();
		
			// Check if instance is invalid
			if(Smaz.instance === Smaz.INVALID)
			
				// Return operation failed
				return Smaz.OPERATION_FAILED;
			
			// Allocate and fill memory
			var inputBuffer = Smaz.instance._malloc(input["length"] * input["BYTES_PER_ELEMENT"]);
			Smaz.instance["HEAPU8"].set(input, inputBuffer / input["BYTES_PER_ELEMENT"]);
			
			// Check if getting compress size failed
			var compressSize = Smaz.instance._compressSize(inputBuffer, input["length"] * input["BYTES_PER_ELEMENT"]);
			
			if(compressSize === Smaz.instance._invalidSize()) {
			
				// Clear memory
				Smaz.instance["HEAPU8"].fill(0, inputBuffer / input["BYTES_PER_ELEMENT"], inputBuffer / input["BYTES_PER_ELEMENT"] + input["length"]);
				
				// Free memory
				Smaz.instance._free(inputBuffer);
			
				// Return operation failed
				return Smaz.OPERATION_FAILED;
			}
			
			// Initialize result to compress size
			var result = new Uint8Array(compressSize);
			
			// Allocate and fill memory
			var resultBuffer = Smaz.instance._malloc(result["length"] * result["BYTES_PER_ELEMENT"]);
			
			// Check if compressing failed
			if(Smaz.instance._compress(resultBuffer, result["length"] * result["BYTES_PER_ELEMENT"], inputBuffer, input["length"] * input["BYTES_PER_ELEMENT"]) === Smaz.C_FALSE) {
			
				// Clear memory
				Smaz.instance["HEAPU8"].fill(0, resultBuffer / result["BYTES_PER_ELEMENT"], resultBuffer / result["BYTES_PER_ELEMENT"] + result["length"]);
				Smaz.instance["HEAPU8"].fill(0, inputBuffer / input["BYTES_PER_ELEMENT"], inputBuffer / input["BYTES_PER_ELEMENT"] + input["length"]);
				
				// Free memory
				Smaz.instance._free(resultBuffer);
				Smaz.instance._free(inputBuffer);
			
				// Return operation failed
				return Smaz.OPERATION_FAILED;
			}
			
			// Get result
			result = new Uint8Array(Smaz.instance["HEAPU8"].subarray(resultBuffer, resultBuffer + result["length"]));
			
			// Clear memory
			Smaz.instance["HEAPU8"].fill(0, resultBuffer / result["BYTES_PER_ELEMENT"], resultBuffer / result["BYTES_PER_ELEMENT"] + result["length"]);
			Smaz.instance["HEAPU8"].fill(0, inputBuffer / input["BYTES_PER_ELEMENT"], inputBuffer / input["BYTES_PER_ELEMENT"] + input["length"]);
			
			// Free memory
			Smaz.instance._free(resultBuffer);
			Smaz.instance._free(inputBuffer);
			
			// Return result
			return result;
		}
		
		// Decompress
		static decompress(input) {
		
			// Check if instance doesn't exist
			if(typeof Smaz.instance === "undefined")
			
				// Set instance
				Smaz.instance = smaz();
		
			// Check if instance is invalid
			if(Smaz.instance === Smaz.INVALID)
			
				// Return operation failed
				return Smaz.OPERATION_FAILED;
			
			// Allocate and fill memory
			var inputBuffer = Smaz.instance._malloc(input["length"] * input["BYTES_PER_ELEMENT"]);
			Smaz.instance["HEAPU8"].set(input, inputBuffer / input["BYTES_PER_ELEMENT"]);
			
			// Check if getting decompress size failed
			var decompressSize = Smaz.instance._decompressSize(inputBuffer, input["length"] * input["BYTES_PER_ELEMENT"]);
			
			if(decompressSize === Smaz.instance._invalidSize()) {
			
				// Clear memory
				Smaz.instance["HEAPU8"].fill(0, inputBuffer / input["BYTES_PER_ELEMENT"], inputBuffer / input["BYTES_PER_ELEMENT"] + input["length"]);
				
				// Free memory
				Smaz.instance._free(inputBuffer);
			
				// Return operation failed
				return Smaz.OPERATION_FAILED;
			}
			
			// Initialize result to decompress size
			var result = new Uint8Array(decompressSize);
			
			// Allocate and fill memory
			var resultBuffer = Smaz.instance._malloc(result["length"] * result["BYTES_PER_ELEMENT"]);
			
			// Check if decompressing failed
			if(Smaz.instance._decompress(resultBuffer, result["length"] * result["BYTES_PER_ELEMENT"], inputBuffer, input["length"] * input["BYTES_PER_ELEMENT"]) === Smaz.C_FALSE) {
			
				// Clear memory
				Smaz.instance["HEAPU8"].fill(0, resultBuffer / result["BYTES_PER_ELEMENT"], resultBuffer / result["BYTES_PER_ELEMENT"] + result["length"]);
				Smaz.instance["HEAPU8"].fill(0, inputBuffer / input["BYTES_PER_ELEMENT"], inputBuffer / input["BYTES_PER_ELEMENT"] + input["length"]);
				
				// Free memory
				Smaz.instance._free(resultBuffer);
				Smaz.instance._free(inputBuffer);
			
				// Return operation failed
				return Smaz.OPERATION_FAILED;
			}
			
			// Get result
			result = new Uint8Array(Smaz.instance["HEAPU8"].subarray(resultBuffer, resultBuffer + result["length"]));
			
			// Clear memory
			Smaz.instance["HEAPU8"].fill(0, resultBuffer / result["BYTES_PER_ELEMENT"], resultBuffer / result["BYTES_PER_ELEMENT"] + result["length"]);
			Smaz.instance["HEAPU8"].fill(0, inputBuffer / input["BYTES_PER_ELEMENT"], inputBuffer / input["BYTES_PER_ELEMENT"] + input["length"]);
			
			// Free memory
			Smaz.instance._free(resultBuffer);
			Smaz.instance._free(inputBuffer);
			
			// Return result
			return result;
		}
		
		// Operation failed
		static get OPERATION_FAILED() {
		
			// Return operation failed
			return null;
		}
	
	// Private
	
		// Invalid
		static get INVALID() {
		
			// Return invalid
			return null;
		}
		
		// C false
		static get C_FALSE() {
		
			// Return C false
			return 0;
		}
}


// Supporting fuction implementation

// Check if document doesn't exist
if(typeof document === "undefined") {

	// Create document
	var document = {};
}

// Check if module exports exists
if(typeof module === "object" && module !== null && "exports" in module === true) {

	// Exports
	module["exports"] = Smaz;
}
