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

// Define a struct for a basic service
typedef struct {
    char* name;
} Service;

// Function to create a new service
Service* create_service(const char* name) {
    Service* service = (Service*)malloc(sizeof(Service));
    service->name = strdup(name);
    return service;
}

// Define a struct for a DI container
typedef struct {
    Service* service;
} DIContainer;

// Function to initialize the DI container
DIContainer* init_container() {
    DIContainer* container = (DIContainer*)malloc(sizeof(DIContainer));
    container->service = create_service("My Service");
    return container;
}

// Function to get a service from the container
Service* get_service(DIContainer* container) {
    return container->service;
}

// Clean up the container
void destroy_container(DIContainer* container) {
    free(container->service->name);
    free(container->service);
    free(container);
}

int main() {
    // Initialize the container
    DIContainer* container = init_container();

    // Get the service from the container
    Service* service = get_service(container);
    printf("Service Name: %s\n", service->name);

    // Clean up
    destroy_container(container);
    return 0;
}
