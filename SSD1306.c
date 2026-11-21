#include "settings.h"
#include "SSD1306Var.h"

void screen_command(uint8_t cmd) {
    I2C_address(SSD1306_ADD, I2C_WRITE);
    I2C_write(0x00, 0);
    I2C_write(cmd, 0);
    I2C_stop();
}


void screen_data(uint8_t data) {
    I2C_address(SSD1306_ADD, I2C_WRITE);
    I2C_write(0x40, 0);
    I2C_write(data, 0);
    I2C_stop();
}


void screen_init() { 
    I2C_address(SSD1306_ADD, I2C_WRITE);
    for (uint8_t i = 0; i < SSD1306_CMD_COUNT; i++) {
        I2C_write(ssd1306Commands[i], 0);  ///< Send initialization command
    }
    I2C_stop();
}


void screen_draw_char(char c) {
    if ((c < 32 || c > 127) && (c != 176) && (c < 192)) {
        c = 32;  ///< If character is unsupported, use a space
    }

    uint8_t minus = 32;  ///< Default offset for printable characters (ASCII 32-127)
    
    if (c == 176) {
        minus = 80;  ///< Special character 176 uses a different offset
    } else if (c > 191) {
        minus = 95;  ///< Characters from 192-255 start from index 97
    }

    // Draw the character using the font data
    for (uint8_t i = 0; i < 5; i++) {
        uint8_t line = font[c - minus][i];  ///< Get the corresponding font byte
        screen_data(line);  ///< Display the character line by line
    }

    screen_data(0x00);  ///< Add space between characters
}


void screen_contrast(uint8_t contrast) {
    if (contrast > 0x3f) {
        contrast = 0x3f;  ///< Ensure contrast does not exceed maximum
    }
    screen_command(0x81);  ///< Send command to set contrast
    screen_command(contrast);  ///< Set the contrast value
}


void screen_clear() {
    for (int page = 0; page < 8; page++) {
        screen_command(0xB0 + page);
        screen_command(0x00);
        screen_command(0x10);

        for (int column = 0; column < 128; column++) {
            screen_data(0x00);
        }
    }

}


void screen_draw_text(char *text, uint8_t max_length) {
    uint8_t length = 0;
    while (*text && length < max_length) {
        screen_draw_char(*text);  ///< Draw each character in the string
        text++;
        length++;
    }
    while (length < max_length) {
        screen_draw_char(' ');  ///< Fill remaining space with spaces
        length++;
    }
}


void screen_write_text(char *text, uint8_t line, uint8_t start_pixel) {
    uint8_t max_chars = (128 - start_pixel) / 6;  ///< Calculate max characters per line
    screen_command(0xB0 | line);  ///< Set the page (line)
    screen_command(0x10 | (start_pixel >> 4));  ///< Set high byte of column address
    screen_command(0x00 | (start_pixel & 0x0F));  ///< Set low byte of column address
    screen_draw_text(text, max_chars);  ///< Draw the text
}


uint8_t calculate_start_pixel(char *text, /*uint8_t max_length,*/ alignment_t alignment) {
    uint8_t text_length = 0;
    while (text[text_length] != '\0' /*&& text_length < max_length*/) {
        text_length++;
    }

    uint8_t text_width = text_length * 6;  ///< Calculate the width of the text in pixels
    switch (alignment) {
        case ALIGN_CENTER:
            return (128 - text_width) / 2;  ///< Center the text
        case ALIGN_RIGHT:
            return (128 - text_width);  ///< Right-align the text
        case ALIGN_LEFT:
        default:
            return 0;  ///< Left-align the text
    }
}


void screen_write_text_aligned(char *text, uint8_t line, alignment_t alignment) {
    uint8_t start_pixel = calculate_start_pixel(text, alignment);  ///< Calculate start pixel
   screen_write_text(text, line, start_pixel);
}


void screen_write_formatted_text(const char *format, uint8_t line, alignment_t alignment, ...) {
    char textStorage[MAX_TEXT_LENGTH];  ///< Buffer for storing formatted text
    va_list args;  ///< Variable argument list

    va_start(args, alignment);  ///< Start reading variable arguments
    vsnprintf(textStorage, MAX_TEXT_LENGTH, format, args);  ///< Format the text
    va_end(args);  ///< End reading variable arguments

    screen_write_text_aligned(textStorage, line, alignment);  ///< Write formatted text to display
}

