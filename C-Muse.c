//a. Header Files
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>		//windows specific library file

//b. Macros
#define up 'w'						//macros for some most used keypresses ASCII code
#define down 's'
#define left 'a'
#define right 'd'
#define enter 13
#define esc 27

#define red     "\033[0;31m"		//color code shortcut macro		ANSI code
#define green   "\033[0;32m"
#define yellow  "\033[0;33m"
#define blue    "\033[0;34m"
#define reset   "\033[0m"			//resets the color used

//c. Function Declarations
void ThemeArt();
int Controls(int, int, char);	//for current selection, max options and horizontal or vertical

int signin();			//done
void signup();			//done
void forgotPassword();	//done
void LandingPage();
void HomePage();
void MainMenu();
void SearchSongPannel();						//Here sort by options ascending descending and artist need to add 
void Playlists();
void Profile();
void initializeAdmin();

void demoSongPlay();					//new	
//void viewAllDemoSongs();				//new
int viewSongs();		//done
int addSongs();			//done
int removeSong();		//done
int viewLikedSongs();		//done
int addToLikedSongs();		//done
int removeLikedSong();		//done

int createPlaylist();
int addSongToPlaylist();
int viewPlaylists();
int removeFromPlaylist();
int deletePlaylist();


int isValidName(const char *name);		//done for checking name is valid
int isUsernameTaken(const char *username);		//done --for checking username availability
int isEmailTaken(const char *email);		//done --for checking email availability
void loginStatus(int);		//done for checking fast switching of login state to logout state

//d. Global Variables
int i;
int logstatusCheck = 0;				//checks if its login or logout state
int invalidKeyPressFlag = 0;		//added..to know if key press is valid
int likedSongFound = 0 ;			//added...for knowing liked songs found or not
int globalSongsFound = 0;			//added...for checkings all songs found or not
int currentRole = 1;				// 1 for user 2 for artist 3 for admin 
char currentArtist[50];				//added...but not used (will be used for uploading songs if used else discarding)
int searchSong(char);

//e. Lists of Options
char *loginOptions[] =		//for showing options in loginpage
{
    "Sign In",
    "Sign Up",
    "Forgot Password",
    "Exit",
};
char *HomeBanner[] =		//for showing options in Laniding
{
    "Home",
    "Main Menu",
    "Search a Song",
    "Playlists",
    "Profile",
    "Logout",
};
char *HomeMenuList[] =		//for showing options in Home option from top banner
{
    "Play a demoSong",
//    "Recent Played",		
};
char *MainMenuList[] =		//for showing options in Main Menu option from top banner
{
    "View All Songs",
    "Playlist (SubMenu)",										
    "View All Liked Songs",
	"Like Music",											
    "Remove Liked Music",								
    //artist specific options added (new)
    blue "Upload Songs (Artist)"reset,
    green "Delete Songs (admin)" reset,
};

//new demo feature options for playing some downloaded songs from system
char *songs[] = {"Your Name.mp3", "Story About You.mp3", "Beyond the Code.mp3", "Don't Give-up Yet.mp3", "Racing In the Night.mp3"};
int totalSongs = 5;

//f. Structure
struct credentials{			//for users credential data
    char name[50];
    char username[50];
    char password[50];
    char email[50];
    char role[10];
    int roleVal;
    int id;
    struct dob{
        int day, month, year;
    }d;
};

//g. AddSongs Structure
struct song {				//for song data
    char title[100];
    char artist[50];
};
//h. PlayList Structure
struct playlist {
    char name[50];
    char songs[10][100];  // Up to 10 songs per playlist
    int songCount;
};
struct credentials logSuccess;			//added...for storing the data of the user temporarily if login is succes by that id

//1. main function(), it loads the loginpage, and switches case according to selection
int main()
{
    int selected = 0;		//to know what is the current selected option
    int currentKeyInput;
    initializeAdmin();
    flag1:
    selected = 0;
    while(1)
    {
        ThemeArt();
        for(i = 0; i<4; i++)
        {
            if(selected == i)
            {
                printf("-->%d. %s<--\n", i + 1, loginOptions[i]);
            }
            else
            {
                printf("%d. %s\n", i + 1, loginOptions[i]);
            }
        }
        if (invalidKeyPressFlag == 1)
        {
            printf(red "\nInvalid keypress! Press: wasd-(navigation), enter-(selection), esc-(exit)" reset);
            invalidKeyPressFlag = 0;
        }
        currentKeyInput = Controls(selected, 4, 'v');				//v for vertical movement
        if (currentKeyInput == enter)
        {
            switch (selected)
            {
                case 0:
                    printf(green "\n--- Sign In selected --- ");
                    printf("\nPlease wait...\n" reset);
                    Sleep(1000);
                    ThemeArt();
                    loginStatus(signin());
                    break;
                case 1:
                    printf(green "\n---  Sign Up selected ---");
                    printf("\nPlease wait...\n" reset);
                    Sleep(1000);
                    signup();
                    break;
                case 2:
                    printf(green "\n--- Forgot Password selected --");
                    printf("\nPlease wait...\n" reset);
                    Sleep(1000);
                    forgotPassword();
                    break;
                case 3:
                    printf(green "\n--- Program has terminated successfully. ---" reset);
                    exit(0);
                    break;
            }
            if (logstatusCheck == 1)		//check for login if success get out of login page loop and loop to Landing page until logout
            {
                break;
            }
            else
            {
                goto flag1;
            }
        }
        else if (currentKeyInput == esc)
        {
            printf(green "\n--- Program has terminated successfully. ---" reset);
            exit(0);
        }
        else
        {
            selected = currentKeyInput;
        }
    }
    while (logstatusCheck == 1)		//if login is true then it loops to the landingpage without return
    {
        LandingPage();
        if (logstatusCheck == 0)
        {
            goto flag1;
        }
    }
    return 0;
}

//2. sign in function(), it contains input form user, masking of password, checking of accounts and validations
int signin()
{
    int loginSuccess = 0;
    char enteredUsername[50];
    char enteredPassword[50];
    char ch;
    int char_count = 0;
    struct credentials tempLog;
    struct credentials admin_user;
    
    FILE *p;
	p = fopen("credentials.dat", "rb");				//checks if file exists or not if not no user
	if(p == NULL)
    {
        printf(red "\n\nNo user accounts found! \n" reset);
        
        strcpy(admin_user.name, "Administrator");
    	strcpy(admin_user.username, "admin");
    	strcpy(admin_user.password, "admin");
    	strcpy(admin_user.email, "admin@example.com");
    	strcpy(admin_user.role, "Admin");
    	admin_user.roleVal = 0; 
    	admin_user.id = 1;     
    	admin_user.d.day = 1;
    	admin_user.d.month = 1;
    	admin_user.d.year = 2000;
    	if (fwrite(&admin_user, sizeof(struct credentials), 1, p) != 1)
		{
        	printf("Error writing admin user to file.\n");
        	fclose(p);			//close file if writing failed
        	getch();
        	return 1;
    	}
    	fclose(p);
    	printf("Admin user 'admin' with password 'admin' created successfully!\n");
    	printf("You can now run your main music application and log in as admin.\n");
        printf("Press any key to return.\n");
        getch();
        return 0;
    }
    fclose(p);
    
    		//input section of login page
    printf(green "--- Sign In Page ---  \n" reset);
    printf("\nEnter Username: ");			
    fflush(stdin);
    scanf("%s", enteredUsername);
    printf("Enter Password: ");			
    while(1)
    {
        ch = getch();
        if(ch == enter)
        {
            enteredPassword[char_count] = '\0';
            break;
        }
        if (ch == 8)
        {
            if (char_count > 0)
            {
                char_count--;
                printf("\b \b");
            }
        }
        else if (char_count < sizeof(enteredPassword) - 1)
        {
            enteredPassword[char_count] = ch;
            printf("*");
            char_count++;
        }
    }
p = fopen("credentials.dat", "rb");		//opens file pointer to compare data for validation
    if(p == NULL)
    {
        printf(red "\n\nNo user accounts found! Please Sign Up first. \n" reset);
        printf("Press any key to return.\n");
        getch();
        return 0;
    }
    while(fread(&tempLog, sizeof(struct credentials), 1, p) == 1)
    {
        if(strcmp(tempLog.username, enteredUsername) == 0 && strcmp(tempLog.password, enteredPassword) == 0)
        {
            loginSuccess = 1;
            currentRole = tempLog.roleVal;		//it temporarily stores the current role to global until new login success
            break;
        }
    }
    fclose(p);
    
    if(loginSuccess)
    {
        printf(green "\n\nLogin successful! Welcome, %s.\n" reset, tempLog.name);
        logSuccess = tempLog;
        Sleep(1500);
        return 1;		//returns 1 to main and main loops to Landing page until logout
    }
    else
    {
        printf(red "\n\nInvalid username or password. Please try again.\n"reset);
        printf("Press any key to continue.\n");
    	getch();
        return 0;
    }
    return 0;
}

//3. Function for Signup, it contains input from user, check if the dataformat is correct, checks if they are available and finally a confirmation adn storage in datafile
void signup()
{
    i=0;
    char ch, tempPass[50];
    struct credentials log;
    FILE *p;
    ThemeArt();
    printf(green "--- Sign Up Page ---  \n" reset);
    //Full name::
    do{
        printf("\nName\n---------------\n");
        printf("Enter Full Name: ");
        fflush(stdin);
        
        gets(log.name);
        if(!isValidName(log.name))		//check if name contains only alphabets
        {
            printf("Invalid name! Please use alphabets and spaces only.\n");
        }
    } while (!isValidName(log.name));
    //Dob::
    printf("\nDate of Birth\n---------------\n");
    do{
        printf("Enter year (1900-2024):");
        if(scanf("%d", &log.d.year)!=1 || log.d.year < 1900 || log.d.year > 2025)
        {
            printf(red "Invalid year. Please enter valid year. \n" reset);
            fflush(stdin);
        }
        else break;
    } while(1);
    
    do{
        printf("Enter month (1-12): ");
        if(scanf("%d", &log.d.month)!=1 || log.d.month < 1  || log.d.month > 12)
        {
            printf(red "Invalid month. Please enter valid month. \n" reset);
            fflush(stdin);
        }
        else break;
    }while(1);
    
    do{
        printf("Enter day (1-31): ");
        if(scanf("%d", &log.d.day) !=1 || log.d.day <1 ||log.d.day > 31)
        {
            printf(red "Invalid day! Please enter valid day. \n" reset);
            fflush(stdin);
        }
        else break;
    } while(1);
    //Email::
    printf("\nEmail Address\n---------------\n");
    do{
        printf("Enter Email (@gmail.com): ");
        if(scanf("%s", log.email) !=1 || strstr(log.email, "@gmail.com") == NULL)
        {
            printf(red "Invalid email! Please enter valid email." reset);
            continue;
            fflush(stdin);
        }
        int count1=0, count2=0;
        int atIndex = -1, dotIndex = -1;
        char *email;
        int j;
        email = log.email;
        for(j=0; email[j] != '\0'; j++)
        {
        	if(email[j] == '@')
        	{
        		count1++;
        		atIndex = j;
			}
			else if(email[j] == '.')
			{
				count2++;
				dotIndex = j;
			}
		}
		if(count1 != 1 || count2 == 0 || email[0] == '@' || email[0]== '.' || atIndex>dotIndex)
		{
			printf(red "Invalid email! Please enter valid email." reset);
			fflush(stdin);
		}
		
        else if (isEmailTaken(log.email))
        {
            printf(red "Email '%s' is already registered. Please use another email.\n" reset, log.email);
        }
        else break;
    } while(1);
    //Username::
    printf("\nUsername\n---------------\n");
    do{
        fflush(stdin);

        printf("Enter Username: ");
        scanf("%s", log.username);
        if (isUsernameTaken(log.username))
        {
            printf("Username '%s' is already taken. Please try another username.\n\n",log.username);
        }
        else
        {
            break;
        }
    } while(1);
    //Password::
    printf("\nPassword\n---------------\n");
    printf("\nEnter a strong password: ");
    i=0;
    int char_count = 0;
    while(1)
    {
        fflush(stdin);

        ch = getch();
        if(ch == 13)
        {
            tempPass[char_count] = '\0';
            break;
        }
        if(ch == 8)
        {
            if(char_count > 0)
            {
                char_count--;
                printf("\b \b");
            }
        }
        else if(char_count < sizeof(tempPass) - 1)
        {
            tempPass[char_count] = ch;
            printf("*");
            char_count++;
        }
    }
    strcpy(log.password, tempPass);
    printf("\n");
    //Role selection::
    int getrole;
     do{
        printf("\nRole\n---------------\n");
        printf("Select a role:\n1. User\n2.Artist ");
        fflush(stdin);
        printf("\nYour choice :");
        if(scanf(" %d", &getrole) != 1 || getrole <=0 || getrole >2)
        {
        	printf(red "Invalid role! Please select 1 or 2.\n" reset);
        	continue;
		}
		else break;
    } while (1);
    if(getrole == 1)
    {
        strcpy(log.role, "User");
        log.roleVal=1;
        printf(green "'User' selected successfully." reset);
    }
    else
    {
        strcpy(log.role, "Artist");
        log.roleVal=2;
        printf(green "'Artist' selected successfully." reset);
    }
    //Profile display::
    printf(green "\n\nYour Profile:\n");
    printf("Name = %s\n",log.name);
    printf("DOB = %d-%d-%d\n",log.d.year, log.d.month, log.d.day);
    printf("Username = %s\n",log.username);
    printf("Email = %s\n",log.email);
    printf("Password = ");
    for(i=0; i<char_count; i++)
    {
        printf("*");
    }
    printf("\n" reset);
    
    //Signup Confirmation::
    printf("\n\nPress "red"enter"reset" to confirm sign_up: \n"reset);
    char confirm = getch();
    if(confirm == enter)
    {
    	p=fopen("credentials.dat", "ab");
    	if(p==NULL)
        {
            printf(red "\nSomething Went Wrong! \n*Please Try Later*\n" reset);
            getch();
            return;
        }
    	if (fwrite(&log, sizeof(struct credentials), 1, p) !=1)
    	{
        	printf(red "\nError! Failed to store credentials." reset);
    	fclose(p);
    	}	
    	else
    	{
    	    printf(green "\nSignup successful.\n" reset "Do you want to login?(y/n): ");
    	    fflush(stdin);

    	    char choice = getchar();
    	    strlwr(&choice);
    	    if(choice == 'y')
    	    {
    	        loginStatus(1);
    	    }
        	else
        	{
            	loginStatus(0);
        	}
    	}
	}
	else
	{
		printf(red "\nError! Signup Cancelled by user." reset);
		getch();
	}
	fclose(p);
    return;
}

//4. Function for RESET Password, it askd for username and email, if valid shows old pass and sets new password
void forgotPassword()
{
    ThemeArt();
    printf(green "--- Account Reset Page ---  \n" reset);
    struct credentials datafile;
    char input_username[50], new_pass[50], input_email[50];
    FILE *p;
    int char_count = 0;
    int account_found=0;
    long int current_pos=0;				//track where data is found
    
    printf("Enter username = ");
	fflush(stdin);
	gets(input_username);
	
	printf("Enter Email Address = ");
	fflush(stdin);
    gets(input_email);
	
    p=fopen("credentials.dat", "rb+");
    if(p==NULL)
    {
        printf(red "Error! file opening failed. " reset);
        Sleep(1500);
        return;
    }
    while(fread(&datafile, sizeof(struct credentials), 1, p) == 1)
    {
        current_pos = ftell(p) - sizeof(struct credentials);				//get the position of the file where account is found
        if(strcmp(datafile.username, input_username)==0 && strcmp(datafile.email, input_email)==0)
        {
            account_found  = 1;
            break;
        }
    }
    if(account_found)
    {
        printf("\nAccount found successfully.\n");
        printf(blue "\nYour password was = %s" reset, datafile.password);
        printf("\nEnter new password = ");
        while(1)
        {
            fflush(stdin);

            char ch = getch();
            if(ch == 13)
            {
                new_pass[char_count] = '\0';
                break;
            }
            if(ch == 8)
            {
                if(char_count > 0)
                {
                    char_count--;
                    printf("\b \b");
                }
            }
            else if(char_count < sizeof(new_pass) - 1)
            {
                new_pass[char_count] = ch;
                printf("*");
                char_count++;
            }
        }
        strcpy(datafile.password, new_pass);

        fseek(p, current_pos, SEEK_SET);
        if(fwrite(&datafile, sizeof(struct credentials), 1, p) == 1)
        {
            printf(green "\n|Password Reset successfully|" reset);
        }
        else
        {
            printf(red "\nError! Failed to update password in file." reset);
        }
        fclose(p);
    }
    else
    {
        printf(red "Account not found! please check your input." reset);
    }
    Sleep(1500);
    fclose(p);
    return;
}

//----------function for checking alphabets in name
int isValidName(const char *name)
{
    if(name==NULL || strlen(name)==0) return 0;
    for(i=0; name[i]!='\0'; i++)
    {
        if (!isalpha(name[i]) && !isspace(name[i])) return 0;
    }
    return 1;
}
//----------function for checking username available or not
int isUsernameTaken(const char *username)
{
    FILE *p;
    p=fopen("credentials.dat", "rb");
    if(p==NULL)
    {
        return 0;
    }
    struct credentials tempLog;
    int taken=0;
    while (fread(&tempLog, sizeof(struct credentials), 1, p)==1)
    {
        if(strcmp(tempLog.username, username)==0)
        {
            taken=1;
            break;
        }
    }
    fclose(p);
    return taken;
}
//----------function for checking email availability
int isEmailTaken(const char *email)
{
    FILE *p;
    struct credentials tempLog;
    int taken = 0;
    p = fopen("credentials.dat", "rb");
    if (p==NULL)
    {
        return 0;
    }
    while (fread(&tempLog, sizeof(struct credentials), 1, p) == 1)
    {
        if (strcmp(tempLog.email, email) == 0)
        {
            taken = 1;
            break;
        }
    }
    fclose(p);
    return taken;
}
//----------login status checker
void loginStatus(int x)
{
    logstatusCheck = x;
    return;
}

//5. Landing page function(), displays the themeart, and top navigationbar and on key press enter selects respected function and on esc triggers logout
void LandingPage()
{
	int max = 6;
    int selectedOption = 0;
    int currentKeyInput;
    while(1)
    {
//        system("cls");
			ThemeArt();
        for(i=0; i<max; i++)
        {
            if(selectedOption == i)
            {
                printf("  |"green"%s"reset"|  ", HomeBanner[i]);
            }
            else
            {
                printf("  |%s|  ", HomeBanner[i]);
            }
        }
//        printf(yellow "\n\nUse: wasd-(navigation), enter-(selection), esc-(exit)" reset);
        if (invalidKeyPressFlag == 1)
        {
            printf(red "\nInvalid keypress! Use: 'a' or 'b'-(navigation), enter-(selection), esc-(exit)" reset);
            getch();
            invalidKeyPressFlag = 0;
             
        }
        currentKeyInput = Controls(selectedOption, max, 'h');			//h for horizontol cursor movement
        if (currentKeyInput == enter)
        {
            switch(selectedOption)
            {
                case 0:
                    HomePage();	
                    break;
                case 1:
                    MainMenu();
                    break;
                case 2:
                    SearchSongPannel();
                    break;
                case 3:
                    Playlists();
                    break;
                case 4:
                    Profile();
                    break;
                case 5:
//                	Artist();
                case 6:
                    printf(green "\nLogging out...\n" reset);
                    Sleep(1500);
                    loginStatus(0);
                    return;
            }
        }
        else if (currentKeyInput == esc)
        {
            printf(green "\nLogging out...\n" reset);
            Sleep(1000);
            loginStatus(0);
            return;
        }
        else
        {
            selectedOption = currentKeyInput;
        }
    }
}

//6. home page function(), lists homemenu list and calls functions according to the selection
void HomePage()
{
	int max = 1;
    int selectedOption = 0;
    int currentKeyInput;
    while(1)
    {
        system("cls");
        printf(blue "|%s|" reset,HomeBanner[0]);
        printf("\n ----\n\n");
        for(i=0; i<max; i++)
        {
            if(selectedOption == i)
            {
                printf("-->%d. %s<--\n",i+1,  HomeMenuList[i]);
            }
            else
            {
                printf("%d. %s\n",i+1, HomeMenuList[i]);
            }
        }
        printf(yellow "\n\nUse: wasd-(navigation), enter-(selection), esc-(return)" reset);
        if (invalidKeyPressFlag == 1)
        {
            printf(red "\nInvalid keypress! Use: wasd-(navigation), enter-(selection), esc-(exit)" reset);
            invalidKeyPressFlag = 0;
        }
//get vertical controls
        currentKeyInput = Controls(selectedOption, max, 'v');
        if(currentKeyInput == esc)
        {
            return;
        }
        else if(currentKeyInput == enter)
        {
            switch(selectedOption)
            {
                case 0:
                    // SongPlay();
                    demoSongPlay();
                    break;
            }
        }
        else
        {
            selectedOption = currentKeyInput;
        }
    }
}

//7. mainmenu page function()
void MainMenu()
{
	int max = 7;			//(changed)
	if(currentRole == 1)			//if user limit the options
	{
		max = 5;
	}
	else if(currentRole == 2)
	{
		max = 6;
	}
    int selectedOption = 0;
    int currentKeyInput;

    while(1)
    {
        system("cls");
        printf(blue "|%s|" reset,HomeBanner[1]);
        printf("\n ---------\n\n");
        for(i=0; i<max; i++)
        {
            if(selectedOption == i)
            {
                printf("-->%d. %s<--\n", i+1, MainMenuList[i]);
            }
            else
            {
                printf("%d. %s\n",i+1, MainMenuList[i]);
            }
        }
        
        printf(yellow"\n\nUse: wasd-(navigation), enter-(selection), esc-(return)" reset);
        if (invalidKeyPressFlag == 1)
        {
            printf(red "\nInvalid keypress! Use: wasd-(navigation), enter-(selection), esc-(return)" reset);
            invalidKeyPressFlag = 0;
        }

        currentKeyInput = Controls(selectedOption, max, 'v');
        if(currentKeyInput == esc)
        {
            return;
        }
        else if(currentKeyInput == enter)
        {
            switch(selectedOption)
            {
                case 0:
                	//view all songs
                    viewSongs();
                    printf(green "\nPress any key to return..." reset);
    				getch();
                    break;
                case 1:
                	Playlists();
//                	"Create or Add Music to Playlist",	
                    break;
                case 2:
//                	"View All Liked Songs", (done)
                	viewLikedSongs();
                	printf(green "\nPress any key to return..." reset);
                		getch();
                    break;
                case 3:
//                	"Like Music"(done)
					viewSongs();
					if(globalSongsFound == 1)
					{
						addToLikedSongs();		
					}
                    break;
                case 4:
//                	"Remove Liked Music",	(done)
					viewLikedSongs();					//(**1fix)
					if(likedSongFound == 1)
					{
                		removeLikedSong();
					}
                    break;
                case 5:
//                	"Upload Songs (Artist)"
                	addSongs();		
                	break;
                case 6:
//                	"Delete Songs (Admin)"
					viewSongs();
					if(globalSongsFound ==1 )
					{
                    	removeSong();
					}
					getch();
                	break;

            	default:
            		printf(red "Invalid option selected!\n" reset);
            		break;
            	}
        }
        else
        {
            selectedOption = currentKeyInput;
        }
    }
}

//8. search function()
void SearchSongPannel()
{
	int ch=0;
    system("cls");
    printf(blue "|%s|" reset,HomeBanner[2]);
    printf("\n -------------\n\n");
    printf("\nSearch a song here");
    printf("\n1. Search by Name\n2. Search by artist\n");
    	invalidOption:
    		fflush(stdin);
    printf("\nEnter your choice :");
    ch = getch();
    if(ch != '1' && ch != '2' & ch != esc)
    {
    	printf(red "\nInvalid input! Please press '1', '2'(select) or 'esc'-(to return)"reset);
    	goto invalidOption;
	}
    switch(ch)
    {
    	case '1':
    		//searchByName;
    		printf(" 1\n");
    		searchSong('n');
    		break;
    	case '2':
    		//searchNyArtist;
    		printf(" 2\n");
    		searchSong('a');
    		break;
    	case esc:
    		printf(" esc\n");
    		break;
	}
    printf("\nPress any key to return.");
    getch();
}


//Search a song by name or artist
int searchSong(char choice) 
{
    FILE *search;
    struct song s;
    char getName[100];
    int found = 0;

    switch (choice){
    case 'n': // Search by Title
        printf("\nEnter the title of the song: ");
        fflush(stdin);
        gets(getName);
        search = fopen("songs.dat", "rb");
        if (search == NULL) 
        {
            printf(red "Failed to open song data!\n" reset);
            return 0;
        }
        while (fread(&s, sizeof(struct song), 1, search) == 1) 
        {
            if (stricmp(s.title, getName) == 0)
            {
                found = 1;
                printf(yellow "\nSong with name '%s' Found.\n\n"reset,getName );
                printf("Title : %s\n", s.title);
                printf("Artist: %s\n", s.artist);
                printf("--------------------------\n");
            }
        }
        if (!found) 
        {
			printf(red "\nSong with name '%s' Not found!\n"reset, getName );
        }
        fclose(search);
        break;

    case 'a': // Search by Artist
        printf("\nEnter the artist of the song: ");
        fflush(stdin);
        gets(getName);
        search = fopen("songs.dat", "rb");
        if (search == NULL) 
        {
            printf(red "Failed to open song data!\n" reset);
            return 0;
        }
        while (fread(&s, sizeof(struct song), 1, search) == 1) 
        {
            if (stricmp(s.artist, getName) == 0)
            {
                found = 1;
                printf(yellow "Artist Match Found:\n" reset);
                printf("Title : %s\n", s.title);
                printf("Artist: %s\n", s.artist);
                printf("--------------------------\n");
            }
        }
        if (!found) 
        {
            printf(red "No matching artist found.\n" reset);
        }
        fclose(search);
        break;
	}
    return 1; 
}

//9. search function()
void Playlists() {
    int selected = 0;
    char *playlistMenu[] = {
        "Create Playlist",
        "Add Songs to Playlist",
        "View Playlists",
        "Remove Songs From Playlist",
        "Delete a Playlist",
    };

    int currentKey;
    while (1) {
        system("cls");
        printf(green "=== Playlist Menu ===\n\n" reset);
        for (i = 0; i < 5; i++) {
            if (selected == i)
                printf("--> %s <--\n", playlistMenu[i]);
            else
                printf("    %s\n", playlistMenu[i]);
        }

        printf(yellow "\nUse W/S to navigate. Enter to select. Esc to go back." reset);
        currentKey = Controls(selected, 5, 'v');

        if (currentKey == enter) {
            switch (selected) {
                case 0: 
				createPlaylist();
				 break;
                case 1:
				 addSongToPlaylist();
				  break;
                case 2: 
				viewPlaylists(); 
				break;
                case 3: 
 				removeFromPlaylist();
				 break;
                case 4: 
			 	deletePlaylist();
			 	break;
				default:
					printf("\nInvalid Entry!\n");
            }
        } else if (currentKey == esc) {
            return;
        } else {
            selected = currentKey;
        }
    }
}
// createPlaylist
int createPlaylist() {
    system("cls");
    struct playlist p;
    FILE *fp = fopen("playlists.dat", "ab");

    if (fp == NULL) {
        printf(red "Error creating playlist file.\n" reset);
        getch();
        return 0;
    }

    // Playlist name input with validation
    do {
        printf("Enter playlist name: ");
        fflush(stdin);
        gets(p.name);
        p.name[strcspn(p.name, "\n")] = 0;  // Remove newline if any

        if (strlen(p.name) == 0) {
            printf(red "Playlist name cannot be empty! Please try again.\n" reset);
        }
    } while (strlen(p.name) == 0);

    fwrite(&p, sizeof(struct playlist), 1, fp);
    fclose(fp);

    // Create empty .dat file for this playlist
    char filename[150];
    strcpy(filename, p.name);
    strcat(filename, ".dat");
    FILE *pf = fopen(filename, "wb");
    if (pf != NULL){
    fclose(pf);	
	}
		 
    printf(green "Playlist '%s' created successfully!\n" reset, p.name);
    
    getch();
    return 1;
}


// addSongToPlaylist
int addSongToPlaylist() {
    system("cls");

    FILE *plistFile = fopen("playlists.dat", "rb");
    if (plistFile == NULL) {
        printf(red "No playlists found. Create one first.\n" reset);
        getch();
        return 0;
    }

    struct playlist p;
    int count = 0;
    struct playlist allPlaylists[100];

    printf(green "=== Select a Playlist ===\n" reset);
    while (fread(&p, sizeof(struct playlist), 1, plistFile) == 1 && count < 100) {
        printf("%d. %s\n", count + 1, p.name);
        allPlaylists[count++] = p;
    }
    fclose(plistFile);

    if (count == 0) {
        printf(yellow "No playlists available.\n" reset);
        getch();
        return 0;
    }

    int choice;
    printf("Enter the playlist number to add song to: ");
    scanf("%d", &choice);
    fflush(stdin);

    if (choice < 1 || choice > count) {
        printf(red "Invalid choice.\n" reset);
        getch();
        return 0;
    }

    FILE *songFile = fopen("songs.dat", "rb");
    if (songFile == NULL) {
        printf(red "No songs available to add.\n" reset);
        getch();
        return 0;
    }

    struct song s;
    int songCount = 0;
    struct song allSongs[100];

    printf("\n" green "=== Available Songs ===\n" reset);
    while (fread(&s, sizeof(struct song), 1, songFile) == 1 && songCount < 100) {
        printf("%d. %s\n", songCount + 1, s.title);
        allSongs[songCount++] = s;
    }
    fclose(songFile);

    if (songCount == 0) {
        printf(yellow "No songs found in library.\n" reset);
        getch();
        return 0;
    }

    int songChoice;
    printf("Enter the number of the song to add: ");
    scanf("%d", &songChoice);
    fflush(stdin);

    if (songChoice < 1 || songChoice > songCount) {
        printf(red "Invalid song selection.\n" reset);
        getch();
        return 0;
    }

    // Check if song already exists in the playlist
    char playlistFilename[150];
   strcpy(playlistFilename, allPlaylists[choice - 1].name);
	strcat(playlistFilename, ".dat");

    FILE *playlistFile = fopen(playlistFilename, "rb");
    int exists = 0;
    if (playlistFile != NULL) {
        while (fread(&s, sizeof(struct song), 1, playlistFile) == 1) {
            if (strcmp(s.title, allSongs[songChoice - 1].title) == 0) {
                exists = 1;
                break;
            }
        }
        fclose(playlistFile);
    }

    if (exists) {
        printf(yellow "\nThis song is already in the playlist '%s'.\n" reset, allPlaylists[choice - 1].name);
        getch();
        return 0;
    }

    // Add song to playlist
    playlistFile = fopen(playlistFilename, "ab");
    if (playlistFile == NULL) {
        printf(red "Error opening playlist file.\n" reset);
        getch();
        return 0;
    }

    fwrite(&allSongs[songChoice - 1], sizeof(struct song), 1, playlistFile);
    fclose(playlistFile);

    printf(green "\nSong added to playlist '%s'!\n" reset, allPlaylists[choice - 1].name);
    getch();
    return 1;
}


// viewPlaylists
int viewPlaylists() {
    system("cls");

    FILE *playlistFile = fopen("playlists.dat", "rb");
    if (playlistFile == NULL) {
        printf(red "No playlists found.\n" reset);
        getch();
        return 0;
    }

    struct playlist p;
    int count = 0;

    printf(green "=== Your Playlists ===\n\n" reset);

    while (fread(&p, sizeof(struct playlist), 1, playlistFile)==1) {
        printf("%d. %s\n", ++count, p.name);
    }

    if (count == 0) {
        printf(yellow "Your playlist list is empty.\n" reset);
    }

// need to show songs in specific playlist
    fclose(playlistFile);
    printf("\nPress any key to return...");
    getch();
    return 1;
}

//removeSongsfromPlaylist
int removeFromPlaylist() {
    system("cls");

    FILE *plistFile = fopen("playlists.dat", "rb");
    if (plistFile == NULL) {
        printf(red "No playlists found.\n" reset);
        getch();
        return 0;
    }

    struct playlist allPlaylists[100];
    struct playlist p;
    int count = 0;

    while (count < 100 && fread(&p, sizeof(struct playlist), 1, plistFile) == 1) {
        allPlaylists[count++] = p;
    }
    fclose(plistFile);

    if (count == 0) {
        printf(yellow "No playlists available.\n" reset);
        getch();
        return 0;
    }
int i;
    printf(green "=== Select a Playlist ===\n" reset);
    for (i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, allPlaylists[i].name);
    }

    int choice;
    printf("Enter the playlist number: ");
    scanf("%d", &choice);
    fflush(stdin);

    if (choice < 1 || choice > count) {
        printf(red "Invalid playlist selection.\n" reset);
        getch();
        return 0;
    }

    char filename[150];
    snprintf(filename, sizeof(filename), "%s.dat", allPlaylists[choice - 1].name);

    FILE *playlistFile = fopen(filename, "rb");
    if (playlistFile == NULL) {
        printf(red "Could not open playlist file.\n" reset);
        getch();
        return 0;
    }

    struct song allSongs[100], s;
    int songCount = 0;

    while (songCount < 100 && fread(&s, sizeof(struct song), 1, playlistFile) == 1) {
        allSongs[songCount++] = s;
    }
    fclose(playlistFile);

    if (songCount == 0) {
        printf(yellow "This playlist has no songs.\n" reset);
        getch();
        return 0;
    }

    printf(green "\n=== Songs in '%s' ===\n" reset, allPlaylists[choice - 1].name);
    for (i = 0; i < songCount; i++) {
        printf("%d. %s\n", i + 1, allSongs[i].title);
    }

    int songChoice;
    printf("Enter the number of the song to remove: ");
    scanf("%d", &songChoice);
    fflush(stdin);

    if (songChoice < 1 || songChoice > songCount) {
        printf(red "Invalid song selection.\n" reset);
        getch();
        return 0;
    }

    // Rewrite the playlist file without the selected song
    playlistFile = fopen(filename, "wb");
    if (playlistFile == NULL) {
        printf(red "Failed to write updated playlist.\n" reset);
        getch();
        return 0;
    }

    for (i = 0; i < songCount; i++) {
        if (i != songChoice - 1) {
            fwrite(&allSongs[i], sizeof(struct song), 1, playlistFile);
        }
    }
    fclose(playlistFile);

    printf(green "Song removed from '%s' successfully.\n" reset, allPlaylists[choice - 1].name);
    getch();
    return 1;
}

//deletePlaylist
int deletePlaylist() {
	system("cls");
    FILE *plistFile = fopen("playlists.dat", "rb");
    if (plistFile == NULL) {
        printf(red "No playlists found.\n" reset);
        getch();
        return 0;
    }

    struct playlist p;
    struct playlist allPlaylists[100];
    int count = 0;

    // Read all playlists into memory
    while (count < 100 && fread(&p, sizeof(struct playlist), 1, plistFile) == 1) {
        allPlaylists[count++] = p;
    }
    fclose(plistFile);

    if (count == 0) {
        printf(yellow "No playlists to delete.\n" reset);
        getch();
        return 0;
    }

    printf(green "=== Select a Playlist to Delete ===\n" reset);
    int i;
    for (i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, allPlaylists[i].name);
    }

    int choice;
    printf("Enter the playlist number to delete: ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > count) {
        printf(red "Invalid selection.\n" reset);
        fflush(stdin);
        getch();
        return 0;
    }
	fflush(stdin);

    printf(red "Are you sure you want to delete playlist '%s'? (1 = Yes, 0 = No): " reset, allPlaylists[choice - 1].name);
    int confirm;
    if (scanf("%d", &confirm) != 1 || (confirm != 0 && confirm != 1)) {
        printf(red "Invalid input.\n" reset);
        fflush(stdin);
		getch();
        return 0;
    }
  fflush(stdin);

    if (confirm != 1) {
        printf(yellow "Operation cancelled.\n" reset);
        getch();
        return 0;
    }

    // Delete the playlist file for the selected playlist
    char playlistFilename[150];
    snprintf(playlistFilename, sizeof(playlistFilename), "%s.dat", allPlaylists[choice - 1].name);
    if (remove(playlistFilename) == 0) {
        printf(green "Playlist file '%s' deleted successfully.\n" reset, playlistFilename);
    } else {
        printf(yellow "Playlist file '%s' could not be deleted or does not exist.\n" reset, playlistFilename);
    }

    // Rewrite playlists.dat without the deleted playlist
    plistFile = fopen("playlists.dat", "wb");
    if (plistFile == NULL) {
        printf(red "Failed to update playlists file.\n" reset);
        getch();
        return 0;
    }

    for (i = 0; i < count; i++) {
        if (i != choice - 1) {
            fwrite(&allPlaylists[i], sizeof(struct playlist), 1, plistFile);
        }
    }
    fclose(plistFile);
    getch();
    return 1;
}


//10. search function()
void Profile()
{
    system("cls");
    printf(blue "|%s|" reset,HomeBanner[4]);
    printf("\n -------\n\n");
    //Profile display::
    printf("\n\nYour Profile:\n");
    printf("Name = %s\n",logSuccess.name);
    printf("Role = %s\n",logSuccess.role);
    printf("DOB = %d-%d-%d\n",logSuccess.d.year, logSuccess.d.month, logSuccess.d.day);
    printf("Username = %s\n",logSuccess.username);
    printf("Email = %s\n",logSuccess.email);
    printf("\nPress any key to continue.");
    getch();
    printf("\n");
}
//11. addSongs function()
int addSongs() {
    struct song newSong;
    FILE *fp = fopen("songs.dat", "ab");  // Append mode
    if (fp == NULL) {
        printf(red "Error opening songs.dat file.\n" reset);
        return 0;
    }

    printf(green "\n--- Add New Song ---\n" reset);

    // Title input with validation
    do {
        printf("Enter song title: ");
        fflush(stdin);
        gets(newSong.title);
        newSong.title[strcspn(newSong.title, "\n")] = 0;  // Remove newline if any

        if (strlen(newSong.title) == 0) {
            printf(red "Title cannot be empty! Please try again.\n" reset);
        }
    } while (strlen(newSong.title) == 0);

    // Artist input with validation
    do {
        printf("Enter artist name: ");
        gets(newSong.artist);
        newSong.artist[strcspn(newSong.artist, "\n")] = 0;

        if (strlen(newSong.artist) == 0) {
            printf(red "Artist name cannot be empty! Please try again.\n" reset);
        }
    } while (strlen(newSong.artist) == 0);

    if (fwrite(&newSong, sizeof(struct song), 1, fp) != 1) {
        printf(red "\nError saving song.\n" reset);
    } else {
        printf(green "\nSong added successfully!\n" reset);
    }

    fclose(fp);
    printf("Press any key to continue...");
    getch();
    return 0;
}

//12. viewSongs Function(), for displaing all added songs
int viewSongs() {
    FILE *p;
    struct song s;
    int count = 0;

    p = fopen("songs.dat", "rb");
    if (p == NULL) {
        printf(red "\n\nNo songs in the library yet.\n" reset);
        globalSongsFound = 0;
        getch();
        return 0;
    }

    system("cls");
    printf(green "=== Your Music Library ===\n\n" reset);
    while (fread(&s, sizeof(struct song), 1, p) == 1) {
        count++;
        printf(yellow "Song #%d:\n" reset, count);
        printf("Title : %s\n", s.title);
        printf("Artist: %s\n", s.artist);
        printf("--------------------------\n");
        
        globalSongsFound = 1;			//added
    }

    if (count == 0) {
        printf(red "No songs found.\n" reset);
        globalSongsFound = 0;
    }

    fclose(p);
    return 0;
}

//13. remove songs
int removeSong() {
    FILE *original, *temp;
    struct song s;
    char titleToRemove[100];
    int found = 0;

    printf("\nEnter the title of the song to remove: ");
    fflush(stdin);
    gets(titleToRemove);

    original = fopen("songs.dat", "rb");
    if (original == NULL) {
        printf(red "No songs found!\n" reset);
        return 0;
    }

    temp = fopen("temp.dat", "wb");
    if (temp == NULL) {
        printf(red "Error creating temporary file!\n" reset);
        fclose(original);
        return 0;
    }

    while (fread(&s, sizeof(struct song), 1, original) == 1) {
        if (strcmp(s.title, titleToRemove) != 0) {
            fwrite(&s, sizeof(struct song), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(original);
    fclose(temp);

    remove("songs.dat");
    rename("temp.dat", "songs.dat");

    if (found){
    	 printf(green "\nSong \"%s\" removed successfully.\n" reset, titleToRemove);
         printf("Press any key to continue...");
	}
       
    else{
     printf(red "\nSong not found!\n" reset);
         printf("Press any key to return...");	
	}
       
}

//function for  assing songs to liked
int addToLikedSongs() {
    char title[100];
    struct song s;
    int found = 0, alreadyLiked = 0;

   do {
        printf("\nEnter the title of the song you want to like: ");
        fflush(stdin);
        gets(title);
        title[strcspn(title, "\n")] = 0; // Remove newline
        if (strlen(title) == 0) {
            printf(red "Song title cannot be empty! Please enter a valid title.\n" reset);
        }
    } while (strlen(title) == 0);

    FILE *allSongs = fopen("songs.dat", "rb");
    if (allSongs == NULL) {
        printf(red "\nNo songs available in library!\n" reset);
        return 0;
    }

    // Check if already liked
    FILE *liked = fopen("liked.dat", "rb");
    if (liked != NULL) {
        while (fread(&s, sizeof(struct song), 1, liked) == 1) {
            if (strcmp(s.title, title) == 0) {
                alreadyLiked = 1;
                break;
            }
        }
        fclose(liked);
    }
    if (alreadyLiked) {
        printf(yellow "\nSong is already in your liked songs!\n" reset);
        fclose(allSongs);
        getch();
        return 0;
    }

    //It try to find liked songs
    FILE *likedWrite = fopen("liked.dat", "ab");
    if (likedWrite == NULL) {
        printf(red "\nFailed to open liked songs file for writing.\n" reset);
        fclose(allSongs);
        return 0;
    }
    while (fread(&s, sizeof(struct song), 1, allSongs) == 1) {
        if (strcmp(s.title, title) == 0) {
            fwrite(&s, sizeof(struct song), 1, likedWrite);
            found = 1;
            break;
        }
    }
    fclose(allSongs);
    fclose(likedWrite);
    if (found) {
        printf(green "\n'%s' added to your liked songs!\n" reset, title);
        printf("Press any key to continue...");
    } else {
        printf(red "\nSong not found in library!\n" reset);
        printf("Press any key to return...");
    }

    getch();
}


//functions for displaying all liked songs if any
int viewLikedSongs() {
    system("cls");

    FILE *likedRead = fopen("liked.dat", "rb");
    if (likedRead == NULL) {
        printf(green "=== Your Music Library ===\n\n" reset);
        printf(red "No liked songs found.\n" reset);
        getch();
        return 0;
    }

    struct song s;
    printf(green "=== Your Liked Songs ===\n" reset);
    int count = 0;

    while (fread(&s, sizeof(struct song), 1, likedRead) == 1) {
        printf("%d. %s\n", ++count, s.title);
        likedSongFound = 1;
    }

    if (count == 0) {
        printf(yellow "You haven't liked any songs yet.\n" reset);
        printf("Press any key to continue...");
        getch();
        fclose(likedRead);
        return 0;
    }

    fclose(likedRead);
    return 0;
}


//remove liked songs
int removeLikedSong() {
    FILE *likedRead = fopen("liked.dat", "rb");
    FILE *tempWrite = fopen("temp.dat", "wb");
    if (likedRead == NULL) {
        printf(red "No liked songs found.\n" reset);
        getch();
        return 0;
    }
    if (tempWrite == NULL) {
        printf(red "Error opening temporary file.\n" reset);
        fclose(likedRead);
        getch();
        return 0;
    }

    char title[100];
    printf("Enter the title of the liked song to remove: ");
    fflush(stdin);
    gets(title);

    struct song s;
    int found = 0;
    while (fread(&s, sizeof(struct song), 1, likedRead) == 1) {
        if (strcmp(s.title, title) == 0) {
            found = 1; // Skip writing this one (remove)
            continue;
        }
        fwrite(&s, sizeof(struct song), 1, tempWrite);
    }
    fclose(likedRead);
    fclose(tempWrite);

    if (found) {
        remove("liked.dat");
        rename("temp.dat", "liked.dat");
        printf(green "Song removed from liked list successfully.\n" reset);
        getch();
    } else {
        remove("temp.dat"); // cleanup
        printf(red "Song not found in liked list.\n" reset);
        getch();
    }
}


//Controls For Console Selections
int Controls(int currentSelection, int maxOptions, char direction) {
    invalidKeyPressFlag = 0;
    int key_pressed;

    key_pressed = getch();
    key_pressed = tolower(key_pressed);

    if (direction == 'v') {
        if (key_pressed == up) {
            currentSelection--;
            if (currentSelection < 0) {
                currentSelection = maxOptions - 1;
            }
        } else if (key_pressed == down) {
            currentSelection++;
            if (currentSelection >= maxOptions) {
                currentSelection = 0;
            }
        } else if (key_pressed == enter) {
            return enter;
        } else if (key_pressed == esc) {
            return esc;
        } else {
            invalidKeyPressFlag = 1;
        }
    } else if (direction == 'h') {
        if (key_pressed == left) {
            currentSelection--;
            if (currentSelection < 0) {
                currentSelection = maxOptions - 1;
            }
        } else if (key_pressed == right) {
            currentSelection++;
            if (currentSelection >= maxOptions) {
                currentSelection = 0;
            }
        } else if (key_pressed == enter) {
            return enter;
        } else if (key_pressed == esc) {
            return esc;
        } else {
            invalidKeyPressFlag = 1;
        }
    } else {
        invalidKeyPressFlag = 1;
    }
    return currentSelection;
}

//demo
void viewAllDemoSongs() 
{
	int j;
    printf("Demo songs:\n");
    for (j = 0; j < totalSongs; j++) {
        printf(" %d. %s\n", j + 1, songs[j]);
    }
}

//play
void demoSongPlay() 
{
    int index = 0;
    int mediaButton;
    int maxSongs = totalSongs;  // number of demo songs
    
    viewAllDemoSongs();

    while (1) 
	{
        system("taskkill /IM vlc.exe >nul 2>&1");  // stop VLC before starting new song
        ThemeArt();
        viewAllDemoSongs();
        printf(yellow "\nPress: 'a' or 'd' to change song, esc to exit.\n" reset);
        printf(yellow "        <      >                   x"reset);
        printf(green "\nNow Playing: %s\n" reset, songs[index]);
        char command[200];
        sprintf(command, "start /min vlc.exe \"%s\"", songs[index]);		//writes the command like keeps name of song
        system(command);		//executes commmand 
        int playTime = 0;
        while (!kbhit()) 			//until any buttonpress
		{
            Sleep(1000);
            playTime++;
            printf("\rPlaying for %2ds", playTime);
            fflush(stdout);			//force output screen to flush
        }
        //Controls to get input and handle next/prev or exit
        mediaButton = Controls(index, maxSongs, 'h');  // 'h' for left-right (prev-next)
        if (mediaButton == esc) break;          // exit
       else 
		{
            index = mediaButton;
        }

        system("taskkill /IM vlc.exe >nul 2>&1"); // stop VLC before next loop
        printf("\n");
    }
    system("taskkill /IM vlc.exe >nul 2>&1");
    printf("\nThanks for demo-ing!\nPress any key to continue.");
    getch();
}

//admin setup
void initializeAdmin() 
{
    FILE *p;
    struct credentials admin_user;
    struct credentials tempLog;
    int admin_exists = 0;
    p = fopen("credentials.dat", "rb");
    if (p != NULL) 
	{
        //check if an admin user is present
        while (fread(&tempLog, sizeof(struct credentials), 1, p) == 1) 
		{
            if (strcmp(tempLog.username, "admin") == 0) 
			{
                admin_exists = 1;
                break;
            }
        }
        fclose(p);
    }
    if (!admin_exists) 
	{
        printf(red "\n\nNo 'admin' account found! Creating one now.\n" reset);
        // admin user data
        strcpy(admin_user.name, "Administrator");
        strcpy(admin_user.username, "admin");
        strcpy(admin_user.password, "admin"); 
        strcpy(admin_user.email, "admin@example.com");
        strcpy(admin_user.role, "Admin");
        admin_user.roleVal = 3;
        admin_user.id = 1;
        admin_user.d.day = 1;
        admin_user.d.month = 1;
        admin_user.d.year = 2000;
        p = fopen("credentials.dat", "ab");
        if (p == NULL) {
            printf("Error creating or opening credentials file for admin.\n");
            getch();
            return;
        }

        if (fwrite(&admin_user, sizeof(struct credentials), 1, p) != 1) {
            printf("Error writing admin user to file.\n");
            fclose(p);
            getch();
            return;
        }
        fclose(p);
        printf("Admin account created successfully!\n");
        printf("You can now log in as admin.\n");
        printf("Press any key to continue.\n");
        getch();
    }
}


void ThemeArt()
{
    system("cls");
    printf("\033[0;33m");
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
    printf("\033[0m");
    printf("\n\n=========================> C Based Music Library System <============================\n");
    printf(yellow"Guide: wasd-(navigation), enter-(selection), esc-(exit)\n\n"reset);
}

