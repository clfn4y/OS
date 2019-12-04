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
#include <string>
#include <sstream>

// Define colors for output
#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */

// Users and groups
struct Users
{
	string user_name;
	vector<string> user_groups;
	string primary_group;

	Users* get_user() {return this;}
	Users &operator = (const Users &user);
};

// Struct
struct Files
{
	string file_name;
	string timestamp;
	string access;
	string format_access;
	string primary_group;
	Users* owner;
};

struct Folders
{
	// Variables
	string folder_name;
	string timestamp;
	string access;
	string format_access;
	string primary_group;
	Users* owner;
	pair <list<Folders>,list<Files>> contents;
	Folders* parent_directory;

	// Functions
	Folders* get_folder() {return this;}
	Folders &operator = (const Folders &folder);
};

Folders &Folders::operator = (const Folders &folder)
{
	return *this;
}
Users &Users::operator = (const Users &user)
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

string has_folder_access(Users* user, Folders* folder, string level)
{
	string true_permission = "";
	bool part_of_group = false;
	bool owner = false;
	if (folder->owner->user_name == user->user_name)
	{
		if (level[0] == 'r' && folder->access[0] == '1')
			true_permission += 'r';
		else
			true_permission += '-';
		if (level[1] == 'w' && folder->access[1] == '1')
			true_permission += 'w';
		else
			true_permission += '-';
		if (level[2] == 'x' && folder->access[2] == '1')
			true_permission += 'x';
		else
			true_permission += '-';
		owner = true;
		return true_permission;
	}
	if (owner == false)
	{
		for (auto & i : user->user_groups)
		{
			if (i == folder->primary_group)
			{
				if (level[0] == 'r' && folder->access[3] == '1')
					true_permission += 'r';
				else
					true_permission += '-';
				if (level[1] == 'w' && folder->access[4] == '1')
					true_permission += 'w';
				else
					true_permission += '-';
				if (level[2] == 'x' && folder->access[5] == '1')
					true_permission += 'x';
				else
					true_permission += '-';
				part_of_group = true;
				return true_permission;
			}
		}
		if (!part_of_group) //user is not a part of the owners group and uses the system permission level
		{
			if (level[0] == 'r' && folder->access[6] == '1')
				true_permission += 'r';
			else
				true_permission += '-';
			if (level[1] == 'w' && folder->access[7] == '1')
				true_permission += 'w';
			else
				true_permission += '-';
			if (level[2] == 'x' && folder->access[8] == '1')
				true_permission += 'x';
			else
				true_permission += '-';
			return true_permission;
		}
	}
}

string has_file_access(Users* user, Files file, string level)
{
	string true_permission = "";
	bool part_of_group = false;
	bool owner = false;
	if (file.owner->user_name == user->user_name)
	{
		if (level[0] == 'r' && file.access[0] == '1')
			true_permission += 'r';
		else
			true_permission += '-';
		if (level[1] == 'w' && file.access[1] == '1')
			true_permission += 'w';
		else
			true_permission += '-';
		if (level[2] == 'x' && file.access[2] == '1')
			true_permission += 'x';
		else
			true_permission += '-';
		owner = true;
		return true_permission;
	}
	if (owner == false)
	{
		for (auto & i : user->user_groups)
		{
			if (i == file.primary_group)
			{
				if (level[0] == 'r' && file.access[3] == '1')
					true_permission += 'r';
				else
					true_permission += '-';
				if (level[1] == 'w' && file.access[4] == '1')
					true_permission += 'w';
				else
					true_permission += '-';
				if (level[2] == 'x' && file.access[5] == '1')
					true_permission += 'x';
				else
					true_permission += '-';
				part_of_group = true;
				return true_permission;
			}
		}
		if (!part_of_group) //user is not a part of the owners group and uses the system permission level
		{
			if (level[0] == 'r' && file.access[6] == '1')
				true_permission += 'r';
			else
				true_permission += '-';
			if (level[1] == 'w' && file.access[7] == '1')
				true_permission += 'w';
			else
				true_permission += '-';
			if (level[2] == 'x' && file.access[8] == '1')
				true_permission += 'x';
			else
				true_permission += '-';
			return true_permission;
		}
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

	e_useradd,
	e_chuser,
	e_groupadd,
	e_usermod,
	e_chown,
	e_chgrp,
	e_userdel,
	e_groupdel,
	e_groups,
	e_users,

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

    if (inString == "useradd") return e_useradd;
    if (inString == "chuser") return e_chuser;
    if (inString == "groupadd") return e_groupadd;
    if (inString == "usermod") return e_usermod;
    if (inString == "chown") return e_chown;
    if (inString == "chgrp") return e_chgrp;
    if (inString == "userdel") return e_userdel;
    if (inString == "groupdel") return e_groupdel;
    if (inString == "groups") return e_groups;
    if (inString == "users") return e_users;

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

void delete_user(string name, Folders* index, Users* root)
{
	if (index->contents.first.empty() && index->contents.second.empty())
		return;
	else
	{
		for (auto & i : index->contents.second)
		{
			if (i.owner->user_name == name)
				i.owner = root->get_user();
		}
		for (auto & i : index->contents.first)
		{
			if (i.owner->user_name == name)
			{
				i.owner = root->get_user();
				delete_user(name, i.get_folder(), root);
			}
		}
		return;
	}

}

void delete_group(string name, Folders* index)
{
	if (index->contents.first.empty() && index->contents.second.empty())
		return;
	else
	{
		for (auto & i : index->contents.second)
		{
			if (i.primary_group == name)
				i.primary_group = "USERS";
		}
		for (auto & i : index->contents.first)
		{
			if (i.primary_group == name)
			{
				i.primary_group = "USERS";
				delete_group(name, i.get_folder());
			}
		}
		return;
	}

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
	ROOT.access = "111111111";
	ROOT.timestamp = currentDateTime();

	// Create initial user and group
	Users ROOT_USER;
	string USERS = "USERS";
	ROOT_USER.user_name = "ROOT_USER";
	ROOT_USER.primary_group = USERS;
	ROOT_USER.user_groups.push_back(USERS);
	ROOT.owner = ROOT_USER.get_user();

	vector<Users*> users_list;
	users_list.push_back(ROOT_USER.get_user());
	vector<string> group_list;
	group_list.push_back(USERS);

	Users* active_user = ROOT_USER.get_user();

	// Variables
	string path = "";
	string command = "";
	string input = "";
	string action_1 = "";
	string action_2 = "";
	string action_3 = "";
	string action_4 = "";
	string action_5 = "";
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
		action_3 = "";
		action_4 = "";
		action_5 = "";
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
			else if (input[i] != ' ' && space == 3)
				action_3 += input[i];
			else if (input[i] != ' ' && space == 4)
				action_4 += input[i];
			else if (input[i] != ' ' && space == 5)
				action_5 += input[i];
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
	    	string permission = has_folder_access(active_user, current_directory->get_folder(), "r--");

	    	if (action_1 == "-l" && permission[0] == 'r')
	    	{
		    	// Convert to correct format for access
	    		for (auto & i : current_directory->contents.first)
	    		{
		    		int index = 0;
						string temp_access = "---------";
		    		while(index < 9)
		    		{
	    				if (i.access[index] == '1' || i.access[index] == 'r')
	    					temp_access[index] = 'r';
	    				else
	    					temp_access[index] = '-';
	    				index++;

	    				if (i.access[index] == '1' || i.access[index] == 'w')
	    					temp_access[index] = 'w';
	    				else
	    					temp_access[index] = '-';
	    				index++;

	    				if (i.access[index] == '1' || i.access[index] == 'x')
	    					temp_access[index] = 'x';
	    				else
	    					temp_access[index] = '-';
	    				index++;
		    		}
						i.format_access = temp_access;
	    		}

	    		// Convert to correct format for access
	    		for (auto & i : current_directory->contents.second)
	    		{
		    		int index = 0;
						string temp_access = "---------";
		    		while(index < 9)
		    		{
	    				if (i.access[index] == '1' || i.access[index] == 'r')
	    					temp_access[index] = 'r';
	    				else
	    					temp_access[index] = '-';
	    				index++;

	    				if (i.access[index] == '1' || i.access[index] == 'w')
	    					temp_access[index] = 'w';
	    				else
	    					temp_access[index] = '-';
	    				index++;

	    				if (i.access[index] == '1' || i.access[index] == 'x')
	    					temp_access[index] = 'x';
	    				else
	    					temp_access[index] = '-';
	    				index++;
		    		}
						i.format_access = temp_access;
	    		}

					cout<<endl;
					for (auto const& i : current_directory->contents.first) {
				    cout << BLUE << "d" << i.format_access << " 1pbg " << i.owner->user_name << " " << i.primary_group << " 1024 " << i.timestamp << " " << i.folder_name << "/" << RESET << endl;
				  }
				  // Print the files
				  for (auto const& i : current_directory->contents.second) {
				    cout << GREEN << "-" << i.format_access << " 1pbg " << i.owner->user_name << " " << i.primary_group << " 1024 " << i.timestamp << " " << i.file_name << RESET << endl;
				  }
				  cout<<endl<<endl;
	    	}
				else if (action_1 == "" && permission[0] == 'r')
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
				else
					cout << RED << "CHOSEN USER DOES NOT HAVE PERMISSION TO PERFORM THIS ACTION OR NOT PROPER ACTION" << RESET << endl;
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
	   		string permission = has_folder_access(active_user, current_directory->get_folder(), "-w-");

	   		if (action_1 != "" && permission[1] == 'w')
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
						create_folder.owner = active_user;
						create_folder.primary_group = active_user->primary_group;

						current_directory->contents.first.push_back(create_folder);
					}
					else
						cout << RED << "ALREADY EXIST" << RESET << endl;
	   		}
	   		else
	   			cout << RED << "NO ACTION OR NOT PROPER PERMISSION" << RESET << endl;

	      break;
	   	}
	    case e_rmdir:
	    {
	    	string permission = has_folder_access(active_user, current_directory->get_folder(), "-w-");

	    	if (action_1 != "" && permission[1] == 'w')
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
	   			cout << RED << "NO ACTION or NOT PROPER PERMISSION" << RESET << endl;

	      break;
	    }
      case e_rm:
      {
      	string permission = has_folder_access(active_user, current_directory->get_folder(), "-w-");

      	if (action_1 != "" && permission[1] == 'w')
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
	   			cout << RED << "NO ACTION OR PROPER PERMISSION" << RESET << endl;

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
	    	string permission = has_folder_access(active_user, current_directory->get_folder(), "-w-");

	    	if (action_1 != "" && permission[1] == 'w')
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
						create_file.owner = active_user;
						create_file.primary_group = active_user->primary_group;

						current_directory->contents.second.push_back(create_file);
					}
	   		}
	   		else
	   			cout << RED << "NO ACTION OR NOT PROPER PERMISSION" << RESET << endl;

	      break;
	    }





	    case e_useradd:
	    {
	    	if (action_1 != "")
	   		{
	   			if (action_1 == "-G")
	   			{
	   				int flag = false;
		   			for (auto & i : users_list)
						{
							if (i->user_name == action_3)
								flag = true;
						}

		   			if (!flag)
		   			{
		   				Users * new_user = new Users;
		   				new_user->user_name = action_3;
		   				new_user->user_groups.push_back(USERS);
		   				new_user->primary_group = USERS;

		   				// Add it to the other groups
							stringstream ss(action_2);
							string group;
							while (getline(ss,group, ','))
							{
								bool found = false;
								for(auto & i : group_list)
								{
									if (i == group)
									{
										new_user->user_groups.push_back(group);
										found = true;
									}
								}
								if (!found)
									cout << RED << "NOT AN ACTUAL GROUP: " << group << RESET << endl;
							}
		   				users_list.push_back(new_user);
		   			}
		   			else
		   				cout << RED << "USER ALREADY EXIST" << RESET << endl;
	   			}
	   			else
	   			{
	   				bool flag = false;
		   			for (auto & i : users_list)
						{
							if (i->user_name == action_1)
								flag = true;
						}

		   			if (!flag)
		   			{
		   				Users * new_user = new Users;
		   				new_user->user_name = action_1;
		   				new_user->user_groups.push_back(USERS);
		   				new_user->primary_group = USERS;
		   				users_list.push_back(new_user);
		   			}
		   			else
		   				cout << RED << "USER ALREADY EXIST" << RESET << endl;
		   		}
	   		}
	   		else
	   			cout << RED << "NO ACTION" << RESET << endl;

	    	break;
	    }

	    case e_chuser:
	    {
	    	if (action_1 != "")
	   		{
	   			bool flag = false;
	   			for (auto & i : users_list)
					{
						if (i->user_name == action_1)
						{
							flag = true;
							active_user = i->get_user();
						}
					}

	   			if (!flag)
	   				cout << RED << "USER DOES NOT EXIST" << RESET << endl;
	   		}
	   		else
	   			cout << RED << "NO ACTION" << RESET << endl;

	   		break;
	  	}

	  	case e_groupadd:
	    {
	    	if (action_1 != "")
	   		{
	   			bool flag = false;
	   			for (auto & i : group_list)
					{
						if (i == action_1)
							flag = true;
					}

	   			if (!flag)
	   			{
	   				string new_group = action_1;
						ROOT_USER.user_groups.push_back(new_group);

						group_list.push_back(new_group);
	   			}
	   			else
	   				cout << RED << "GROUP ALREADY EXIST" << RESET << endl;
	   		}
	   		else
	   			cout << RED << "NO ACTION" << RESET << endl;

	   		break;
	   	}

	   	case e_usermod:
	    {
	    	if (action_1 != "")
	   		{
	   			if (action_1 == "-g")
	   			{
	   				if(find(group_list.begin(), group_list.end(), action_2) != group_list.end())
	   				{
	   					bool found = false;
		   				for(auto & i : users_list)
		   				{
		   					if (i->user_name == action_3)
		   					{
		   						found = true;
		   						if(find(i->user_groups.begin(), i->user_groups.end(), action_2) != i->user_groups.end())
		   							i->primary_group = action_2;
		   						else
		   							cout << RED << "NOT IN GROUP" << RESET << endl;
		   					}
		   				}
		   				if (!found)
								cout << RED << "NOT AN ACTUAL USER" << RESET << endl;
	   				}
	   				else
	   					cout << RED << "NOT AN ACTUAL GROUP" << RESET << endl;
	   			}
	   			else if (action_1 == "-a" && action_2 == "-G")
	   			{
	   				if(find(group_list.begin(), group_list.end(), action_3) != group_list.end())
	   				{
	   					bool found = false;
		   				for(auto & i : users_list)
		   				{
		   					if (i->user_name == action_4)
		   					{
		   						found = true;
		   						i->user_groups.push_back(action_3);
		   					}
		   				}
		   				if (!found)
								cout << RED << "NOT AN ACTUAL USER" << RESET << endl;
	   				}
	   				else
	   					cout << RED << "NOT AN ACTUAL GROUP" << RESET << endl;
	   			}
	   			else
	   				cout << RED << "WRONG FLAGS" << RESET << endl;
	   		}
	   		else
	   			cout << RED << "NO ACTION" << RESET << endl;

	   		break;
	   	}

	   	case e_chown:
	    {
				bool user_found = false, object_found = false;
	    	if (action_1 != "")
	   		{
					for (auto & i : users_list)
					{
						if (action_1 == i->user_name)
						{
							user_found = true;
							for (auto & j : current_directory->contents.first)
							{
								if (action_2 == j.folder_name)
								{
									object_found = true;
									string permission = has_folder_access(active_user, j.get_folder(), "-w-");
									if (permission[1] == 'w')
									{
										j.owner = i;
										j.access = "111000000";
										j.primary_group = i->primary_group;
									}
									else
										cout << RED << "CHOSEN USER DOES NOT HAVE PERMISSION TO PERFORM THIS ACTION" << RESET << endl;
								}
							}
							for (auto & j : current_directory->contents.second)
							{
								if (action_2 == j.file_name)
								{
									object_found = true;
									string permission = has_file_access(active_user, j, "-w-");
									if (permission[1] == 'w')
									{
										j.owner = i;
										j.access = "111000000";
										j.primary_group = i->primary_group;
									}
									else
										cout << RED << "CHOSEN USER DOES NOT HAVE PERMISSION TO PERFORM THIS ACTION" << RESET << endl;
								}
							}
						}
					}
					if (!user_found)
						cout << RED << "NOT AN ACTUAL USER" << RESET << endl;
					if (!object_found && user_found)
						cout << RED << "NOT AN ACTUAL FOLDER OR FILE" << RESET << endl;
	   		}
	   		else
	   			cout << RED << "NO ACTION" << RESET << endl;
	   		break;
	   	}

	   	case e_chgrp:
	    {
	    	if (action_1 != "")
	   		{
	   			bool found_group = false;
	   			bool found_object = false;
	   			for (int i = 0; i < group_list.size(); ++i)
	   			{
	   				if (group_list[i] == action_1)
	   					found_group = true; 
					}

	   			if (found_group)
	   			{
	   				for (auto & i : current_directory->contents.first)
						{
							if (i.folder_name == action_2)
							{
								found_object = true;

								string permission = has_folder_access(active_user, i.get_folder(), "-w-");
								if (permission[1] == 'w')
									i.primary_group = action_1;
								else
									cout << RED << "CHOSEN USER DOES NOT HAVE PERMISSION TO PERFORM THIS ACTION" << RESET << endl;
							}
		   			}
		   			if (!found_object)
		   			{
		   				for (auto & i : current_directory->contents.second)
							{
								if (i.file_name == action_2)
								{
									found_object = true;

									string permission = has_file_access(active_user, i, "-w-");
									if (permission[1] == 'w')
										i.primary_group = action_1;
									else
										cout << RED << "CHOSEN USER DOES NOT HAVE PERMISSION TO PERFORM THIS ACTION" << RESET << endl;
								}
			   			}
			   		}
		   			if (!found_object)
		   				cout << RED << "OBJECT NOT FOUND" << RESET << endl;
		   		}
		   		else
		   			cout << RED << "GROUP NOT FOUND" << RESET << endl;
	   		}
	   		else
	   			cout << RED << "NO ACTION" << RESET << endl;

	   		break;
	   	}

	   	case e_userdel:
	    {
	    	if (action_1 != "")
	   		{
	   			if (action_1 == "-G")
	   			{
	   				bool found = false;
	   				int user_index = 0;
	   				int group_index = 0;
	   				if(find(group_list.begin(), group_list.end(), action_2) != group_list.end())
	   				{
 							for(auto & i : users_list)
	   					{
	   						if (i->user_name == action_3)
	   						{
	   							for (auto & j : i->user_groups)
	   							{
	   								if (j == action_2)
	   								{
	   									found = true;
	   									if (action_2 == "USERS" || action_3 == "ROOT_USER")
				   							cout << RED << "CANT EDIT Users group or ROOT_USER" << RESET << endl;
				   						else
				   							users_list[user_index]->user_groups.erase(users_list[user_index]->user_groups.begin()+group_index);
	   								}
	   								group_index++;
	   							}

	   						}
	   						user_index++;
	   					}

	   					if (!found)
	   						cout << RED << "USER NOT APART OF GROUP OR USER NOT FOUND" << RESET << endl;
	   				}
	   				else
	   					cout << RED << "NOT AN ACTUAL GROUP" << RESET << endl;
	   			}
	   			else
	   			{
	   				if (action_1 != "ROOT_USER")
	   				{
	   					bool found = false;
	   					int index = 0;
	   					for (auto & i : users_list)
	   					{
	   						if (i->user_name == action_1)
	   						{
	   							found = true;
	   							delete_user(i->user_name, ROOT.get_folder(), ROOT_USER.get_user());
	   							users_list.erase(users_list.begin()+index);
	   						}
	   						index ++;
	   					}
	   					if (!found)
	   						cout << RED << "USER NOT FOUND" << RESET << endl;
	   				}
	   				else
	   					cout << RED << "CANT DELETE ROOT_USER" << RESET << endl;
	   			}
	   		}
	   		else
	   			cout << RED << "NO ACTION" << RESET << endl;

	   		break;
	   	}

	   	case e_groupdel:
	    {
	    	if (action_1 != "")
	   		{
	   			if (action_1 != "USERS")
	   			{
	   				bool found = false;
	   				for (int i = 0; i < group_list.size(); ++i)
	   				{
	   					if (group_list[i] == action_1)
	   					{
	   						found = true;
	   						for (auto & i : users_list)
	   						{
	   							for (int j = 0; j < i->user_groups.size(); j++)
		   						{
		   							if (i->user_groups[j] == action_1)
		   							{
		   								i->user_groups.erase(i->user_groups.begin()+j);
		   							}
		   						}
	   						}
	   						delete_group(action_1, ROOT.get_folder());
	   						group_list.erase(group_list.begin()+i);
	   					}
	   				}
	   				if (!found)
	   					cout << RED << "GROUP NOT FOUND" << RESET << endl;
	   			}
	   			else
	   				cout << RED << "CAN'T DELETE USERS GROUP" << RESET << endl;
	   		}
	   		else
	   			cout << RED << "NO ACTION" << RESET << endl;

	   		break;
	   	}

	   	case e_groups:
	    {
	    	if (action_1 != "")
	   		{
	   			bool found = false;
	   			for(auto & i : users_list)
	   			{
	   				if(i->user_name == action_1)
	   				{
	   					found = true;
	   					for(auto & j : i->user_groups)
	   						cout << GREEN << j << RESET << " ";
	   					cout << endl;
	   				}
	   			}
	   			if (!found)
	   				cout << RED << "NO USER FOUND" << RESET << endl;

	   		}
	   		else
	   			cout << RED << "NO ACTION" << RESET << endl;

	   		break;
	   	}

	   	case e_users:
	    {
	    	for(auto & i: users_list)
	    		cout << GREEN << i->user_name << " " << RESET;
	    	cout << endl;

	   		break;
	   	}

	    case e_empty:
	      break;
    }

	}

	return 0;
}








