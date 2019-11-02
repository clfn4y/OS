/*******************************************************************************
*		This assignment will emulate core functionality of a Linux-style shell 		 *
*		with respect to managing a file system. Your primary job is to develop     *
*		data structures and algorithms for managing files and folders such that a  *
*		user can interact with a limited set of commands via a command-line        *
*		familiar to users of Linux. You need NOT actually create files or folders  *
*		or in any way manipulate data in a real file system – this assignment      *
*		focuses on the management aspect only. 																		 *
*																																							 *
*																																							 *
*		Required commands to support: ls, ls -l, cd, pwd, mkdir, rmdir, rm,        *
*		chmod, touch (to ‘create’ a file or update the timestamp), exit/quit       *
*******************************************************************************/

#include<iostream>
using namespace std;

#include <vector> 
#include <algorithm>
#include <utility>
#include <list>
#include <string>
#include <ctime>
#include <sstream> 
#include <bitset>

// Define colors for output
#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */


// Struct
struct Files
{
	string file_name;
	string timestamp;
	string access; 
};

struct Folders
{
	// Variables
	string folder_name;
	string timestamp;
	string access; 
	pair <list<Folders>,list<Files>> contents;
	Folders* parent_directory;

	// Functions
	Folders* get_folder() {return this;}
	Folders &operator = (const Folders &folder);
};

// overload = 
Folders &Folders::operator = (const Folders &folder)
{
	return *this;
}

// get stored path
string path_func(Folders* current_directory)
{
	string path = "C:/";
	if (current_directory->parent_directory == NULL)
		return path + current_directory->folder_name + "$ ";
	else
	{
		string temp = "";
		Folders* i = current_directory->get_folder();
		while(i->parent_directory != NULL)
		{
			temp.insert(0, i->folder_name);
			temp.insert(0, "/");
			i = i->parent_directory->get_folder();
		}	
		return path + i->folder_name + temp + "$ ";

	}
}

// check enum commands for string input
enum enum_command {
	e_ls,
	e_cd,
	e_pwd,
	e_mkdir,
	e_rmdir,
	e_rm,
	e_chmod,
	e_touch,
	e_exit,
	e_empty,
	e_none    
};

// convert string input to enum
enum_command hashit (string const& inString) {
    if (inString == "ls") return e_ls;
    if (inString == "cd") return e_cd;
    if (inString == "pwd") return e_pwd;
    if (inString == "mkdir") return e_mkdir;
    if (inString == "rmdir") return e_rmdir;
    if (inString == "rm") return e_rm;
    if (inString == "chmod") return e_chmod;
    if (inString == "touch") return e_touch;
    if (inString == "exit" || inString == "quit") return e_exit;
    if (inString == "") return e_empty;
    return e_none;
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}


// Main
int main()
{
	// Start up
	cout << "\n\n******************************************" << endl;
	cout << "* Welcome to Caleb's Linux-style shell!! *" << endl;
	cout << "******************************************" << endl;

	// Create Root directory
	Folders ROOT;
	ROOT.folder_name = "ROOT";
	ROOT.parent_directory = NULL;
	ROOT.access = "111000000";
	ROOT.timestamp = currentDateTime();

	// Variables
	string path = "";
	string command = "";
	string input = "";
	string action_1 = "";
	string action_2 = "";
	int space = 0;
	bool running = true; 
	Folders* current_directory;

	current_directory = ROOT.get_folder();

	// Shell loop
	while(running)
	{
		// Location and getting input
		path = path_func(current_directory);
		cout << path;
		getline(cin, input);

		space = 0;
		command = "";
		action_1 = "";
		action_2 = "";
		for (int i = 0; i < input.length(); i++)
		{
			if (input[i] == ' ')
				space += 1;
			else if (input[i] != ' ' && space == 0)
				command += input[i]; 
			else if (input[i] != ' ' && space == 1)
				action_1 += input[i]; 
			else if (input[i] != ' ' && space == 2)
				action_2 += input[i];
		}

		// Logic for input using the enums
    switch (hashit(command)) 
    {
    	case e_none:
    		cout << RED << "PLEASE ENTER AN ACTUAL COMMMAND" << RESET << endl;
	      break;
	    case e_exit:
    		running = false;
	      break;
	    case e_ls:
	    {
	    	if (action_1 == "-l")
	    	{
		    	// Convert to correct format for access
	    		for (auto & i : current_directory->contents.first)
	    		{
		    		int index = 0;
		    		while(index < 9)
		    		{
	    				if (i.access[index] == '1' || i.access[index] == 'r')
	    					i.access[index] = 'r';
	    				else
	    					i.access[index] = '-';
	    				index++;
		    		
	    				if (i.access[index] == '1' || i.access[index] == 'w')
	    					i.access[index] = 'w';
	    				else
	    					i.access[index] = '-';
	    				index++;
	    			
	    				if (i.access[index] == '1' || i.access[index] == 'x')
	    					i.access[index] = 'x';
	    				else
	    					i.access[index] = '-';
	    				index++;
		    		}
	    		}

	    		// Convert to correct format for access
	    		for (auto & i : current_directory->contents.second)
	    		{
		    		int index = 0;
		    		while(index < 9)
		    		{
	    				if (i.access[index] == '1' || i.access[index] == 'r')
	    					i.access[index] = 'r';
	    				else
	    					i.access[index] = '-';
	    				index++;
		    		
	    				if (i.access[index] == '1' || i.access[index] == 'w')
	    					i.access[index] = 'w';
	    				else
	    					i.access[index] = '-';
	    				index++;
	    			
	    				if (i.access[index] == '1' || i.access[index] == 'x')
	    					i.access[index] = 'x';
	    				else
	    					i.access[index] = '-';
	    				index++;
		    		}
	    		}
	    		
					cout<<endl;
					for (auto const& i : current_directory->contents.first) {
				    cout << BLUE << "d" << i.access << " 1pbg " << "clfn4y" << " 1024 " << i.timestamp << " " << i.folder_name << "/" << RESET << endl;
				  }
				  // Print the files
				  for (auto const& i : current_directory->contents.second) {
				    cout << GREEN << "-" << i.access << " 1pbg " << "clfn4y" << " 1024 " << i.timestamp << " " << i.file_name << RESET << endl;
				  }
				  cout<<endl<<endl;
	    	}
				else
				{
					cout<<endl;
					for (auto const& i : current_directory->contents.first) {
				    cout << BLUE << i.folder_name << RESET << "\t";
				  }
				  // Print the files
				  for (auto const& i : current_directory->contents.second) {
				    cout << GREEN << i.file_name << RESET << "\t";
				  }
				  cout<<endl<<endl;
				}
	    	break;
	    }
	    case e_cd:
	    {
	    	if (action_1 == "..")
				{
					if (current_directory->parent_directory != NULL)
						current_directory = current_directory->parent_directory;
					else
						cout << RED << "CANNOT LEAVE ROOT" << RESET << endl;
						
				}
				else if (action_1 == "")
				{
				  current_directory = ROOT.get_folder();
				}
				else
				{
					bool found_folder = false;
					for (auto & i : current_directory->contents.first) 
					{
				    if (action_1 == i.folder_name && found_folder == false)
				    {
				    	found_folder = true;
				    	i.parent_directory = current_directory->get_folder();
				    	current_directory = i.get_folder();
				    }
				  }
					if (found_folder == false)
						cout << RED << "DOES NOT EXIST" << RESET << endl;
				}
	      break;
	    }
	    case e_pwd:
	    	cout << CYAN << endl << path << RESET << endl << endl;
	      break;
	   	case e_mkdir:
	   	{
	   		if (action_1 != "")
	   		{
	   			// Checks if folder already exist
					bool match = false;
					for (auto const& i : current_directory->contents.first) {
				    if (i.folder_name == action_1)
							match = true;
				  }
				  for (auto const& i : current_directory->contents.second) {
				    if (i.file_name == action_1)
							match = true;
				  }

					if (!match)
					{
						Folders create_folder;
						create_folder.folder_name = action_1;
						create_folder.access = "111000000";
						create_folder.timestamp = currentDateTime();

						current_directory->contents.first.push_back(create_folder);
					}
					else
						cout << RED << "ALREADY EXIST" << RESET << endl;
	   		}
	   		else
	   			cout << RED << "NO ACTION" << RESET << endl;
	   		
	      break;
	   	}
	    case e_rmdir:
	    {
	    	if (action_1 != "")
	   		{
	   			bool match = false;
		    	list<Folders>::iterator it = current_directory->contents.first.begin();
					while (it != current_directory->contents.first.end()) {
						if (it->folder_name == action_1) 
						{
							it = current_directory->contents.first.erase(it);
							match = true;
						}
						else
							it++;
					}
					if (!match)
						cout << RED << "DOES NOT EXIST" << RESET << endl;
	   		}
	   		else
	   			cout << RED << "NO ACTION" << RESET << endl;
			 
	      break;
	    }
      case e_rm:
      {
      	if (action_1 != "")
	   		{
	   			bool match = false;
	      	list<Files>::iterator it = current_directory->contents.second.begin();
					while (it != current_directory->contents.second.end()) {
						if (it->file_name == action_1) 
						{
							it = current_directory->contents.second.erase(it);
							match = true;
						}
						else
							it++;
					}
					if (!match)
						cout << RED << "DOES NOT EXIST" << RESET << endl;
	   		}
	   		else
	   			cout << RED << "NO ACTION" << RESET << endl;

	      break;
      }
	    case e_chmod:
	    {
	    	if (action_1 != "")
	   		{
	   			string temp = "";
		    	bool fail = false;
		    	bool found = false;
		    	if (action_1.length() == 3)
		    	{
		    		for (int i = 0; i < action_1.length(); i++)
			    	{

			    		string num_char = "";
			    		num_char += action_1[i]; 
			    		stringstream converter(num_char); 
					    int num = 0; 
					    converter >> num; 

					    if (num <= 7)
					    {
					    	temp += bitset<3>(action_1.c_str()[i]).to_string();
					    }
					    else
					    {
					    	cout << RED << "NOT VALID NUMBER" << RESET << endl; 
					    	fail = true;
					    }

					 
			    	}
			    	if (!fail)
			    	{
			    		for (auto & i : current_directory->contents.first) {
					    	if (i.folder_name == action_2)
					    	{
					    		i.access = temp;
					    		found = true;
					    	}
						  }

						  for (auto & i : current_directory->contents.second) {
						    if (i.file_name == action_2)
					    	{
					    		i.access = temp;
					    		found = true;
					    	}
						  }
						  if (!found)
						  	cout << RED << "NOT VALID NAME" << RESET << endl; 
			    	}
		    	}
		    	else
		    		cout << RED << "NOT VALID NUMBER" << RESET << endl;
	   		}
	   		else
	   			cout << RED << "NO ACTION" << RESET << endl;


	    	

	      break;
	    }
	    case e_touch:
	    {
	    	if (action_1 != "")
	   		{
	   			bool match = false;
					for (auto & i : current_directory->contents.second) {
				    if (i.file_name == action_1)
				    {
							match = true;
							i.timestamp  = currentDateTime();
				    }
				  }
				  for (auto & i : current_directory->contents.first) {
				    if (i.folder_name == action_1)
				    {
							match = true;
							i.timestamp  = currentDateTime();
				    }
				  }
					if (!match)
					{
						Files create_file;
						create_file.file_name = action_1;
						create_file.access = "111000000";
						create_file.timestamp = currentDateTime();

						current_directory->contents.second.push_back(create_file);
					}
	   		}
	   		else
	   			cout << RED << "NO ACTION" << RESET << endl;

	      break;
	    }
	    case e_empty:
	      break;
    }

	}

	return 0;
}



