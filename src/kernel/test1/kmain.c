#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t makecolor(enum vga_color fg, enum vga_color bg) { return fg | bg << 4; }
static inline uint16_t makevgachar(unsigned char uc, uint8_t color) { return (uint16_t)uc | (uint16_t)color << 8; }

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len]) // null char = 0 = false
		len++;
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t tty_row;
size_t tty_col;
uint8_t tty_color;
uint16_t* tty_buff;

void tty_init(void)
{
	tty_row = 0;
	tty_col = 0;
	tty_color = makecolor(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
	tty_buff = (uint16_t*) 0xb8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			tty_buff[index] = makevgachar(' ', tty_color);
		}
	}
}

void tty_setcolor(uint8_t color)
{
	tty_color = color;
}

void tty_putvgachar(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	tty_buff[index] = makevgachar(c, color);
}

void tty_printc(char c)
{
	if (c == '\n')
	{
		tty_row++;
		tty_col = 0;
	}
	else if (c == '\r')
		tty_col = 0;
	else
		tty_putvgachar(c, tty_color, tty_col, tty_row);
	if (tty_col++ == VGA_WIDTH)
	{
		tty_col = 0;
		if (tty_row++ == VGA_HEIGHT) tty_row = 0;
	}
}

void tty_printd(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		tty_printc(data[i]);
}

void tty_prints(const char* str)
{
	tty_printd(str, strlen(str));
}

void kmain()
{
	tty_init();
	tty_prints("Hello World!\n");
}
