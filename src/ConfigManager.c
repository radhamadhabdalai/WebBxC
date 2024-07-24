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

#define CONFIG_FILE "config.cfg"

typedef struct {
    char* key;
    char* value;
} Config;

Config* load_config(const char* filename, int* config_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Could not open config file");
        return NULL;
    }

    Config* configs = NULL;
    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        char* key = strtok(line, "=");
        char* value = strtok(NULL, "\n");
        if (key && value) {
            configs = (Config*)realloc(configs, sizeof(Config) * (count + 1));
            configs[count].key = strdup(key);
            configs[count].value = strdup(value);
            count++;
        }
    }
    fclose(file);
    *config_count = count;
    return configs;
}

char* get_config_value(Config* configs, int config_count, const char* key) {
    for (int i = 0; i < config_count; i++) {
        if (strcmp(configs[i].key, key) == 0) {
            return configs[i].value;
        }
    }
    return NULL;
}

void free_config(Config* configs, int config_count) {
    for (int i = 0; i < config_count; i++) {
        free(configs[i].key);
        free(configs[i].value);
    }
    free(configs);
}

int main() {
    int config_count;
    Config* configs = load_config(CONFIG_FILE, &config_count);
    if (configs) {
        char* service_name = get_config_value(configs, config_count, "service_name");
        if (service_name) {
            printf("Service Name from config: %s\n", service_name);
        }
        free_config(configs, config_count);
    }
    return 0;
}
