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

//declaration section
void signUp();
int i=0, key=0, val=0, OptionCounter=0;

struct credentials{
	char fname[30], lname[30], username[80], email[60], password[50];
	struct dob{
		int day, month, year;
	}d;
};

char *SignIn_Options[] =
{
	"Sign In",
	"Sign Up",
	"Forgot Password",
	"Contact Admin",
	"Exit",
};
//------------------- functions ------------------
//1. funtion for c_muse art
void ThemeArt()
{
	system("cls");
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

//2. funtion for SignIn menu page
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

//3. funtion for using arrow keys
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
//				SignIn();
				break;
			case 1:
				signUp();
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
	}
}

//4. funtion for SIGN UP
void signUp()
{
	FILE *p;
	struct credentials input;
	
	p=fopen("Credentials.txt","a+");
	if(p==NULL)
	{
		printf("\nSignUp failed!");
		exit(0);
	}
	printf("\n************************Sign Up************************");
	printf("\n\n--------Full Name--------\n");
	printf("\nEnter First Name = ");
	scanf("%s",input.fname);
	printf("Enter Last Name = ");
	scanf("%s",input.lname);
	printf("\n\n--------Date of Birth--------\n");
	printf("Enter year = ");
	scanf("%d",&input.d.year);
	printf("Enter Month = ");
	scanf("%d",&input.d.month);
	printf("Enter Day = ");
	scanf("%d",&input.d.day);
	printf("\n\n--------Email and Password--------\n");	
	printf("\nEnter Email = ");
	scanf("%s",input.email);
	printf("\nEnter Password = ");
	scanf("%s",input.password);
	
	fwrite(&input,sizeof(input),1,p);
	fclose(p);
		printf("\nAccount has been created successfully. Please Sign in to access.");
		val=0;
	getch();
}

//===================================
//#0 MAIN FUNCTION
int main()
{
	while(1)
	{
		SignInMenu();
		KeyPress();
	}
	
return 0;
}