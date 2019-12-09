// ´íÎóÏûÏ¢

#include "defs.h"

void error(const char* message,int errl);
void error(const char* message,int errl) {

	printf("Error(line  %d ):  %s\n", errl,message);
	/*
	if ( strstr(message,"zero")!=NULL) {
		cout << "can't continue because of divide by zero" << endl;
		exit(1);
	}
	*/
}
#pragma once
