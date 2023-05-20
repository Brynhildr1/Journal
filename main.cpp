#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include "sqlite3.h"

using namespace std;

static int createDB(const char *s);
static int createTable(const char *s);

int main()
{
    cout << "hello world" << endl;

    const char *dir = "C:\\Users\\ellia\\Desktop\\coding\\Journal\\journal_database.db";

    sqlite3 *DB;

    createDB(dir);
    createTable(dir);

    int x;
    cin >> x;

    return 0;
}

static int createDB(const char *s)
{
    sqlite3 *DB;
    int exit = 0;

    exit = sqlite3_open(s, &DB);

    sqlite3_close(DB);

    return 0;
}
static int createTable(const char *s)
{
    sqlite3 *DB;

    string sql = "CREATE TABLE IF NOT EXIST ENTRIES(TITLE TEXT NOT NULL PRIMARY KEY, CONTENT TEXT NOT NULL, TAGS TEXT NOT NULL);";

    try
    {
        int exit = 0;
        exit = sqlite3_open(s, &DB);

        char *messageError;
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

        if (exit != SQLITE_OK)
        {
            cerr << "Error creating Table" << endl;
            sqlite3_free(messageError);
        }
        else
            cout << "Table created successfully" << endl;
        sqlite3_close(DB);
    }
    catch (const exception &e)
    {
        cerr << e.what();
    }
    return 0;
}

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