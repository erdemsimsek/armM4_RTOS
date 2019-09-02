
int __svc(0x00) svc_service_add(int x, int y);
int __svc(0x01) svc_service_sub(int x, int y);
int __svc(0x02) svc_service_mul(int x, int y);
int __svc(0x03) svc_service_div(int x, int y);

void SVC_Handler_C(unsigned int * svc_args);

int x,y,z;

int main(void)
{
	x = 1;
	y = 5;
	z = svc_service_add(x,y);
	
	
	x = 9;
	y = 2;
	z = svc_service_sub(x,y);
	
	x = 3;
	y = 4;
	z = svc_service_mul(x,y);
	
	x = 12;
	y = 6;
	z= svc_service_div(x,y);
	
}

__asm void SVC_Handler(void)
{
	TST LR, #4 //Line 1
	ITE EQ // Line 2
	MRSEQ R0, MSP // Line 3
	MRSNE R0, PSP // Line 4
	
	B __cpp(SVC_Handler_C)
}

void SVC_Handler_C(unsigned int * svc_args)
{
	unsigned int svc_number;
	
	svc_number = ((char *)svc_args[6])[-2]; // -2 is used to get first btye which represents SVC Number
	
	switch(svc_number){
		case 0:
			/* Parameter are stored in R0 and R1 */ 
			svc_args[0] = svc_args[0] + svc_args[1];
			break;
		
		case 1:
			svc_args[0] = svc_args[0] - svc_args[1];
			break;
		
		case 2:
			svc_args[0] = svc_args[0] * svc_args[1];
			break;
		
		case 3:
			svc_args[0] = svc_args[0] / svc_args[1];
			break;
		
		default:
			break;
	}
}





