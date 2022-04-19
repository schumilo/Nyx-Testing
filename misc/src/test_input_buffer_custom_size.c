#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include "nyx.h"
#include "helper.h"

int main(int argc, char** argv){

	uint32_t bitmap_size, ijon_bitmap_size, payload_buffer_size;
	uint8_t* trace_buffer = allocate_page_aligend_buffer(DEFAULT_COVERAGE_BITMAP_SIZE);

	dump_mappings();
	get_host_config(&bitmap_size, &ijon_bitmap_size, &payload_buffer_size);
	set_agent_config(true, (uintptr_t)trace_buffer, false, (uintptr_t)NULL, false, DEFAULT_COVERAGE_BITMAP_SIZE, 0, 0);

	kAFL_payload* payload_buffer = allocate_input_buffer(payload_buffer_size);

	hprintf("payload_buffer_size -> %d\n", payload_buffer_size);
	memset(payload_buffer, 0x0, payload_buffer_size);

	kAFL_hypercall(HYPERCALL_KAFL_USER_FAST_ACQUIRE, 0);

	uint32_t first_data_idx = 0;
	uint32_t last_data_idx = payload_buffer_size-NYX_INPUT_BUFFER_HEADER_SIZE-1;

	hprintf("first byte: %d\n", payload_buffer->data[first_data_idx]);
	hprintf("last byte: %d\n", payload_buffer->data[last_data_idx]);

	hassert(payload_buffer->data[first_data_idx] == 0xEF && payload_buffer->data[last_data_idx] == 0xDF, "test failed (first and last byte != 0xEF)");
	
	payload_buffer->data[first_data_idx] = 0xCC;
	payload_buffer->data[last_data_idx] == 0xBC;

	kAFL_hypercall(HYPERCALL_KAFL_RELEASE, 0);

	return 0;
}
