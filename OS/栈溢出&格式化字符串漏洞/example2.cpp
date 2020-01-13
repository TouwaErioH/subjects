#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void function(int a,int b,int c)
{
	char buffer1[5];
	char buffer2[10];
	int *ret;
	
	ret = (int*)buffer1 + 12;
	(*ret) += 8;
 } 

int main()
{
	int x;
	x=1;
	function(1,2,3);
	x=0;
	printf("%d\n",x);
 } 
