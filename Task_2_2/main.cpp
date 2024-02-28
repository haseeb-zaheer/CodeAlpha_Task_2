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
    cout<<endl;
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
    cout<<endl;
}
void printFolders(const string directory)
{
    for (const auto& entry : fs::directory_iterator(directory)) 
    {
        if(fs::is_directory(entry))
            cout << entry.path().filename() <<endl;
    }
    cout << endl;
}
void navigate(string& currentDirectory)
{
    string temp;
    cout << "Current directory: " << currentDirectory << endl;
    printFolders(currentDirectory);
    cout << "Enter folder name to open it in current directory (to go back a folder, enter '..')\n(Enter nothing to enter full directory): ";
    cout << "Examples:"<<endl;
    cout << "1. Input: 'C:\\Users\\user\\Desktop' if folder is not in current directory"<<endl;
    cout << "2. Input: 'Desktop' if folder is in current directory"<<endl;
    cout << "3. Input: '..' if you would like to go a folder back in directory"<<endl;
    cout << "4. Input: '' if you would like to abort"<<endl;
    cout << "Enter new directory: ";
    getline(cin, temp);
    
    while (true)
    {
        if(pathExist(temp) && temp!="..")
        {
            currentDirectory = temp;
            cout << "Navigated to directory: " << currentDirectory << endl;
            return;
        }
        else if(temp.size()==0)
        {
            cout << "Path not modified"<<endl;
            return;
        }
        else if(temp == "..")
        {
            size_t backSlash = currentDirectory.rfind("\\");
            if(backSlash!=string::npos)
                currentDirectory.erase(backSlash);
            else
                cout << "Path not modified"<<endl;
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


    if(destinationPath.find(currentPath) != string::npos)
    {
        cout << "Cannot copy folder to itself." << endl;
        return;
    }
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

    if(destinationPath.find(currentPath) != string::npos)
    {
        cout << "Cannot copy file to itself." << endl;
        return;
    }
    if(fs::exists(destination))
        fs::remove(destination);

    fs::copy_file(currentPath, destination);
    cout << "File successfully copied." << std::endl;
}
void moveFolder(string& currentPath, const string destinationPath)
{
    fs::path currentPathObj(currentPath);
    fs::path destination = fs::path(destinationPath) / currentPathObj.filename();

    if(destinationPath.find(currentPath) != string::npos)
    {
        cout << "Cannot move folder to itself." << endl;
        return;
    }
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

    if(destinationPath.find(currentPath) != string::npos)
    {
        cout << "Cannot move file to itself." << endl;
        return;
    }
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
    cout << "Examples: "<<endl;
    cout << "1. Input: 'folderName' to create a folder of folderName in current directory"<<endl;
    cout << "2. Input: '' to abort operation"<<endl;
    cout << "Enter new directory name: ";
    getline(cin, newDirectory);
    while (true)
    {
        if(newDirectory.size()==0)
        {
            cout << "Directory not created." << endl;
            break;
        }
        else if(fs::exists(currentPath+'\\'+newDirectory))
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
    string currentDirectory = "c:\\Users";  // starting directory
    cout <<endl;
    cout << "Welcome to the file system navigator." << endl;
    cout << "These are the commands you can use: " << endl << endl;
    printHelp();
    while (true)
    {
        cout << "Enter command ('exit' to quit or 'help' for list of commands): ";
        getline(cin, command);

        transform(command.begin(), command.end(), command.begin(), ::tolower);
        cout << endl;
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
            bool abort=false;
            bool isFile = false;
            string sourcePath = currentDirectory, destinationPath;
            cout << "Current directory: " << sourcePath << endl;
            cout << "Folders/Files in current directory: " <<endl;
            printDirectory(sourcePath);
            cout << "Examples: " <<endl;
            cout << "1. Input: 'C:\\Users\\user\\Desktop\\file.txt' if file is not in current directory"<<endl;
            cout << "2. Input: 'C:\\Users\\user\\Desktop\\folder' if folder to be copied is not current directory"<<endl;
            cout << "3. Input: 'file.txt' if file is present in current directory"<<endl;
            cout << "4. Input: 'copy dir' to copy the current directory"<<endl;
            cout << "5. Input: Empty input if you would like to abort operation"<<endl;
            cout << "Enter source path: ";
            getline(cin, sourcePath);
            while (true)
            {
                if(sourcePath.size()==0)
                {
                    abort = true;
                    break;
                }
                if(sourcePath == "copy dir")
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

            if(!abort)
            {
                cout << "Enter destination path (e.g 'C:\\Users\\user\\Desktop'): ";
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
        }
        else if (command == "move")
        {
            bool abort = false;
            bool isFile = false;
            string sourcePath = currentDirectory, destinationPath;
            cout << "Current directory: " << sourcePath << endl;
            cout << "Folders/Files in current directory: " <<endl;
            printDirectory(sourcePath);
            cout << "Examples: " <<endl;
            cout << "1. Input: 'C:\\Users\\user\\Desktop\\file.txt' if file is not in current directory"<<endl;
            cout << "2. Input: 'C:\\Users\\user\\Desktop\\folder' if folder to be moved is not current directory"<<endl;
            cout << "3. Input: 'file.txt' if file is present in current directory"<<endl;
            cout << "4. Input: 'move dir' to copy the current directory"<<endl;
            cout << "5. Input: Empty input if you would like to abort operation"<<endl;
            cout << "Enter source path: ";
            getline(cin, sourcePath);
            while (true)
            {
                if(sourcePath.size()==0)
                {
                    abort = true;
                    break;
                }
                else if(sourcePath == "move dir")
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

            if(!abort)
            {
                cout << "Enter destination path: ";
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
        }
        else if (command == "open")
        {
            bool open = true;
            string fileName;
            
            cout << "Current directory: " << currentDirectory << endl;
            cout << "Files in current directory: " <<endl;
            printDirectory(currentDirectory);
            cout << "Enter file name with extension (empty input to abort): ";            
            getline(cin, fileName);
            if(fileName.size()==0)
            {
                cout << "Operation aborted."<<endl;
                open = false;
                continue;
            }
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
