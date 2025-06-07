/*
project:Cmuse 

*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define up 72
#define down 80
#define enterKey 13
int i=0, key=0, val=0, OptionCounter=0;

void ThemeArt()
{
	printf("      ___                             ___           ___           ___           ___     \n");
	printf("     /  /\\                           /__/\\         /__/\\         /  /\\         /  /\\    \n");
	printf("    /  /:/                          |  |::\\        \\  \\:\\       /  /:/_       /  /:/_   \n");
	printf("   /  /:/                           |  |:|:\\        \\  \\:\\     /  /:/ /\\     /  /:/ /\\  \n");
	printf("  /  /:/  ___                     __|__|:|\\:\\   ___  \\  \\:\\   /  /:/ /::\\   /  /:/ /:/_ \n");
	printf(" /__/:/  /  /\\                   /__/::::| \\:\\ /__/\\  \\__\\:\\ /__/:/ /:/\\:\\ /__/:/ /:/ /\\\n");
	printf(" \\  \\:\\ /  /:/                   \\  \\:\\~~\\__\\/ \\  \\:\\ /  /:/ \\  \\:\\/:/~/:/ \\  \\:\\/:/ /:/\n");
	printf("  \\  \\:\\  /:/                     \\  \\:\\        \\  \\:\\  /:/   \\  \\::/ /:/   \\  \\::/ /:/ \n");
	printf("   \\  \\:\\/:/                       \\  \\:\\        \\  \\:\\/:/     \\__\\/ /:/     \\  \\:\\/:/  \n");
	printf("    \\  \\::/                         \\  \\:\\        \\  \\::/        /__/:/       \\  \\::/   \n");
	printf("     \\__\\/                           \\__\\/         \\__\\/         \\__\\/         \\__\\/    \n");
	
	printf("\n\n==================== C based Music Library System ======================\n\n");
};

char *SignIn_Options[] =
{
	"Sign In",
	"Sign Up",
	"Forgot Password",
	"Contact Admin",
	"Exit",
};

void SignInMenu()
{
	system("cls");
	fflush(stdout);
	ThemeArt();
	printf("\nLogin Options:\n-----------------------------------------\n\n");
	for(i=0; i<5; i++)
	{
		if(val==i)
		{
			printf("\t-->%s<--\n",SignIn_Options[i]);
		}
		else
		{
			printf("%\t%s\n",SignIn_Options[i]);
		}
	}
}

void KeyPress()
{
	key=getch();
	if(key==224)
	{
		key=getch();
		if(key==up)
		{
			val--;
			if(val<0)
			{
				val=4;
			}
		}
		else if(key==down)
		{
			val++;
			if(val>=5)
			{
				val=0;
			}
		}	
	}
	else if(key==enterKey)
	{
		switch(val)
		{
			case 0:
				//SignIn();
				break;
			case 1:
//				SignUp();
				break;
			case 2:
//				ResetPass();
				break;
			case 3:
//				AdminContact();
				break;
			case 4:
				exit(0);
				break;
		}
		getch();
	}
}
//----------------------------------------------------------------------------------------------------------------
void SignIn()
{
	FILE *p;
	p=fopen("Credentials.txt","a+");
	if(p==NULL)
	{
		printf("\nsignin failed");
		exit(0);
	}
	
}

//===============================main function===============================================
int main()
{
	while(1)
	{
		SignInMenu();
		KeyPress();
	}
	
return 0;
}