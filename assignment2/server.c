
// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 80 
int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server";
	//printf("ARGC = %ls", &argc);
    if(argc > 1){
	printf("We are in the exec-ed process. PID = %d, UID = %d\n", getpid(),getuid());
	server_fd = atoi(argv[1]);  //reading the argument passed in exec
	if (listen(server_fd, 3) < 0) { 
        		perror("listen"); 
        		exit(EXIT_FAILURE); 
		} 
    		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0){ 
        		perror("accept"); 
        		exit(EXIT_FAILURE); 
    		} 
    		valread = read( new_socket , buffer, 1024); 
    		printf("%s\n",buffer ); 
    		send(new_socket , hello , strlen(hello) , 0 ); 
    		printf("Hello message sent back from PID = %d\n", getpid()); 
    		exit(0);
    } 
	printf("We are in the parent process. PID = %d, UID = %d\n", getpid(), getuid());
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    }
    	address.sin_family = AF_INET; 
    	address.sin_addr.s_addr = INADDR_ANY; 
    	address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    }  
    
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

	//Start of changes by Vanditt
	if(fork() == 0){
		printf("Fork Successful! Child UID = %d\n", getuid());
		if(setuid(65534) == 0){
			printf("Child process user ID set to: %d \n", getuid());
			char server_param[11];
			sprintf(server_param, "%d", server_fd);
			execv("./server.o", (char*[]){"./server.", server_param, NULL});
			printf("Exec failed!!");   //This code should never run
		}else{
			printf("Set UID Failed");	
		}
    	}

	return 0;
	
 
} 
