#include "myAPI.h"
#include "math.h"
extern ADC_HandleTypeDef hadc1;

#define R_rt 10000.0
#define TemperatureOffset -3.4
float calculate_to_temperature(int ad){
    float v=((float)ad)/4095*3.3;
    float r=(v*R_rt)/(3.3-v);//10kΩ
    float t=(1/(log(r/R_rt)/3950.0+(1/(273.15+23.0))))-273.15+TemperatureOffset;
    return  t;
}

int measure_the_temperature(void){
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1,1000);
   return HAL_ADC_GetValue(&hadc1);
}

