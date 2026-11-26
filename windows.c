#include "settings.h"
#include "windowsVar.h"

void show_window(){
    
    screen_write_formatted_text("%s  T%s", 0, ALIGN_RIGHT, Joystick.Button ? "Manual mode": " Auto mode ", ws_data.Device_ID);
    
    screen_write_formatted_text("---------------------", 1, ALIGN_CENTER);

    screen_write_formatted_text("W.S: crc ", 2, ALIGN_LEFT, weather_station.status);

    screen_write_formatted_text(" T.C: noa", 2, ALIGN_RIGHT, top_controller.status);

    screen_write_formatted_text("---------------------", 3, ALIGN_CENTER);

    screen_write_formatted_text("Am 60V 0A ", 4, ALIGN_LEFT);
    screen_write_formatted_text(" Em 23V 3A", 4, ALIGN_RIGHT);

    screen_write_formatted_text("---------------------", 5, ALIGN_CENTER);
    
    screen_write_formatted_text("Target:", 6, ALIGN_LEFT);
    screen_write_formatted_text("a.%3d° e.%2d°", 6, ALIGN_RIGHT, ws_data.AZ / 100, ws_data.EL / 100 );

    screen_write_formatted_text("Current:", 7, ALIGN_LEFT);
    screen_write_formatted_text("a.%3d° e.%2d°", 7, ALIGN_RIGHT, sensor_data.AZ, sensor_data.EL);  
}
