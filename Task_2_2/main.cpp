#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void displayDirectory(string currentPath)
{
	cout<<endl;
    cout << "Current Directory Path: " << currentPath <<endl;
	cout << "Directory Content: "<<endl;
    for (const auto& entry : fs::directory_iterator(currentPath)) 
	    cout << entry.path().filename() << endl;
	cout <<endl;
}

int main()
{
	ifstream fileInput;
	ofstream fileOutput;
	string src, dest, fileName;
    char option;
    bool keepRunning = true;
    int answer=0;

    string currentPath = "C:\\Users";

	cout << "Would you like to enter a default directory or use the default program directory? (y/n): ";
    cin >> option;

    if(option=='y')
    {
        cout << "Enter a directory path: ";
        getline(cin,currentPath);
    }

    while(keepRunning)
    {
		displayDirectory(currentPath);
        cout << "1. To navigate directories."<<endl;
        cout << "2. To copy file from current directory."<<endl;
        cout << "3. To move file from current directory."<<endl;
        cout << "4. To open a file from current directory."<<endl;
        cout << "5. To create a directory in current path."<<endl;
        cout << "0. To exit program."<<endl;
        cin >> answer;
        cin.ignore();

        if(answer==0)
            return 0;
        else if(answer==1)
        {
            string str;
            cout << "Enter where you would like to go: ";
            getline(cin,str);
            
            currentPath.push_back('\\');
            currentPath += str;
        }
        else if(answer == 2)
        {
			string destinationPath = "C:\\Users";
			string temp = "";
			displayDirectory(destinationPath);

			cout << "Please navigate to where you would like to go (Press Enter without any input \nif you have opened destination directory)";
			getline(cin, temp);
			cout << temp.size() << endl;
			if(temp.size()==0)
			{
				
			}
			else
			{
				
			}
        }

        keepRunning = true;
    }	

	cout << "Input file directory: ";
	getline(cin, src);
	cout << "Input destination directory: ";
	getline(cin, dest);
	cout << "Enter filename with extension (eg .txt): ";
	getline(cin, fileName);

	// this will add the source file name to the directory so it can be opened and copied
	if (src[src.size() - 1] != '/')
	{
		src.push_back('\\');
	}
	src += fileName;
	fileInput.open(src, ios::binary);
	if (!fileInput.is_open())
	{
		cerr << "Failed to open input file" << endl;
		return 1;
	}

	if (dest[dest.size() - 1] != '/')
	{
		dest.push_back('\\');
	}
	dest += fileName;
	fileOutput.open(dest, ios::binary);
	if (!fileOutput.is_open())
	{
		cerr << "Failed to open output file" << endl;
		return 1;
	}

	string input;
	while (getline(fileInput, input))
		fileOutput << input << endl;
	fileOutput.flush();
	cout << "Copied succesfully" << endl;

	fileInput.close();
	fileOutput.close();

	return 0;
}