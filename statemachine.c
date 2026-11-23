#include "settings.h"
#include "statemachineVar.h"

void states(){
    //1. read weather station
    ReadWeatherStation();
    //2. read top controller
    ReadTopController();
    ADC_read_all(); //read all data from local sensors
    //3. show sensor data and check joystick?
    show_window();  
    //4. spin both motors
        
    //5. send answer to weather station.
    
    //6. repeat until task is completed
}
