#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t *)0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static enum vga_color terminal_color;
static uint16_t *terminal_buffer;

void terminal_initialize(void)
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
}

void terminal_setcolor(enum vga_color color)
{
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, enum vga_color color, size_t x,
			 size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll(int line)
{
	int loop;
	char c;

	for (loop = line * (VGA_WIDTH * 2) + 0xB8000; loop < (int)VGA_WIDTH * 2;
	     loop++) {
		c = *(char *)loop;
		*(char *)(loop - (VGA_WIDTH * 2)) = c;
	}
}

void terminal_delete_last_line()
{
	int x, *ptr;

	for (x = 0; x < (int)VGA_WIDTH * 2; x++) {
		ptr = (int *)((char *)0xB8000 +
			      (VGA_WIDTH * 2) * (VGA_HEIGHT - 1) + x);
		*ptr = 0;
	}
}

void terminal_putchar(char c)
{
	int line;
	unsigned char uc = c;

	terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT) {
			for (line = 1; line <= (int)VGA_HEIGHT - 1; line++) {
				terminal_scroll(line);
			}
			terminal_delete_last_line();
			terminal_row = VGA_HEIGHT - 1;
		}
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
