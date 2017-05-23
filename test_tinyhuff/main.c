#define TINYHUFF_IMPL
#include "../tinyhuff.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( )
{
	// allocate scratch memory for building shared keysets
	int scratch_bytes = TH_SCRATCH_MEMORY_BYTES;
	void* scratch_memory = malloc( scratch_bytes );
	thKey compress;
	thKey decompress;

	// the data to compress
	const char* string = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras nec faucibus leo. Praesent risus tellus, dictum ut ipsum vitae, fringilla elementum justo. Sed placerat, mauris ac elementum rhoncus, dui ipsum tincidunt dolor, eu vehicula ipsum arcu vitae turpis. Vivamus pulvinar odio non orci sodales, at dictum ex faucibus. Donec ornare a dolor vel malesuada. Donec dapibus, mauris malesuada imperdiet hendrerit, nisl dui rhoncus nisi, ac gravida quam nulla at tellus. Praesent auctor odio vel maximus tempus. Sed luctus cursus varius. Morbi placerat ipsum quis velit gravida rhoncus. Nunc malesuada urna nisl, nec facilisis diam tincidunt at. Aliquam condimentum nulla ac urna feugiat tincidunt. Nullam semper ullamcorper scelerisque. Nunc condimentum consectetur magna, sed aliquam risus tempus vitae. Praesent ornare id massa a facilisis. Quisque mollis tristique dolor. Morbi ut velit quis augue placerat sollicitudin a eu massa.";
	int bytes = strlen( string ) + 1;

	// construct compression and decompression shared keyset
	int ret = thBuildKeys( &compress, &decompress, string, bytes, scratch_memory );
	if ( !ret )
	{
		printf( "thBuildKeys failed: %s", th_error_reason );
		return -1;
	}

	// do compression
	ret = thCompress( &compress, string, bytes, scratch_memory, scratch_bytes );
	if ( !ret )
	{
		printf( "thCompress failed: %s", th_error_reason );
		return -1;
	}

	// compute size of data after it would be compressed
	int compressed_bits = thCompressedSize( &compress, string, bytes );

	// do decompression
	char* buf = (char*)malloc( bytes );
	thDecompress( &decompress, scratch_memory, compressed_bits, buf, bytes );

	if ( strcmp( buf, string ) )
	{
		printf( "String mismatch. Something bad happened" );
		return -1;
	}

	return 0;
}
