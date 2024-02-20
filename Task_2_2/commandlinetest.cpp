#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <windows.h>
#include <cstdint>

using namespace std;
namespace fs = std::filesystem;

void printHelp()
{
    cout << "navigate - to navigate directories." << endl;
    cout << "dir - to print current directory" << endl;
    cout << "list - to print files in current directory." << endl;
    cout << "copy - to copy file from current directory." << endl;
    cout << "move - to move file from current directory." << endl;
    cout << "open - to open a file from current directory." << endl;
    cout << "create - to create a new directory in current path." << endl;
    cout << "exit - to exit program." << endl;
    cout << endl;
}
bool pathExist(const string str)
{
    return fs::exists(str) && fs::is_directory(str);
}
bool fileExist(const string str)
{
    return fs::exists(str) && fs::is_regular_file(str);
}
void printDirectory(const string directory)
{
    for (const auto& entry : fs::directory_iterator(directory)) 
        cout << entry.path().filename() <<endl;
}
void printFolders(const string directory)
{
    for (const auto& entry : fs::directory_iterator(directory)) 
    {
        if(fs::is_directory(entry))
            cout << entry.path().filename() <<endl;
    }
}
void navigate(string& currentDirectory)
{
    string temp;
    cout << "Current directory: " << currentDirectory << endl;
    printFolders(currentDirectory);
    cout << "Enter folder name to open it in current directory (to go back a folder, enter '..')\n(Enter nothing to enter full directory): ";
    getline(cin, temp);
    
    while (true)
    {
        if(temp.size()==0)
        {
            cout << "Enter full directory: ";
            getline(cin,temp);
            if (pathExist(temp))
            {
                currentDirectory = temp;
                cout << "Navigated to directory: " << currentDirectory << endl;
                return;
            }
        }
        else if(temp == "..")
        {
            size_t backSlash = currentDirectory.rfind("\\");
            if(backSlash!=string::npos)
                currentDirectory.erase(backSlash);
            else
                cout << "String not modified"<<endl;
            cout << "Navigated to directory: " << currentDirectory << endl;
            return;
        }
        else if(pathExist(currentDirectory + '\\' + temp))
        {
            currentDirectory = currentDirectory + '\\' + temp;
            cout << "Navigated to directory: " << currentDirectory << endl;
            return;
        }
        cout << "Invalid directory. Please try again." << endl;
        cout << "Enter new directory: ";
        getline(cin,temp);
    }  
}


void copyFolder(const string currentPath, const string destinationPath)
{
    fs::path currentPathObj(currentPath);
    fs::path destination = fs::path(destinationPath) / currentPathObj.filename();

    if(fs::exists(destination))
        fs::remove_all(destination);
    fs::create_directories(destination);

    try {
        for (const auto& entry : fs::directory_iterator(currentPath)) {
            const fs::path currentPath2 = entry.path();
            const fs::path newPath = destination / currentPath2.filename();

            if (fs::is_directory(currentPath2))
                copy(currentPath2, newPath);
            else 
                fs::copy_file(currentPath2, newPath, fs::copy_options::overwrite_existing); //recursive call
        }
        cout << "Folder successfully copied." << std::endl;
    } catch (const fs::filesystem_error& e) {
        cerr << "Error copying file/directory: " << e.what() << endl;
    }
}
void copyFile(const string currentPath, const string destinationPath)
{
    fs::path currentPathObj(currentPath);
    fs::path destination = fs::path(destinationPath) / currentPathObj.filename();

    if(fs::exists(destination))
        fs::remove(destination);

    fs::copy_file(currentPath, destination);
    cout << "File successfully copied." << std::endl;
}
void moveFolder(string& currentPath, const string destinationPath)
{
    fs::path currentPathObj(currentPath);
    fs::path destination = fs::path(destinationPath) / currentPathObj.filename();

    if(fs::exists(destination))
        fs::remove_all(destination);
    fs::create_directories(destination);

    try {
        for (const auto& entry : fs::directory_iterator(currentPath)) {
            const fs::path currentPath2 = entry.path();
            const fs::path newPath = destination / currentPath2.filename();

            if (fs::is_directory(currentPath2))
                copy(currentPath2, newPath);
            else 
                fs::copy_file(currentPath2, newPath, fs::copy_options::overwrite_existing); //recursive call
        }
        fs::remove_all(currentPath);
        size_t backSlash = currentPath.rfind("\\");
		if(backSlash!=string::npos)
			currentPath.erase(backSlash);
        cout << "Folder successfully moved." << std::endl;
    } catch (const fs::filesystem_error& e) {
        cerr << "Error moving file/directory: " << e.what() << endl;
    }
}
void moveFile(const string currentPath, const string destinationPath)
{
    fs::path currentPathObj(currentPath);
    fs::path destination = fs::path(destinationPath) / currentPathObj.filename();

    if(fs::exists(destination))
        fs::remove(destination);

    fs::copy_file(currentPath, destination);
    fs::remove(currentPath);
    cout << "File successfully moved." << std::endl;
}
void createDirectory(const string currentPath)
{
    string newDirectory;
    cout << "Current Directory: "<<currentPath<<endl;
    cout << "Current Directory Folders: "<<endl;
    printFolders(currentPath);
    cout << "Enter new directory name: ";
    getline(cin, newDirectory);
    while (true)
    {
        if(fs::exists(currentPath+'\\'+newDirectory))
            cout << "Directory already exists. Please try again." << endl;
        else
        {
            fs::create_directory(currentPath+'\\'+newDirectory);
            cout << "Directory successfully created." << endl;
            break;
        }
        cout << "Enter new directory name: ";
        getline(cin, newDirectory);
    }
}

int main()
{
    string command;
    string currentDirectory = "c:\\Users\\FlyHercules\\Downloads";
    while (true)
    {
        cout << "Enter command ('exit' to quit or 'help' for list of commands): ";
        getline(cin, command);

        transform(command.begin(), command.end(), command.begin(), ::tolower);
        if(command == "exit")
            return 0;
        else if (command == "help")
            printHelp();
        else if (command == "navigate")
            navigate(currentDirectory);
        else if (command == "dir")
            cout << "Current directory: " << currentDirectory << endl;
        else if (command == "list")
            printDirectory(currentDirectory);
        else if (command == "copy")
        {
            bool isFile = false;
            string sourcePath = currentDirectory, destinationPath;
            cout << "Current directory: " << sourcePath << endl;
            cout << "Enter source path (Press Enter if you are already in the current directory to copy from) \n(Include filename if you want to copy a file)\n: ";
            getline(cin, sourcePath);
            while (true)
            {
                if(sourcePath.size()==0)
                {
                    sourcePath = currentDirectory;
                    break;
                }
                if(fileExist(sourcePath))
                {
                    isFile = true;
                    break;
                }
                else if (pathExist(sourcePath))
                    break;
                cout << "Invalid directory. Please try again." << endl;
                cout << "Enter source directory: ";
                getline(cin,sourcePath);
            }

            cout << "Enter destination path\n: ";
            getline(cin, destinationPath);
            while (true)
            {
                if(pathExist(destinationPath))
                    break;
                cout << "Invalid directory. Please try again." << endl;
                cout << "Enter destination directory: ";
                getline(cin,destinationPath);
            }
            if(!isFile)
                copyFolder(sourcePath, destinationPath);
            else
                copyFile(sourcePath, destinationPath);
            cout << "Press enter to continue: "<<endl;
            cin.ignore();
        }
        else if (command == "move")
        {
            bool isFile = false;
            string sourcePath = currentDirectory, destinationPath;
            cout << "Current directory: " << sourcePath << endl;
            cout << "Enter source path (Press Enter if you are already in the current directory to move from) \n(Include filename if you want to move a file)\n: ";
            getline(cin, sourcePath);
            while (true)
            {
                if(sourcePath.size()==0)
                {
                    sourcePath = currentDirectory;
                    break;
                }
                if(fileExist(sourcePath))
                {
                    isFile = true;
                    break;
                }
                else if (pathExist(sourcePath))
                    break;
                cout << "Invalid directory. Please try again." << endl;
                cout << "Enter source directory: ";
                getline(cin,sourcePath);
            }

            cout << "Enter destination path\n: ";
            getline(cin, destinationPath);
            while (true)
            {
                if(pathExist(destinationPath))
                    break;
                cout << "Invalid directory. Please try again." << endl;
                cout << "Enter destination directory: ";
                getline(cin,destinationPath);
            }
            if(!isFile)
                moveFolder(sourcePath, destinationPath);
            else
                moveFile(sourcePath, destinationPath);
            currentDirectory = sourcePath;
            cout << "Press enter to continue: "<<endl;
            cin.ignore();
        }
        else if (command == "open")
        {
            bool open = true;
            cout << "Enter file name with extension: ";
            string fileName;
            getline(cin, fileName);
            while(!fs::exists(currentDirectory+'\\'+fileName))
            {
                cout << "Invalid file. Please enter a valid file (or enter nothing to abort operation): ";
                getline(cin,fileName);
                if(fileName.size()==0)
                {
                    open = false;
                    break;
                }
            }
            if(open)
            {
                string temp = currentDirectory + '\\' + fileName;
                const char* filePath = temp.c_str();
                HINSTANCE result = ShellExecuteA(NULL, "open", filePath, NULL, NULL, SW_SHOWNORMAL);

                if (reinterpret_cast<intptr_t>(result) > 32)
                    cout << "File opened successfully."<<endl;
                else
                    cerr << "Error opening file. Error code: " << reinterpret_cast<intptr_t>(result) << endl;
            }
            
            cout << "Press enter to continue: "<<endl;
            cin.ignore();
        }
        else if (command == "create")
            createDirectory(currentDirectory);
        else
            cout << "Invalid command. Please try again." << endl;
        
        cout << endl;
    }
}
