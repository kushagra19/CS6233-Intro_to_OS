#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<time.h>
#include<string.h>
#include<errno.h>

#define PORT 10019

void generateElements(int clientSockfd,int n , int d)
{
    for(int k = 0 ; k < n ; ++k)
    {
        int kthElement = (k) * (d);
        //random time(0-1s) between generating elements
        sleep((unsigned int)rand() % 2);
        //write element into the socket
        write(clientSockfd, &kthElement, sizeof(kthElement));
    }
}

void serverChild()
{   
    // Create a TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set SO_REUSEADDR option to use PORT again without WAIT_TIME
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //server
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind the socket to PORT
    while(bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) 
    {   
        //check if error is anything other than port still in use
        if (errno != EADDRINUSE) 
        {
            perror("bind");
            exit(EXIT_FAILURE);
        }
    }
    //random wait time between (1-5s) before listen and accept
    sleep(1 + rand() % 5);

    //start listening from socket with sockfd
    if(listen(sockfd, 1) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //client address
    struct sockaddr_in client;
    socklen_t clientLen = sizeof(client);

    //accept connection from client
    int clientSocket = accept(sockfd, (struct sockaddr *)&client, &clientLen);
    //error handling
    if(clientSocket == -1)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    //to get n and d    
    int data[2];

    //get n and d from the parent(client) in this case
    read(clientSocket, data, sizeof(data));

    //store n and d
    int n = data[0], d = data[1];

    //write elements into PORT using socket
    generateElements(clientSocket, n, d);
    //close socket   
    close(clientSocket);
    //close socket
    close(sockfd);
}

void printElements(int serverSockfd ,int n, int d)
{
    printf("Parent Printing...\n");
    int kthElement, elementsPrinted = 0;
    for(int i = 0; i < n; i++) 
    {
        //read elements as and when generated
        read(serverSockfd, &kthElement, sizeof(kthElement));
        //print sequence elements
        if(elementsPrinted == 0)
            printf("%dst element of the sequence is %d.\n", elementsPrinted+1, kthElement);
        else if(elementsPrinted == 1)
            printf("%dnd element of the sequence is %d.\n", elementsPrinted+1, kthElement);
        else if(elementsPrinted == 2)
            printf("%drd element of the sequence is %d.\n", elementsPrinted+1, kthElement);
        else
            printf("%dth element of the sequence is %d.\n", elementsPrinted+1, kthElement);
        fflush(stdout);
        elementsPrinted += 1;
    }
}

void clientParent(int n, int d)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    // Set SO_REUSEADDR optionto use PORT again without WAIT_TIME
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //server
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    
    // Parent process (client) attempting to connect
    while (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        // Wait 100 ms between attempts
        usleep(100000);
    }

    //to send n and d
    int data[2] = {n,d};
    //send n and d to child (server)
    write(sockfd, data, sizeof(data));

    //print elements
    printElements(sockfd, n, d);
    //close socket  
    close(sockfd);
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "Usage: %s <n> <d>\n", argv[0]); //check number of params passed
        return 1;
    }

    int n = atoi(argv[1]); //number of element in the AP
    int d = atoi(argv[2]); //common difference of the AP

    if(n < 1)
    {
        printf("Improper value of n.");
        return 1;
    }

    //seed at the start of run
    srand((unsigned int)time(NULL));

    // Create child process
    pid_t childPid = fork();

    if (childPid == -1) 
    {
        perror("fork");
        return 1;
    } 
    else if (childPid == 0) 
    {   
        // Child process (server)
        serverChild();
    } 
    else 
    {   
        //Parent process (client)
        clientParent(n,d);   
        // Wait for the child to exit
        wait(NULL);
    }
    
    return 0;
}