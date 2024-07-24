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

// Example usage
int main() {
    char* template = read_file("template.html");
    if (template) {
        char* rendered = render_template(template, "{{name}}", "John Doe");
        printf("%s\n", rendered);

        free(template);
        free(rendered);
    }
    return 0;
}
