#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <algorithm>
#include <queue>
#include <regex>
/* You will to add includes here */

int clientid;

int gsready(std::string &ip, int port,int* ipstatus);

int file_exists(const std::string& filename);

    struct sockaddr_in server_addr;
    struct timeval timeout;

void *callback(void* temp);

std::vector<std::string> split(std::string sString, std::string delimiter);

int main(int argc, char *argv[]){

clientid =0;
struct sockaddr_in clientaddress;
socklen_t addrlen = sizeof(clientaddress);
  
  /* Do more magic */
 std::string delimiter = ":";

 std::vector<std::string> outputString = split(argv[1],":"); 

 std::string ipString = "";

 int port;


 if(outputString.size() > 2){
  port = atoi(outputString[outputString.size()-1].c_str());
  for(int i=0; i < 8 ; i++){
   ipString = ipString + outputString[i];}}
else{
   port = atoi(outputString[1].c_str());
   ipString = outputString[0];}

 int *ipstatus = new int;

 int mastersocketfd = gsready(ipString ,port, ipstatus);
  
  delete ipstatus;

std::vector<char> recvbuffvec(1024);

while (1)
{
  int *csocketfd = new int;
  *csocketfd = accept(mastersocketfd,(struct sockaddr*)&clientaddress, &addrlen);
  clientid++;
  std::cout << "New client is connected with client id " << clientid << std::endl;
  if(*csocketfd < 0){perror("error with accept function"); exit(1);}
   timeout.tv_sec = 1;
   timeout.tv_usec = 0;
    if (setsockopt(*csocketfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
   pthread_t calhandel;
   if(pthread_create(&calhandel,NULL,callback,(void*)csocketfd)!= 0) {
            perror("pthread_create");
            close(*csocketfd);
            continue;
        }

 pthread_detach(calhandel);
} //end master socker while loop



printf("done.\n");
return(0);
}


void *callback(void* temp){
      int *csocketfd = (int*)temp;
      std::vector<char> recvbuffvec(1024);
        std::cout << "BLOCKED AT RECV" << std::endl;
      int sentrecvbytes = recv(*csocketfd, &recvbuffvec[0], recvbuffvec.size(), 0);
      if(sentrecvbytes == 0){close(*csocketfd);clientid--;}
      else if(sentrecvbytes < 0){perror("error with recv");clientid--;}
      char* recvbuff = new char[recvbuffvec.size()];
      std::memcpy(recvbuff, recvbuffvec.data(), recvbuffvec.size());
      std::vector<std::string> sVectorString = split(std::string(recvbuff), "\n");
      delete recvbuff;
      std::vector<std::string> sVectorStringfirstline = split(sVectorString[0], " ");
      if(sVectorStringfirstline[0] == "GET"){

      if(sVectorStringfirstline[1] == "/"){
      std::ifstream htmlFile("html/get_index.html");
     if (!htmlFile.is_open()) {std::cerr << "Failed to open index.html\n";exit(1);}
     std::string htmlContent((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
     std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(htmlContent.size()) + "\r\n\r\n" + htmlContent;
    // Send HTML content to the client
    int bytesSent = send(*csocketfd, httpResponse.c_str(), httpResponse.size(), 0);
    if (bytesSent < 0) {std::cerr << "Error sending HTML content\n";close(*csocketfd);clientid--;} 
    else if (bytesSent == 0){close(*csocketfd);}

      }
      else if (file_exists(sVectorStringfirstline[1].substr(1))){
    std::ifstream htmlFile(sVectorStringfirstline[1].substr(1));
    if (!htmlFile.is_open()) {std::cerr << "Failed to open afile.html\n";exit(1);}
    std::string htmlContent((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
    std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(htmlContent.size()) + "\r\n\r\n" + htmlContent;
    // Send HTML content to the client
    int bytesSent = send(*csocketfd, httpResponse.c_str(), httpResponse.size(), 0);
    if (bytesSent < 0) {std::cerr << "Error sending HTML content\n";close(*csocketfd);clientid--;} 
    else if (bytesSent == 0){close(*csocketfd);clientid--;}
    
      }
      else{


    std::ifstream htmlFile("html/error.html");
    if (!htmlFile.is_open()) {std::cerr << "Failed to open error.html\n";exit(1);}
    std::string htmlContent((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
    std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(htmlContent.size()) + "\r\n\r\n" + htmlContent;
    // Send HTML content to the client
    int bytesSent = send(*csocketfd, httpResponse.c_str(), httpResponse.size(), 0);
    if (bytesSent < 0) {std::cerr << "Error sending HTML content\n";close(*csocketfd);clientid--;}
     else if (bytesSent == 0){close(*csocketfd);} 



      }
      


      }
      else if(sVectorStringfirstline[0] == "HEAD")
      {
    if(sVectorStringfirstline[1] == "/"){
      std::ifstream htmlFile("html/get_index.html");
     if (!htmlFile.is_open()) {std::cerr << "Failed to open index.html\n";exit(1);}
     std::string htmlContent((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
     std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(htmlContent.size());
    // Send HTML content to the client
    int bytesSent = send(*csocketfd, httpResponse.c_str(), httpResponse.size(), 0);
    if (bytesSent < 0) {std::cerr << "Error sending HTML content\n";close(*csocketfd);clientid--;} 
    else if (bytesSent == 0){close(*csocketfd);clientid--;}

      }

  else if (file_exists(sVectorStringfirstline[1].substr(1))){
    std::ifstream htmlFile(sVectorStringfirstline[1].substr(1));
     if (!htmlFile.is_open()) {std::cerr << "Failed to open index.html\n";exit(1);}
    std::string htmlContent((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
    std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(htmlContent.size());
    // Send HTML content to the client
    int bytesSent = send(*csocketfd, httpResponse.c_str(), httpResponse.size(), 0);
    if (bytesSent < 0) {std::cerr << "Error sending HTML content\n";close(*csocketfd);clientid--;exit(1);} 
      else if (bytesSent == 0){close(*csocketfd);clientid--;} 

      }
      else{

    std::ifstream htmlFile("html/error.html");
    if (!htmlFile.is_open()) {std::cerr << "Failed to open error.html\n";exit(1);}
    std::string htmlContent((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
    std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(htmlContent.size());
    // Send HTML content to the client
    int bytesSent = send(*csocketfd, httpResponse.c_str(), httpResponse.size(), 0);
    if (bytesSent < 0) {std::cerr << "Error sending HTML content\n";close(*csocketfd);clientid--;exit(1);} 
      else if (bytesSent == 0){close(*csocketfd);clientid--;} 



      }
      
      
      }

close(*csocketfd);
delete csocketfd;
pthread_exit(0);
}


int gsready(std::string &ip, int port,int* ipstatus){
int socketfd; 
std::string portStr = std::to_string(port);
struct addrinfo hint, *output, *temp;
int check = 0;
memset(&hint, 0, sizeof(hint));
hint.ai_family = AF_UNSPEC;
hint.ai_socktype = SOCK_STREAM;
int status = getaddrinfo(ip.c_str(),portStr.c_str(), &hint, &output);

if(status != 0){
perror("There is problem in getting getaddrinfo");
exit(1);
}


for(temp=output; temp != NULL;temp->ai_addr){
socketfd = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);

if(bind(socketfd,temp->ai_addr,temp->ai_addrlen) < 0){if(check == 2){perror("bind error");}else{continue;}}
else{
  if(socketfd > 0){
  std::cout << "[x]Listening on " << ip << ":" << port << std::endl;
  break;
  }
}

check++;

 
  }
   

if(listen(socketfd, 5 < 0)){perror("error with listen function");exit(1);}
  
freeaddrinfo(output);
return socketfd;
}

std::vector<std::string> split(std::string sString, std::string delimiter) {
    std::vector<std::string> nString;
    std::string temp;
    int count = 0; 

    for (int i = 0; i < static_cast<int>(sString.length()); i++) {
        if (sString[i] == delimiter[0]) {
            count++;
            nString.push_back(temp);
            temp = "";
        } else {
            temp = temp + sString[i];
        }
    }

   
    if (count == 0 || (count > 0 && temp != "")) {
        nString.push_back(temp);
    }

    return nString;
}

int file_exists(const std::string& filename) {
    // Check if file exists and we can access it
    if (access(filename.c_str(), F_OK) != -1) {
        // File exists
        return 1;
    } else {
        // File doesn't exist or we don't have access
        return 0;
    }
}