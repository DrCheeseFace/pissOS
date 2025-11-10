#include <kernel/tty.h>
#include <kernel/vga.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t *const)0xB8000;
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

void terminal_init(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}

	printf("tty init OK\n");
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, enum vga_color color, size_t x,
			 size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll(void)
{
	const void *second_line_ptr = (const void *)(VGA_MEMORY + VGA_WIDTH);

	static const size_t bytes_to_shift_up =
		(VGA_HEIGHT - 1) * VGA_WIDTH * sizeof(*VGA_MEMORY);
	memcpy((void *)VGA_MEMORY, second_line_ptr, bytes_to_shift_up);
}

void terminal_delete_last_line(void)
{
	uint16_t *ptr = VGA_MEMORY;
	ptr += VGA_WIDTH * (VGA_HEIGHT - 1);

	for (int x = 0; x < (int)VGA_WIDTH; x++) {
		*ptr = 0;
		ptr++;
	}
}

void terminal_putchar(char c)
{
	unsigned char uc = c;

	if (terminal_row == VGA_HEIGHT) {
		terminal_scroll();
		terminal_delete_last_line();
		terminal_row = VGA_HEIGHT - 1;
	}

	switch (uc) {
	case '\n': // newline
		terminal_column = 0;
		terminal_row++;
		break;
	default: // regular ass character
		terminal_putentryat(uc, terminal_color, terminal_column,
				    terminal_row);
		terminal_column++;

		if (terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			terminal_row++;
		}
		break;
	}
}

void terminal_write(const char *data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char *data)
{
	terminal_write(data, strlen(data));
}
