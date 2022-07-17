#include "./utils.h"

#include "../uart_protocol_driver/uart_protocol_driver.h"

char* binary_to_string(uint8_t binary){
    char *string = malloc(9);
    for(uint8_t i = 0; i < 8; i++){
        uint8_t bitStatus = (binary >> (i)) & 1;
        if (bitStatus){
            string[7-i] = '1';
        }else{
            string[7-i] = '0';
        }
    }
    string[8] = '\0';

    return string;
}

char* int_to_string(int number){
    uint8_t negative = 0;
    // if (number < 0){
    //     negative = 1;
    //     number *= -1;
    // }
    
    
    // get digit count
    uint8_t count = 0;   // variable declaration 
    int number_temp = number; 
    while(number_temp!=0)  {  
        number_temp=number_temp/10;
        count++;  
    }    

    char *string = malloc(count+negative+1);
    
    for(uint8_t i = 0+negative; i < count+negative; i++){
        uint32_t divisor = 1;
        for(uint8_t j = 0; j < count-1-i; j++){
            divisor = divisor * 10;
        }

        string[i] = 48 + (number / divisor);
        number =  number % divisor;
    }  

    if(negative){
        string[0] = '-';
    }

    string[count] = '\0';

    return string;
}


int n_tu(int number, int count)
{
    int result = 1;
    while(count-- > 0)
        result *= number;

    return result;
}

/*** Convert float to string ***/
char* float_to_string(float f){
    long long int length, length2, i, number, position, sign;
    float number2;


    sign = -1;   // -1 == positive number
    if (f < 0){
        sign = '-';
        f *= -1;
    }

    number2 = f;
    number = f;
    length = 0;  // Size of decimal part
    length2 = 0; // Size of tenth

    /* Calculate length2 tenth part */
    while( (number2 - (float)number) != 0.0 && !((number2 - (float)number) < 0.0) ){
         number2 = f * (n_tu(10.0, length2 + 1));
         number = number2;

         length2++;
    }

    /* Calculate length decimal part */
    for (length = (f > 1) ? 0 : 1; f > 1; length++)
        f /= 10;


    position = length;
    length = length + 1 + length2;
    number = number2;
    if (sign == '-'){
        length++;
        position++;
    }
    
    uint8_t adjustment = 0;
    if(length2 == 0){
        adjustment = 1;
    }


    char *string = malloc(length+1-adjustment);


    for (i = length; i >= 0 ; i--){
        if (i == (length))
            string[i] = '\0';
        else if(i == (position) && length2 != 0)
            string[i] = '.';
        else if(i == (position) && length2 == 0)
            continue;
        else if(sign == '-' && i == 0)
            string[i] = '-';
        else
        {
            string[i] = (number % 10) + '0';
            number /=10;
        }
    }

    string[length-adjustment] = '\0';

    return string;
}