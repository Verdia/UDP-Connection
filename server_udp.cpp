// server_udp.cpp
 
#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<chrono>
#include<iostream>
 
#define PORT 9333  
#define SERVER_IP "127.0.0.1"  
#define BUFFER_SIZE 1024
 
FILE *input;
FILE *checkfile;
int n;
int count = 0;
 
struct sockaddr_in remote;  
int s;
using namespace std;
 
int main(){
 
unsigned char buffer[BUFFER_SIZE];
char check[100];
 
printf("Type file and ENTER to send!\n");
fgets(check, 100, stdin);
 

input = fopen("a.mp4", "r");
checkfile = fopen("", "w");

chrono::steady_clock sc;
auto start = sc.now();
 
if(input){
 
    if((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        fprintf(stderr, "Error getting a socket! \n");
        exit(-1);
    }
 
    memset((char *) &remote, 0, sizeof(remote));
 
    remote.sin_family = AF_INET;
    remote.sin_port = htons(PORT);
 
 
    if(inet_aton(SERVER_IP, &remote.sin_addr) == 0){
        fprintf(stderr, "maybe no valid adress\n");
        exit(1);
    }
 
    while(!feof(input)){
        n = fread(buffer, 1, BUFFER_SIZE, input);
        count += n;
        printf("n = %d\n", n);
 
        if(sendto(s, buffer, n, 0, (struct sockaddr*)&remote, sizeof(remote)) == -1){
            fprintf(stderr, "error while sending data!\n");
            exit(-1);
        }
        fwrite(buffer, 1, n, checkfile);
    }
    printf("%d bytes sent. \n", count);
 
}else{
    printf("error while opening input file!");
}
printf("File sent!!\n");

auto end = sc.now();
auto time_span = static_cast<chrono::duration<double>>(end-start);
cout << "Operation took : "<<time_span.count()<<"sec\n";
 
close(s);
fclose(input);
 
return 0;
}