#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void displayDirectory(const string path)
{
    for (const auto& entry : fs::directory_iterator(path)) 
	{
		if(fs::is_directory(entry))
	    	cout << entry.path().filename() << endl;
	}

}
void displayFiles(const string path)
{
	for (const auto& entry : fs::directory_iterator(path))
	{
		if(fs::is_regular_file(entry))
			cout << entry.path().filename() << endl;
	}
}
void displayMenu()
{
	cout << "1. To navigate directories."<<endl;
	cout << "2. To copy file from current directory."<<endl;
	cout << "3. To move file from current directory."<<endl;
	cout << "4. To open a file from current directory."<<endl;
	cout << "5. To create a new directory in current path."<<endl;
	cout << "0. To exit program."<<endl;
}
bool pathExist(const string path)
{
	return fs::exists(path) && fs::is_directory(path);
}
void copyFile(string& currentPath)
{
	string fileName;
	cout << "Enter filename you would like to copy with extension (eg .txt): ";
	getline(cin, fileName);
    cout<<endl;

	while(!fs::exists(currentPath+'\\'+fileName))
	{
		cout << "Invalid file. Please enter a valid file: ";
		getline(cin,fileName);
	}

	string destinationPath = "C:\\Users\\FlyHercules\\Desktop";	//path
	string temp = "";
	cout << "Current Directory: "<<destinationPath<<endl;
	cout << "Directory content: "<<endl;
	displayDirectory(destinationPath);
	cout<<endl;
	while(temp.size()==0)
	{
		cout << "Please navigate to location for paste (Press Enter without any input \nif are already in destination directory): ";
		getline(cin, temp);

		if(temp.size()==0)	//copy
		{
			if(fs::exists(destinationPath + '\\' + fileName))
				fs::remove(destinationPath + '\\' + fileName);

			fs::copy(currentPath + '\\' + fileName, destinationPath + '\\' + fileName);
			cout << "File was copied!!" <<endl;
			break;
		}
		else
		{
			while(!pathExist(destinationPath+'\\'+temp))
			{
				cout << "Invalid path. Please enter a valid folder/directory: ";
				getline(cin,temp);
			}
			destinationPath.push_back('\\');
			destinationPath += temp;
			temp = "";
		}
	}
}
void moveFile(string& currentPath)
{
    string fileName;
    cout << "Enter filename you would like to move with extension (eg .txt): ";
    getline(cin, fileName);
    cout << endl;

    while (!fs::exists(currentPath + '\\' + fileName))
    {
        cout << "Invalid file. Please enter a valid file: ";
        getline(cin, fileName);
    }

    string destinationPath = "C:\\Users\\FlyHercules\\Desktop";	//path
    string temp = "";
    cout << "Current Directory: " << destinationPath << endl;
    cout << "Directory content: " << endl;
    displayDirectory(destinationPath);
    cout << endl;
    while (temp.size() == 0)
    {
        cout << "Please navigate to location for paste (Press Enter without any input \nif are already in destination directory): ";
        getline(cin, temp);

        if (temp.size() == 0)	//copy
        {
            if (fs::exists(destinationPath + '\\' + fileName))
                fs::remove(destinationPath + '\\' + fileName);

            fs::copy(currentPath + '\\' + fileName, destinationPath + '\\' + fileName);
            fs::remove(currentPath + '\\' + fileName);
            cout << "File was moved!!" << endl;
            break;
        }
        else
        {
            while (!pathExist(destinationPath + '\\' + temp))
            {
                cout << "Invalid path. Please enter a valid folder/directory: ";
                getline(cin, temp);
            }
            destinationPath.push_back('\\');
            destinationPath += temp;
            temp = "";
        }
    }
}
// void openFile(const string currentPath, string& fileName)
// {
	
// }
void changeDirectory(string& currentPath)
{
	string str;
	cout << "Current Directory: "<<currentPath<<endl;
    cout << "Specify your destination (Press Enter if you are already in the destination directory)\nType '..' if you would like to go back a folder in directory:\n";
    getline(cin,str);
	cout << endl<<endl;
	while(str.size()!=0)
	{
		if(str=="..")
		{
			size_t backSlash = currentPath.rfind("\\");
			if(backSlash!=string::npos)
			{
				currentPath.erase(backSlash);
				cout << "Current Directory: "<<currentPath<<endl;
			}
			else
				cout << "String not modified"<<endl;

			cout << "Current Directory: "<<currentPath<<endl;
			cout << "Directory Content: "<<endl;
			displayDirectory(currentPath);
			displayFiles(currentPath);
			cout << endl;
			cout << "Specify your destination (Press Enter if you are already in the destination directory): \n";
			getline(cin,str);
		}
		else
		{
			while (!pathExist(currentPath+'\\'+str)) 
			{
				cout << "Invalid path. Please enter a valid folder/directory (Press enter if no input is needed):\n";
				getline(cin,str);
				cout<<endl;	
			}
			currentPath.push_back('\\');
			currentPath += str;	

			cout << "Current Directory: "<<currentPath<<endl;
			cout << "Directory Content: "<<endl;
			displayDirectory(currentPath);
			displayFiles(currentPath);
			cout << endl;
			cout << "Specify your destination (Press Enter if you are already in the destination directory): \n";
			getline(cin,str);
		}
	}         	
}

int main()
{
	string fileName;
    char option;
    int answer=0;

    string currentPath = "C:\\Users\\FlyHercules\\Downloads";

	cout << "Would you like to enter a default directory or use the default program directory? (y/n): \n";
    cin >> option;

    if(option=='y' || option=='Y' || option=='YES' || option=='yes')
    {
        cout << "Specify your destination (Press Enter if you are already in the destination directory): \n";
        getline(cin,currentPath);
    }
	cout<<endl;

    while(true)
    {
		cout << "Current Directory: "<<currentPath<<endl;
		cout << "Directory Content: "<<endl;
		displayDirectory(currentPath);
		displayFiles(currentPath);
		cout << endl;
        displayMenu();
        cin >> answer;
		cin.ignore();
		cout << endl;

        if(answer==0)
            break;
        else if(answer==1)
        {
            changeDirectory(currentPath);
			cout << "Press enter to continue: "<<endl;
			cin.ignore(); 			
        }
        else if(answer == 2)
        {
			copyFile(currentPath);
            cout << "Press enter to continue: "<<endl;
			cin.ignore(); 	
        }
        else if(answer==3)
        {
            moveFile(currentPath);
            cout << "Press enter to continue: "<<endl;
            cin.ignore();
        }
		else if(answer==4) //Issues
		{
			cout << "Enter filename with extension (eg .txt): ";
			getline(cin, fileName);
			
			while(!fs::exists(currentPath+'\\'+fileName))
			{
				cout << "Invalid file. Please enter a valid file: ";
				getline(cin,fileName);
			}
			string command = "start " + currentPath + '\\' + fileName; 
			if(system(command.c_str())==0)
				cout << "File was opened successfully!"<<endl;
			else
				cout << "File was not opened"<<endl;
			cout << "Press enter to continue: "<<endl;
			cin.ignore();
		}
        else if(answer==5)
        {
			string name;
			cout << "Enter new file name: ";
			getline(cin, name);
			while(pathExist(currentPath+'\\'+name))
			{
				string temp;
				cout << "Path already exists. Would you like to remove and over-write previous path with new one? (y/n): \n";
				getline(cin,temp);
				if(temp=="y")
				{
					fs::remove(currentPath+'\\'+name);
					try {			
						fs::create_directory(currentPath+'\\'+name);
						cout << "Directory was created successfully." << endl;
					} catch (const fs::filesystem_error& e) {
						cerr << "Filesystem error: " << e.what() << endl;
					} catch (const std::exception& e) {
						cerr << "Exception: " << e.what() << endl;
    				}
					cout << "Press enter to continue: "<<endl;
					cin.ignore();
					break;
				}
				else
				{
					cout << "Enter new file name: \n";
					getline(cin, name);
				}
			}
			
        }
    }	

	return 0;
}