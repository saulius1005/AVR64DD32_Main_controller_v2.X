/* 
 * File:   SSD1306.h
 * Author: Saulius
 *
 * Created on ?e?tadienis, 2026, lapkritis 21, 22.36
 */

#ifndef SSD1306_H
#define	SSD1306_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SSD1306_ADD 0x3c
#define SSD1306_CMD_COUNT 26
#define SSD1306_SCREEN_WIDTH 12    
#define SSD1306_SCREEN_HEIGHT 64    
#define SSD1306_PAGE_COUNT (SSD1306_SCREEN_HEIGHT / 8) 
#define CHARACTER_WIDTH 6   
#define MAX_LINES 8

#define MAX_TEXT_LENGTH 50

typedef enum {
    ALIGN_LEFT,   
    ALIGN_CENTER, 
    ALIGN_RIGHT  
} alignment_t;


void screen_init();
void screen_clear();
void screen_write_formatted_text(const char *format, uint8_t line, alignment_t alignment, ...);


#ifdef	__cplusplus
}
#endif

#endif	/* SSD1306_H */

