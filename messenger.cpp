#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <filesystem>
#include <thread> 
#include <windows.h>
#include <mutex> // Added for file locking
#include <chrono> // Added for sleep duration 
#include <conio.h>  // For _getch()


using namespace std;
std::mutex fileMutex;

string log_file = "LOCATION OF FILE";
string folder_location = "LOCATION OF FILE";


class NextClass {
public:
    void display() {
        cout << " " << endl;
    }
};

class Welcome {
private:
    char x;
    HANDLE hconsole;
    int colorcode;

    void changeColor() {
        for (int i = 0; i < 5; ++i) {
            SetConsoleTextAttribute(hconsole, colorcode);
            ++colorcode;
            Sleep(5);
        }
    }

    void clearScreen() {
        COORD topLeft = { 0, 0 };
        CONSOLE_SCREEN_BUFFER_INFO screen;
        DWORD written;

        GetConsoleScreenBufferInfo(hconsole, &screen);
        FillConsoleOutputCharacterA(
            hconsole, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
        );
        FillConsoleOutputAttribute(
            hconsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
            screen.dwSize.X * screen.dwSize.Y, topLeft, &written
        );
        SetConsoleCursorPosition(hconsole, topLeft);
    }

public:
    Welcome(char z) : x(z), colorcode(5) {
        hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    void animation() {
        changeColor();

        for (int i = 0; i < 112; i++) {
            cout << x << x << x;
            cout << x << x << x;
            Sleep(5);
        }

        // Get console width
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int console_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;

        string project_name = "C O M N I T R O N";
        int padding = (console_width - project_name.length()) / 2;

        cout << endl;
        cout << string(padding, ' ') << project_name << endl;
        cout << endl;

        for (int i = 0; i < 112; i++) {
            cout << x << x << x;
            cout << x << x << x;
            Sleep(5);
        }
    }

    void pressAnyKeyToContinue() {
        cout << "\nPress any key to continue...";
        _getch();  // Waits for a key press
        clearScreen();
    }
};

// FileManger class definition
class FileManger {
private:
    string directory;

public:
    FileManger() { directory = folder_location; }

    void creat_file(string File_name) {
        lock_guard<mutex> lock(fileMutex); // Lock the file
        fstream myfile(directory + "\\" + File_name + ".txt", ios::app);
        if (!myfile.is_open()) {
            myfile << " ";
        }
        myfile.close();
    }

    string Read_a_line(string File_name, int line) {
        lock_guard<mutex> lock(fileMutex); // Lock the file
        fstream myfile(directory + "\\" + File_name + ".txt", ios::in);
        string output;
        if (myfile.is_open()) {
            string temp;
            int currentLine = 1;
            while (currentLine < line && getline(myfile, temp)) {
                currentLine++;
            }
            if (getline(myfile, output)) {
                myfile.close();
                return output;
            }
            else {
                myfile.close();
                return "";
            }
        }
        else {
            cerr << "Failed to open the file." << endl;
            return "";
        }
    }
    string Read_a_lineRF(string File_name, string reference_point, int lines_after) {
        fstream myfile(directory + "\\" + File_name + ".txt", ios::in);
        string output;
        bool foundReference = false;

        if (myfile.is_open()) {
            string temp;
            // Search for the reference point
            while (getline(myfile, temp)) {
                if (temp == reference_point) {
                    foundReference = true;
                    break;
                }
            }

            if (foundReference) {
                // cout << "Reference point found: " << reference_point << endl;
                 // Read the desired number of lines after the reference point
                for (int i = 0; i < lines_after; ++i) {
                    if (!getline(myfile, output)) {
                        cerr << "Failed to read line after reference point: " << reference_point << endl;
                        myfile.close();
                        return "";
                    }
                }
                // cout << "Line read after " << lines_after << " lines: " << output << endl;
                myfile.close();
                return output;
            }
            else {
                cerr << "Reference point not found in the file: " << reference_point << endl;
                myfile.close();
                return "";
            }
        }
        else {
            cerr << "Failed to open the file: " << File_name << endl;
            return "";
        }
    }

    void Read_Whole(string File_name, int line) {
        ifstream myfile(directory + "\\" + File_name + ".txt", ios::in);
        string output;
        if (myfile.is_open()) {
            int currentLine = 1;
            int find = line;

            while (getline(myfile, output)) {
                if (currentLine == find) {
                    cout << output << endl;
                    find = find + 4;
                }
                currentLine++;
            }

            myfile.close();
        }
        else {
            cerr << "Failed to open the file." << endl;
        }
    }

    bool Exist_in_file(string filename, string search)
    {
        fstream myFile(folder_location + "\\" + filename + ".txt", ios::in);
        string line;

        if (myFile.is_open())
        {
            while (getline(myFile, line))
            {
                if (line == search)
                {
                    myFile.close();
                    return true;
                }
            }

            return false;
        }
        else
        {
            cerr << "Failed to open the file." << endl;
            return false;
        }
    }


    void Add_in_file(string FileName, string add_element)
    {
        fstream File(folder_location + "\\" + FileName + ".txt", ios::app);
        if (File.is_open())
        {
            File << add_element << endl; // Add newline after each element
            File.close();
        }
        else
        {
            cerr << "Failed to open the file." << endl;
        }
    }


    void Push_line(string filename, string referancepoint, int line_number, string newline)
    {
        ifstream readfile(folder_location + "\\" + filename + ".txt");
        ofstream tempfile(folder_location + "\\temp.txt");

        string line;

        if (!readfile.is_open() || !tempfile.is_open())
        {
            cerr << "Failed to open files for reading/writing." << endl;
            return;
        }

        bool reference_found = false;
        int current_line = 0;

        while (getline(readfile, line))
        {
            if (line == referancepoint)
            {
                reference_found = true;
                tempfile << line << endl; // Write the reference point
                for (int i = 0; i < line_number - 1; ++i)
                {
                    if (!getline(readfile, line))
                    {
                        cerr << "Line number exceeds the end of file." << endl;
                        readfile.close();
                        tempfile.close();
                        remove((folder_location + "\\temp.txt").c_str());
                        return;
                    }
                    tempfile << line << endl; // Write lines before the target line
                }
                tempfile << newline << endl; // Write the new line
            }
            else
            {
                tempfile << line << endl; // Write other lines unchanged
            }
        }

        if (!reference_found)
        {
            cerr << "Reference point not found in the file." << endl;
        }

        readfile.close();
        tempfile.close();

        // Remove the original file
        if (remove((folder_location + "\\" + filename + ".txt").c_str()) != 0)
        {
            cerr << "Error deleting original file." << endl;
            return;
        }

        // Rename the temporary file to the original file name
        if (rename((folder_location + "\\temp.txt").c_str(), (folder_location + "\\" + filename + ".txt").c_str()) != 0)
        {
            cerr << "Error renaming temporary file." << endl;
            return;
        }

        cout << "Line replaced successfully." << endl;
    }

    void replace_line(string filename, string reference_point, int line_number, string new_line)
    {
        ifstream readfile(folder_location + "\\" + filename + ".txt");
        ofstream tempfile(folder_location + "\\temp.txt");

        string line;
        int current_line = 0;

        if (!readfile.is_open() || !tempfile.is_open())
        {
            cerr << "Failed to open files for reading/writing." << endl;
            return;
        }

        bool reference_found = false;

        while (getline(readfile, line))
        {
            if (line == reference_point)
            {
                reference_found = true;
                tempfile << line << endl; // Write the reference point

                for (int i = 0; i < line_number - 1; ++i)
                {
                    if (!getline(readfile, line))
                    {
                        cerr << "Line number exceeds the end of file." << endl;
                        readfile.close();
                        tempfile.close();
                        remove((folder_location + "\\temp.txt").c_str());
                        return;
                    }
                    tempfile << line << endl; // Write lines before the target line
                }

                // Skip the line to be replaced
                getline(readfile, line);

                // Write the new line
                tempfile << new_line << endl;
            }
            else
            {
                tempfile << line << endl; // Write other lines unchanged
            }
        }

        if (!reference_found)
        {
            cerr << "Reference point not found in the file." << endl;
        }

        readfile.close();
        tempfile.close();

        // Remove the original file
        if (remove((folder_location + "\\" + filename + ".txt").c_str()) != 0)
        {
            cerr << "Error deleting original file." << endl;
            return;
        }

        // Rename the temporary file to the original file name
        if (rename((folder_location + "\\temp.txt").c_str(), (folder_location + "\\" + filename + ".txt").c_str()) != 0)
        {
            cerr << "Error renaming temporary file." << endl;
            return;
        }

    }


    bool File_Exist(const string file_name) {
        // Construct the file path based on the username and location
        string filePath = directory + "/" + file_name + ".txt";

        // Open the file
        ifstream file(filePath.c_str());

        // Check if the file exists and can be opened
        if (file.good()) {
            file.close(); // Close the file
            return true;
        }
        else {
            return false;
        }
    }

};

class login_info {
protected:
    string Username;
    string password;
    string email;
    string status;
    FileManger f;
public:
    login_info() {
        Username = " ";
        password = " ";

    }

    bool login() {
        ifstream file(log_file, ios::in);
        if (file.is_open()) {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            bool found = false;
            string cpassword;
            string line;
            string mail;
            string stat;

            while (getline(file, line)) {
                if (line == username) {
                    getline(file, cpassword); // Read the password
                    getline(file, mail);     // Read the email
                    getline(file, stat);     // Read the status
                    if (password == cpassword) {
                        found = true;
                        break;
                    }
                    else {
                        cout << "Incorrect password. Please try again.\n";
                        return false;
                    }
                }
            }

            file.close(); // Close the input file

            if (found) {
                cout << "Login successful!\n";
                Username = username;
                email = mail;
                status = "online"; // Set status to "online"
                f.replace_line("loginfo", username, 3, "online");
                // Optionally update the user's status to "online" in memory or perform other actions
                return true;
            }
            else {
                cout << "Username not found. Please check your username or register.\n";
                return false;
            }
        }
        else {
            cout << "Unable to open file.\n";
            return false;
        }
    }




    void Register() {
        ofstream file(log_file, ios::app);
        string username, password, email;

        bool usernameExists = false; // Define usernameExists outside of the loop

        // Username validation
        do {
            ifstream infile(log_file); // Open the file for reading
            string line;

            cout << "Enter Username: ";
            cin >> username;

            // Check if the username is empty
            if (username.empty()) {
                cout << "Username cannot be empty. Please try again.\n";
                continue; // Skip the rest of the loop and start over
            }

            // Check if the username already exists in the file
            while (getline(infile, line)) {
                if (line == username) {
                    usernameExists = true;
                    break;
                }
                // Skip other lines (password, email, status)
                getline(infile, line); // password
                getline(infile, line); // email
                getline(infile, line); // status
            }

            infile.close(); // Close the file

            // If the username already exists, prompt the user to choose another username
            if (usernameExists) {
                cout << "Username already exists. Please choose another username.\n";
            }
        } while (username.empty() || usernameExists);



        // Password validation
        do {
            cout << "Enter Password (at least 6 characters): ";
            cin >> password;
            if (password.length() < 6) {
                cout << "Password must be at least 6 characters long. Please try again.\n";
            }
        } while (password.length() < 6);

        // Email validation
        regex email_regex("^[\\w-\\.]+@([\\w-]+\\.)+[\\w-]{2,4}$");
        do {
            cout << "Enter e-mail: ";
            cin >> email;
            if (!regex_match(email, email_regex)) {
                cout << "Invalid email format. Please try again.\n";
            }
        } while (!regex_match(email, email_regex));



        if (file.is_open()) {
            file << username << endl;
            file << password << endl;
            file << email << endl;
            file << "offline" << endl; // Set initial status to "offline"

            file.close();

            cout << "Registration successful!\n";
        }
        else {
            cout << "Unable to open file for registration.\n";
        }
    }
};

class messaging_system {
private:
    string sender;
    string reciver;
    FileManger file;

public:
    messaging_system()
    {
        sender = " ";
        reciver = " ";



    }

    void sending_messages(string user1, string user2, string messages)
    {
        sender = user1;
        reciver = user2;

        // Check if the message content is empty
        if (messages.empty()) {
            // cout << "Cannot send blank message." << endl;
            return;
        }

        //File name of the users
        string sortedUsernames = sender + reciver;
        sort(sortedUsernames.begin(), sortedUsernames.end());

        //history file
        if (!(file.File_Exist(sortedUsernames)))
        {
            file.creat_file(sortedUsernames);
        }

        //message of sender
        if (!(file.File_Exist(sender + " messagefile")))
        {
            file.creat_file(sender + " messagefile");
        }

        //notification
        string notifiction = sortedUsernames + " " + sender + "_notification";
        if (!(file.File_Exist(notifiction)))
        {
            file.creat_file(notifiction);
        }

        // Add message only if it's not empty
        if (!messages.empty()) {
            file.Add_in_file(sortedUsernames, sender + " : " + messages);
            file.Add_in_file(sender + " messagefile", sender + " : " + messages);
            file.Add_in_file(notifiction, "New Message");
        }
    }


    void Read_messages(const string& sender, const string& receiver) {
        // Sort the usernames to construct the filename
        string sortedUsernames = sender + receiver;
        sort(sortedUsernames.begin(), sortedUsernames.end());

        // Construct the filename based on sorted usernames
        string filename = folder_location + "\\" + sortedUsernames + ".txt";

        // Open the file for reading
        ifstream readFile(filename);

        // Check if the file opened successfully
        if (readFile.is_open()) {
            string line;
            // Read and display each line in the file
            while (getline(readFile, line)) {
                // Extract the sender's username from the message
                string senderUsername = line.substr(0, line.find(":"));
                // Print the sender's username in green
                cout << "\033[0;32m" << senderUsername << ":\033[0m " << line.substr(senderUsername.length() + 2) << endl;
            }
            // Close the file
            readFile.close();
        }
        else {
            // Display error message if the file couldn't be opened
            cerr << "Error: Unable to open file " << filename << " for reading." << endl;
        }
    }


    void read_newMessage(const string& receiver, const string& sender) {
        // Sort the usernames to construct the filenames
        string sortedUsernames = sender + receiver;
        sort(sortedUsernames.begin(), sortedUsernames.end());

        // Construct the filename for the notification file
        string notificationFile = folder_location + "\\" + sortedUsernames + " " + sender + "_notification.txt";
        // Construct the filename for the sender's message file
        string senderMessageFile = folder_location + "\\" + sender + " messagefile.txt";

        // Open the notification file for reading
        ifstream notificationRead(notificationFile);
        // Check if the file opened successfully
        if (notificationRead.is_open()) {
            string line;
            // Read the first line to check if there's a new message
            if (getline(notificationRead, line) && line == "New Message") {
                // Display any new messages if they exist
                ifstream messageRead(senderMessageFile);
                if (messageRead.is_open()) {
                    while (getline(messageRead, line)) {
                        cout << line << endl;
                    }
                    messageRead.close();
                }
                else {
                    cerr << "Error: Unable to open message file " << senderMessageFile << " for reading." << endl;
                }
                // Clear the sender's message file
                ofstream messageClear(senderMessageFile);
                if (messageClear.is_open()) {
                    messageClear.close();
                }
                else {
                    cerr << "Error: Unable to open message file " << senderMessageFile << " for clearing." << endl;
                }
                // Clear the notification file
                ofstream notificationClear(notificationFile);
                if (notificationClear.is_open()) {
                    notificationClear.close();
                }
                else {
                    cerr << "Error: Unable to open notification file " << notificationFile << " for clearing." << endl;
                }
            }
            notificationRead.close();
        }
        else {
            cerr << "Error: Unable to open notification file " << notificationFile << " for reading." << endl;
        }
    }



};

class User : public login_info {
private:

    FileManger file;


public:
    User() : login_info() {
        email = " ";
        status = " ";
    }

    string getUsername() const {
        return Username;
    }

    string getEmail() const {
        return email;
    }

    string getStatus() const {
        return status;
    }

    void setEmail(string x) {
        email = x;
    }

    void setStatus(string x) {
        status = x;
    }

    void view_friends_list() {

        file.creat_file(Username + " Friends");
        cout << "List of Users\n";
        file.Read_Whole("loginfo", 1);



    }

    void add_friends(string name)
    {
        bool exist = false;
        bool Newfriend = false;


        exist = file.Exist_in_file("loginfo", name);
        Newfriend = !(file.Exist_in_file(Username + " Friends", name));

        if (exist && Newfriend)
        {
            file.Add_in_file(Username + " Friends", name);
            cout << "User added Successfully!";

        }
        else if (exist == false)
        {
            cout << "No user found please try again\n";


        }
        else if (Newfriend == false)
        {
            cout << "User already a Friend\n";


        }



    }

    void message_friends() {
        FileManger file;
        int friends = 1;
        string currentFriend;
        string status;

        while (true) {
            currentFriend = file.Read_a_line(Username + " Friends", friends);
            if (currentFriend.empty()) {
                break;
            }


            status = file.Read_a_lineRF("loginfo", currentFriend, 3);
            if (status.empty()) {
                cout << "Failed to retrieve status for: " << currentFriend << endl;
            }
            else {
                cout << currentFriend << ": " << status << endl;
            }
            friends++;
        }

        cout << "Please select a friend: ";
    }


    void Exit()
    {
        file.replace_line("loginfo", Username, 3, "offline");
    }


};

int main() {
    User u1;
    FileManger j;

    Welcome wlc(222);
    wlc.animation();
    wlc.animation();

    wlc.pressAnyKeyToContinue();
    NextClass next;
    next.display();

    bool loggedIn = false;
    cout << "~~~~~";
    cout << "\033[1;34mWelcome to CommuniTron\033[0m";
    cout << "~~~~~\n";
    while (!loggedIn) {
        int choice;

        cout << "\033[0;33mWould you like to:\n1. Login\n2. Register\033[0m\n";
        cin >> choice;
        cout << "\n";

        if (choice == 1) {
            loggedIn = u1.login();
        }
        else if (choice == 2) {
            u1.Register();
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }
        cout << "\n";
    }

    cout << "\033[0;32m\nWelcome " << u1.getUsername() << "!\n\033[0m" << endl;


    bool end_program = false;

    while (!end_program)
    {
        int choice;
        cout << "\033[0;33mWould you like to:\n";
        cout << "1. Add friends\n";
        cout << "2. Message friends\n";
        cout << "3. Exit\n\033[0m";
        cin >> choice;
        cout << "\n";

        switch (choice)
        {
        case 1:
            while (true)
            {
                string name;
                u1.view_friends_list();
                cout << "Please enter name to add to friends or type -1 to exit: ";
                cin >> name;
                if (name == "-1")
                {
                    break;
                }
                u1.add_friends(name);
                cout << "\n";
            }
            cout << "\n";
            break;

        case 2:
        {
            string receiver;

            u1.message_friends();
            cin >> receiver;

            // Create an instance of the messaging_system class
            messaging_system communication;
            // Display previous messages
            communication.Read_messages(u1.getUsername(), receiver);

            // Start a thread to continuously check for new messages
            thread message_reader([&] {
                while (true) {
                    // Check for and display new messages
                    communication.read_newMessage(u1.getUsername(), receiver);
                    // Sleep for a short duration to avoid busy waiting
                    this_thread::sleep_for(chrono::milliseconds(500));
                }
                });

            // Enter a loop to prompt the user to send a message or exit
            while (true) {
                // Prompt the user to send a message or exit
               // cout << "Enter your message (or -1 to exit): ";
                string message;
                getline(cin, message); // Read the entire line of input

                // If the user wants to exit, break out of the loop
                if (message == "-1") {
                    break;
                }

                // Send the message
                communication.sending_messages(u1.getUsername(), receiver, message);
            }

            // Join the message_reader thread to avoid premature termination
            message_reader.join();

            cout << "\n";
            break;
        }


        case 3:
        {
            cout << "\033[0;36mExiting...\n\033[0m";
            end_program = true;
            u1.Exit();
            return 0;
        }
        default:
            cout << "Invalid choice. Please try again.\n";
            cout << "\n";
            break;
        }
    }

    return 0;
}
