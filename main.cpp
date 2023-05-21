#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include "sqlite3.h"

using namespace std;

static int createDB(const char *s);
static int createTable(const char *s, string sql);
static int displayEntries(const char *s);

// Create a callback function
static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char *)data);

    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

// stores all the information for the journal entry
struct JournalEntry
{
    string title;
    string content;
    string tags;
    string date;
};

vector<JournalEntry> journalEntries;

// gathering the data from the user for the entry
void createEntry(const char *s)
{

    JournalEntry entry;
    sqlite3 *DB;

    cout << "Title: ";
    getline(cin >> ws, entry.title);

    cout << "Content: ";
    getline(cin >> ws, entry.content);

    cout << "Tags: ";
    getline(cin >> ws, entry.tags);

    /*     cout << "Date: ";
        getline(cin >> ws, entry.date); */

    string sql = "INSERT INTO ENTRIES(TITLE, CONTENT, TAGS) VALUES('" + entry.title + "','" + entry.content + "','" + entry.tags + "');";

    try
    {
        int exit = 0;
        exit = sqlite3_open(s, &DB);

        char *messageError;
        exit = sqlite3_exec(DB, sql.c_str(), callback, 0, &messageError);

        if (exit != SQLITE_OK)
        {
            cerr << "Error creating Entry" << endl;
            sqlite3_free(messageError);
        }
        else
            cout << "Entry created successfully" << endl;
        sqlite3_close(DB);
    }
    catch (const exception &e)
    {
        cerr << e.what();
    }
}

// displays the stored entries, this will need to be learnt and probably changed alot
/* void displayEntries()
{
    cout << "------ Journal Entries ------" << endl;

    for (const auto &entry : journalEntries)
    {
        cout << "Title: " << entry.title << endl;
        cout << "Content: " << entry.content << endl;
        cout << "Tags: " << entry.tags << endl;
        cout << "Date: " << entry.date << endl;
        cout << "----------------------------" << endl;
    }

    if (journalEntries.empty())
    {
        cout << "No entries found." << endl;
    }
} */

int main()
{
    const char *dir = "C:\\Users\\ellia\\Desktop\\coding\\Journal\\journal_database.db";

    sqlite3 *DB;

    // First time set up for database and tables if they dont already exist
    createDB(dir);

    // Create SQL statement, One time only if doesnt exist
    createTable(dir, "CREATE TABLE IF NOT EXISTS ENTRIES("
                     "ID INTEGER NOT NULL PRIMARY KEY,"
                     "TITLE TEXT NOT NULL,"
                     "CONTENT TEXT NOT NULL,"
                     "TAGS TEXT NOT NULL);");
    createTable(dir, "CREATE TABLE IF NOT EXISTS USER("
                     "ID INTEGER NOT NULL PRIMARY KEY,"
                     "EMAIL NOT NULL,"
                     "PASSWORD NOT NULL);");

    // user choice to add ar view or exit.
    int choice;

    do
    {
        cout << "----- Journal Application -----" << endl;
        cout << "1. Create Entry" << endl;
        cout << "2. Display Entries" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cout << endl;

        switch (choice)
        {
        case 1:
            createEntry(dir);
            break;
        case 2:
            displayEntries(dir);
            break;
        case 3:
            cout << "Exiting the application. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }

        cout << endl;
    } while (choice != 3);

    int x;
    cin >> x;

    return 0;
}

// function used to create the database if one doesnt exist at the dir mentioned in main
static int createDB(const char *s)
{
    sqlite3 *DB;
    int exit = 0;

    exit = sqlite3_open(s, &DB);

    sqlite3_close(DB);

    return 0;
}

// function used to create the tables with in the database assuming that they do not already exist.
static int createTable(const char *s, string sql)
{
    sqlite3 *DB;

    try
    {
        int exit = 0;
        exit = sqlite3_open(s, &DB);
        char *messageError;

        // execute SQL statement
        exit = sqlite3_exec(DB, sql.c_str(), callback, 0, &messageError);

        // Error check
        if (exit != SQLITE_OK)
        {
            cerr << "Error creating Table" << endl;
            sqlite3_free(messageError);
        }
        else
            sqlite3_close(DB);
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
    int exit = 0;
    exit = sqlite3_open(s, &DB);
    string sql = "SELECT * FROM ENTRIES";
    char *messageError;
    const char *data = "Callback function called";

    /* Execute SQL statement */
    exit = sqlite3_exec(DB, sql.c_str(), callback, (void *)data, &messageError);

    if (exit != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", messageError);
        sqlite3_free(messageError);
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }

    sqlite3_close(DB);
    return 0;
}

/* // Create a callback function
int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    // Return successful
    return 0;
}

int main()
{

    // Pointer to SQLite connection
    sqlite3 *db;

    // Save any error messages
    char *zErrMsg = 0;

    // Save the result of opening the file
    int rc;

    // Save any SQL
    string sql;

    // Save the result of opening the file
    rc = sqlite3_open("journal_database.db", &db);

    if (rc)
    {
        // Show an error message
        cout << "DB Error: " << sqlite3_errmsg(db) << endl;
        // Close the connection
        sqlite3_close(db);
        // Return an error
        return (1);
    }

    // Save SQL to create a table
    sql = "CREATE TABLE USER ("
          "ID INT PRIMARY KEY     NOT NULL,"
          "NAME           TEXT    NOT NULL,"
          "PASSWORD     iNTEGER   NOT NULL);";

    // Run the SQL (convert the string to a C-String with c_str() )
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    // Close the SQL connection
    sqlite3_close(db);

    return 0;
} */

/* struct JournalEntry
{
    string title;
    string content;
    string tags;
    string date;
};

vector<JournalEntry> journalEntries;

void createEntry()
{
    JournalEntry entry;

    cout << "Title: ";
    getline(cin >> ws, entry.title);

    cout << "Content: ";
    getline(cin >> ws, entry.content);

    cout << "Tags: ";
    getline(cin >> ws, entry.tags);

    cout << "Date: ";
    getline(cin >> ws, entry.date);

    journalEntries.push_back(entry);

    cout << "Entry created successfully!" << endl;
}

void displayEntries()
{
    cout << "------ Journal Entries ------" << endl;

    for (const auto &entry : journalEntries)
    {
        cout << "Title: " << entry.title << endl;
        cout << "Content: " << entry.content << endl;
        cout << "Tags: " << entry.tags << endl;
        cout << "Date: " << entry.date << endl;
        cout << "----------------------------" << endl;
    }

    if (journalEntries.empty())
    {
        cout << "No entries found." << endl;
    }
}

int main()
{
    int choice;

    do
    {
        cout << "----- Journal Application -----" << endl;
        cout << "1. Create Entry" << endl;
        cout << "2. Display Entries" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            createEntry();
            break;
        case 2:
            displayEntries();
            break;
        case 3:
            cout << "Exiting the application. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }

        cout << endl;
    } while (choice != 3);

    return 0;
}
 */