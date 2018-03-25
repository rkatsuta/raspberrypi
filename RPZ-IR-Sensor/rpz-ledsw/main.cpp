
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <wiringPi.h>


int main(int argc, char *argv[]){
	if(wiringPiSetupGpio() == -1){
		return 1;
	}

	// LED and SW setting
	pinMode(5, INPUT);
	pullUpDnControl(5, PUD_UP);
	pinMode(6, INPUT);
	pullUpDnControl(6, PUD_UP);

	pinMode(17, OUTPUT);
	pinMode(18, OUTPUT);
	pinMode(22, OUTPUT);
	pinMode(27, OUTPUT);

	while(1){
		if(0==digitalRead(5) | 0==digitalRead(6)){
			digitalWrite(17, 1);
			digitalWrite(18, 1);
			digitalWrite(22, 1);
			digitalWrite(27, 1);
		}else{
			digitalWrite(17, 0);
			digitalWrite(18, 0);
			digitalWrite(22, 0);
			digitalWrite(27, 0);
		}
	}

	return 0;
}
