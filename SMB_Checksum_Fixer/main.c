#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static uint16_t readBigShort(FILE *file) {
	uint16_t c1 = (uint16_t)getc(file) << 8;
	uint16_t c2 = (uint16_t)getc(file);
	return (c1 | c2);
}

static void writeBigShort(FILE *file, uint16_t value) {
	putc((value >> 8), file);
	putc((value), file);
}


/*
* Used in F-Zero GX
*/
static void fixChecksumLSB(const char *filepath) {
	FILE *file = fopen(filepath, "rb+");
	if (file == NULL) {
		printf("File not found: %s\n", filepath);
		return;
	}

	// Get filesize
	fseek(file, 0, SEEK_END);
	uint32_t filesize = ftell(file);
	rewind(file);

	// Size of data after checksum
	uint32_t datasize = filesize - 0x42;

	// Seek to just past existing checksum (0x44)
	fseek(file, 0x42, SEEK_SET);

	// Allocate memory to store data
	uint8_t *bytes = (uint8_t *)malloc(sizeof(uint8_t) * datasize);
	fread(bytes, sizeof(uint8_t), datasize, file);

	// Loop through bytes and calculate the checksum
	uint16_t generatorPolynomial = 0x8408;
	uint16_t checksum = 0xFFFF;

	for (uint32_t i = 0; i < datasize; i++) {
		checksum ^= bytes[i] & 0xFF;

		for (int j = 0; j < 8; j++) {
			if ((checksum & 1) == 1) {
				checksum >>= 1;
				checksum ^= generatorPolynomial;
			}
			else {
				checksum >>= 1;
			}
		}
	}

	// Flip bits in checksum
	checksum ^= 0xFFFF;

	// Write in the new checksum value (at 0x40)
	fseek(file, 0x40, SEEK_SET);
	writeBigShort(file, checksum);

	// Cleanup
	fclose(file);
	free(bytes);

}

/*
* Used in SMB 2
*/
static void fixChecksumMSB(const char *filepath) {
	FILE *file = fopen(filepath, "rb+");
	if (file == NULL) {
		printf("File not found: %s\n", filepath);
		return;
	}

	// Get filesize
	fseek(file, 0, SEEK_END);
	uint32_t filesize = ftell(file);
	rewind(file);

	// Size of data after checksum
	uint32_t datasize = filesize - 0x42;

	// Seek to just past existing checksum (0x44)
	fseek(file, 0x42, SEEK_SET);

	// Allocate memory to store data
	uint8_t *bytes = (uint8_t *)malloc(sizeof(uint8_t) * datasize);
	fread(bytes, sizeof(uint8_t), datasize, file);

	// Loop through bytes and calculate the checksum
	uint16_t generatorPolynomial = 0x1021;
	uint16_t checksum = 0xFFFF;

	for (uint32_t i = 0; i < datasize; i++) {
		checksum ^= (bytes[i] << 8);

		for (int j = 0; j < 8; j++) {
			if (checksum & 0x8000) {
				checksum <<= 1;
				checksum ^= generatorPolynomial;
			}
			else {
				checksum <<= 1;
			}
			checksum &= 0xFFFF;
		}
	}

	// Flip bits in checksum
	checksum ^= 0xFFFF;

	// Write in the new checksum value (at 0x40)
	fseek(file, 0x40, SEEK_SET);
	writeBigShort(file, checksum);

	// Cleanup
	fclose(file);
	free(bytes);

}

int main(int argc, char * argv[]) {

	for (int i = 1; i < argc; i++) {
		fixChecksumMSB(argv[i]);
	}

	return 0;
}