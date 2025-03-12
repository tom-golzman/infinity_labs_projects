#include <unistd.h> /* For reaad() function */
#include <termios.h> /* For terminal control */
#include <ctype.h> /* For toupper() */
#include <stdio.h> /* For printf() */
#include <stdlib.h> /* For system() */

#define ESC 27 /* ASCII value for esc key */
#define SUCCESS 0 

typedef int (*keys)(void);

int InputUsingSwitch();
int InputUsingIf();
int InputUsingLUT();
int APressed();
int TPressed();
int EscPressed();
int OtherKeys();

int main()
{
	int usr_choice = 0;
	
	printf("Enter 1 to use switch\nEnter 2 to use if/else\nEnter 3 to use LUT\n");
	scanf("%d", &usr_choice);

	system("stty -icanon -echo");
	
	switch (usr_choice)
	{
		case 1:
			InputUsingSwitch();
			break;
		case 2:
			InputUsingIf();
			break;
		case 3:
			InputUsingLUT();
			break;
	}
	
	system("stty icanon echo");

	return SUCCESS;
}

int InputUsingSwitch()
{
	char usr_input = 0;
	
	while (1)
	{
		read(STDIN_FILENO, &usr_input, 1);

		switch (toupper(usr_input))
		{
			case 'A':
				APressed();
				break;
			case 'T':
				TPressed();
				break;
			case ESC:
				EscPressed();
			default:
				break;
		}
	}
	
	return (SUCCESS);
}

int InputUsingIf()
{
	char usr_input = 0;
		
	read(STDIN_FILENO, &usr_input, 1);
	usr_input = toupper(usr_input);
	
	if(usr_input == 'A')
	{
		APressed();
	}
	
	else if (usr_input == 'T')
	{
		TPressed();	
	}
		
	return SUCCESS;	
}

int InputUsingLUT()
{
	int i = 0;
	char usr_input = 0;
	keys lut[256] = {NULL};
	
	for (i = 0; i < 256; i++)
	{
		lut[i] = OtherKeys;
	}
	
	lut['A'] = APressed;
	lut['T'] = TPressed;
	lut[ESC] = EscPressed;
		
	read(STDIN_FILENO, &usr_input, 1);
	
	while (1)
	{
		if (usr_input == ESC)
		{
			printf("Exiting program\n");
		}
		
		lut[toupper(usr_input)]();
	}
	
	return (SUCCESS);
}

int APressed(void)
{
	printf("A-pressed\n");
	
	return (SUCCESS);
}

int TPressed(void)
{
	printf("T-pressed\n");
	
	return (SUCCESS);
}

int EscPressed(void)
{
	printf("Exiting program\n");
	exit(SUCCESS);
}

int OtherKeys(void)
{
	return (SUCCESS);
}
















