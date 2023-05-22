#include <iostream>
#include <vector>
#include <string>
#include "sqlite3.h"
#include <functional>

using namespace std;

static int createDB(const char *s);
static int createTable(const char *s, const string &sql);
static int displayEntries(const char *s);
static pair<string, int> createUser(const char *s);
static pair<string, int> login(const char *s);

struct JournalEntry
{
    string title;
    string content;
    string tags;
    string date;
};

struct JournalUser
{
    string username;
    string password;
};

static int callback(void *data, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

static int callbackCount(void *data, int argc, char **argv, char **azColName)
{
    int *count = static_cast<int *>(data);
    if (argc > 0 && argv[0])
        *count = atoi(argv[0]);
    return 0;
}

void createEntry(const char *s, int loggedUserID)
{
    JournalEntry entry;

    cout << "Title: ";
    getline(cin >> ws, entry.title);

    cout << "Content: ";
    getline(cin >> ws, entry.content);

    cout << "Tags: ";
    getline(cin >> ws, entry.tags);

    string sql = "INSERT INTO ENTRIES(UserID, TITLE, CONTENT, TAGS) VALUES(" + to_string(loggedUserID) + ",'" + entry.title + "','" + entry.content + "','" + entry.tags + "');";

    sqlite3 *DB;
    int rc = sqlite3_open(s, &DB);
    if (rc != SQLITE_OK)
    {
        cerr << "Error opening database: " << sqlite3_errmsg(DB) << endl;
        sqlite3_close(DB);
        return;
    }

    rc = sqlite3_exec(DB, sql.c_str(), callback, 0, nullptr);
    if (rc != SQLITE_OK)
    {
        cerr << "Error creating Entry: " << sqlite3_errmsg(DB) << endl;
    }
    else
    {
        cout << "Entry created successfully" << endl;
    }

    sqlite3_close(DB);
}

int main()
{
    const char *dir = "C:\\Users\\ellia\\Desktop\\coding\\Journal\\journal_database.db";
    JournalUser user;
    createDB(dir);
    createTable(dir, "CREATE TABLE IF NOT EXISTS ENTRIES("
                     "ID INTEGER NOT NULL PRIMARY KEY,"
                     "USERID INTEGER NOT NULL,"
                     "TITLE TEXT NOT NULL,"
                     "CONTENT TEXT NOT NULL,"
                     "TAGS TEXT NOT NULL);");
    createTable(dir, "CREATE TABLE IF NOT EXISTS USER("
                     "ID INTEGER NOT NULL PRIMARY KEY,"
                     "USERNAME NOT NULL,"
                     "PASSWORD NOT NULL);");

    // Get user login or make new account.
    int choice = 0;
    string username;
    bool loginKey = false;

    string loggedUsername;
    int loggedUserID = -1;

    do
    {
        cout << "----- Journal Application -----" << endl;
        cout << "1. Existing User" << endl;
        cout << "2. New User" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cout << "\n----------------------------------\n\n";

        switch (choice)
        {
        case 1:
        {
            // user logs in
            pair<string, int> loginResult = login(dir);
            if (!loginResult.first.empty() && loginResult.second != -1)
            {
                loggedUsername = loginResult.first;
                loggedUserID = loginResult.second;
                loginKey = true;
            }
            break;
        }
        case 2:
        {
            // user creates account
            cout << "Directing to account creation..." << endl
                 << endl;
            pair<string, int> userInfo = createUser(dir);
            if (userInfo.second != -1)
            {
                loggedUsername = userInfo.first;
                loggedUserID = userInfo.second;
                loginKey = true;
            }
            break;
        }
        case 3:
            cout << "Exiting the application. Goodbye!" << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }

        cout << endl;
    } while (loginKey == false);

    choice = 0;

    while (choice != 3)
    {
        cout << "----------------------------------\n\n";
        cout << "Logged ID: " << loggedUserID << ", User: " << loggedUsername << endl;
        cout << "1. Create Entry\n";
        cout << "2. Display Entries\n";
        cout << "3. Logout/Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;
        cout << endl;

        switch (choice)
        {
        case 1:
            createEntry(dir, loggedUserID);
            break;
        case 2:
            displayEntries(dir);
            break;
        case 3:
            cout << "Exiting the application. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }

        cout << endl;
    }

    int x;
    cin >> x;

    return 0;
}

static int createDB(const char *s)
{
    sqlite3 *DB;
    int rc = 0;

    rc = sqlite3_open(s, &DB);

    sqlite3_close(DB);

    return 0;
}

static int createTable(const char *s, const string &sql)
{
    sqlite3 *DB;

    try
    {
        int rc = 0;
        rc = sqlite3_open(s, &DB);

        // execute SQL statement
        rc = sqlite3_exec(DB, sql.c_str(), callback, 0, nullptr);

        // Error check
        if (rc != SQLITE_OK)
        {
            cerr << "Error creating Table" << endl;
        }
        else
        {
            sqlite3_close(DB);
        }
    }
    catch (const exception &e)
    {
        cerr << e.what();
    }
    return 0;
}

static int displayEntries(const char *s)
{
    sqlite3 *DB;
    int rc = 0;
    rc = sqlite3_open(s, &DB);
    string sql = "SELECT * FROM ENTRIES";
    const char *data = "Callback function called";

    /* Execute SQL statement */
    rc = sqlite3_exec(DB, sql.c_str(), callback, (void *)data, nullptr);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(DB));
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }

    sqlite3_close(DB);
    return 0;
}

static pair<string, int> createUser(const char *s)
{
    JournalUser user;
    sqlite3 *DB;
    int rc = 0;
    rc = sqlite3_open(s, &DB);

    cout << "Username: ";
    getline(cin >> ws, user.username);

    cout << "Password: ";
    getline(cin >> ws, user.password);

    string checkQuery = "SELECT COUNT(*) FROM USER WHERE USERNAME = '" + user.username + "';";
    string insertQuery = "INSERT INTO USER(USERNAME, PASSWORD) VALUES('" + user.username + "','" + user.password + "');";

    char *zErrMsg;
    int count = 0;

    rc = sqlite3_exec(DB, checkQuery.c_str(), callbackCount, &count, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "Error creating User" << endl;
        sqlite3_free(zErrMsg);
    }
    else if (count > 0)
    {
        cerr << "Username already exists" << endl;
    }
    else
    {
        rc = sqlite3_exec(DB, insertQuery.c_str(), nullptr, nullptr, &zErrMsg);

        if (rc != SQLITE_OK)
        {
            cerr << "Error creating User" << endl;
            sqlite3_free(zErrMsg);
        }
        else
        {
            cout << "User created successfully" << endl;
            int userID = sqlite3_last_insert_rowid(DB);
            sqlite3_close(DB);
            return make_pair(user.username, userID);
        }
    }

    sqlite3_close(DB);

    return make_pair("", -1);
}

static pair<string, int> login(const char *s)
{
    string username;
    sqlite3 *DB;
    int rc = 0;
    rc = sqlite3_open(s, &DB);

    cout << "Username: ";
    getline(cin >> ws, username);

    string checkQuery = "SELECT COUNT(*) FROM USER WHERE USERNAME = '" + username + "';";
    string selectQuery = "SELECT ID FROM USER WHERE USERNAME = '" + username + "';";
    char *zErrMsg;
    int count = 0;
    int userID = -1;

    rc = sqlite3_exec(
        DB, checkQuery.c_str(), [](void *data, int argc, char **argv, char **azColName)
        {
        int *count = static_cast<int *>(data);
        if (argc > 0 && argv[0])
            *count = atoi(argv[0]);
        return 0; },
        &count, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "Error accessing User table" << endl;
        sqlite3_free(zErrMsg);
    }
    else if (count == 0)
    {
        cerr << "Username does not exist. Would you like to create a new account? (Y/N): ";
        string response;
        getline(cin >> ws, response);
        if (response == "Y" || response == "y")
        {
            return createUser(s);
        }
    }
    else
    {
        string password;
        cout << "Password: ";
        getline(cin >> ws, password);

        string passwordQuery = "SELECT COUNT(*) FROM USER WHERE USERNAME = '" + username + "' AND PASSWORD = '" + password + "';";

        rc = sqlite3_exec(
            DB, passwordQuery.c_str(), [](void *data, int argc, char **argv, char **azColName)
            {
            int *count = static_cast<int *>(data);
            if (argc > 0 && argv[0])
                *count = atoi(argv[0]);
            return 0; },
            &count, &zErrMsg);

        if (rc != SQLITE_OK)
        {
            cerr << "Error accessing User table" << endl;
            sqlite3_free(zErrMsg);
        }
        else if (count == 0)
        {
            cerr << "Incorrect password" << endl;
        }
        else
        {
            rc = sqlite3_exec(
                DB, selectQuery.c_str(), [](void *data, int argc, char **argv, char **azColName)
                {
                int *userID = static_cast<int *>(data);
                if (argc > 0 && argv[0])
                    *userID = atoi(argv[0]);
                return 0; },
                &userID, &zErrMsg);

            if (rc != SQLITE_OK)
            {
                cerr << "Error accessing User table" << endl;
                sqlite3_free(zErrMsg);
            }
            else
            {
                cout << "Login successful" << endl;
                sqlite3_close(DB);
                return make_pair(username, userID);
            }
        }
    }

    sqlite3_close(DB);
    return make_pair("", -1);
}