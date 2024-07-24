/*
 * Copyright (c) 2011 Radhamadhab Dalai
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

// Function to read a file into a string
char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(length + 1);
    fread(content, 1, length, file);
    content[length] = '\0';

    fclose(file);
    return content;
}

// Function to render HTML template with placeholders
char* render_template(const char* template, const char* placeholder, const char* value) {
char* rendered = (char*)malloc(strlen(template) + strlen(value) - strlen(placeholder) + 1);
char* pos = strstr(template, placeholder);
if (pos) {
strncpy(rendered, template, pos - template);
rendered[pos - template] = '\0';
strcat(rendered, value);
strcat(rendered, pos + strlen(placeholder));
} else {
strcpy(rendered, template);
}
return rendered;
}

// Database initialization
sqlite3* init_db(const char* db_name) {
    sqlite3* db;
    if (sqlite3_open(db_name, &db)) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    return db;
}

// Execute a query
void execute_query(sqlite3* db, const char* query) {
    char* err_msg = NULL;
    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Query executed successfully\n");
    }
}

// Close the database
void close_db(sqlite3* db) {
    sqlite3_close(db);
}

// Function to get user name from the database
char* get_user_name(sqlite3* db, int user_id) {
    sqlite3_stmt* stmt;
    const char* query = "SELECT name FROM users WHERE id = ?";

    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement\n");
        return NULL;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    char* name = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        name = strdup((const char*)text);
    }

    sqlite3_finalize(stmt);
    return name;
}

// Example usage
int main() {
    // Initialize the database
    sqlite3* db = init_db("test.db");
    if (!db) {
        return 1;
    }

    // Create the table and insert a user
    const char* create_table_query = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT);";
    execute_query(db, create_table_query);

    const char* insert_query = "INSERT INTO users (name) VALUES ('John Doe');";
    execute_query(db, insert_query);

    // Get user name from database
    char* user_name = get_user_name(db, 1);
    if (!user_name) {
        fprintf(stderr, "User not found\n");
        close_db(db);
        return 1;
    }

    // Read and render the HTML template
    char* template = read_file("template.html");
    if (template) {
        char* rendered = render_template(template, "{{name}}", user_name);
        printf("%s\n", rendered);

        free(template);
        free(rendered);
    }

    // Clean up
    free(user_name);
    close_db(db);

    return 0;
}
