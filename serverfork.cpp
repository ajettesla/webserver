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

int gsready(std::string &ip, int port,int* ipstatus);

std::vector<std::string> split(std::string sString, std::string delimiter);

int main(int argc, char *argv[]){

int clientid =0;
struct sockaddr_in clientaddress;
socklen_t addrlen = sizeof(clientaddress);
  
  /* Do more magic */
 std::string delimiter = ":";

 std::vector<std::string> outputString = split(argv[1],":"); 

 std::string ipString = "";

 int port;
 clientid = 0;

 if(outputString.size() > 2){
  port = atoi(outputString[outputString.size()-1].c_str());
  for(int i=0; i < 8 ; i++){
   ipString = ipString + outputString[i];}}
else{
   port = atoi(outputString[1].c_str());
   ipString = outputString[0];}

 int *ipstatus = new int;

 int mastersocketfd = gsready(ipString ,port, ipstatus);

std::vector<char> recvbuffvec(1024);

  
while (1)
{
  std::cout << "New client is connected with client id " << clientid << std::endl;
  int csocketfd = accept(mastersocketfd,(struct sockaddr*)&clientaddress, &addrlen);
  clientid++;
  if(csocketfd < 0){perror("error with accept function"); exit(1);}
   
  int pid = fork();
  if(pid==0) {
    close(mastersocketfd);
    while(1){
      int sentrecvbytes = recv(csocketfd, &recvbuffvec[0], recvbuffvec.size(), 0);
      char* recvbuff = new char[recvbuffvec.size()];
      std::memcpy(recvbuff, recvbuffvec.data(), recvbuffvec.size());
      if(sentrecvbytes < 0){perror("sentrecv");close(csocketfd);exit(1);}
      if(sentrecvbytes == 0){exit(0);}
      std::vector<std::string> sVectorString = split(std::string(recvbuff), "\n");
      std::vector<std::string> sVectorStringfirstline = split(sVectorString[0], " ");
      if(sVectorStringfirstline[0] == "GET"){

      if(sVectorStringfirstline[1] == "/"){

    std::ifstream htmlFile("html/get_index.html");
    if (!htmlFile.is_open()) {std::cerr << "Failed to open index.html\n";exit(1);}
    std::string htmlContent((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
    std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(htmlContent.size()) + "\r\n\r\n" + htmlContent;
    // Send HTML content to the client
    int bytesSent = send(csocketfd, httpResponse.c_str(), httpResponse.size(), 0);
    if (bytesSent < 0) {std::cerr << "Error sending HTML content\n";clientid--;exit(1);} 

      }
      else if (sVectorStringfirstline[1] == "/afile")
      {
    std::ifstream htmlFile("html/afile.html");
    if (!htmlFile.is_open()) {std::cerr << "Failed to open afile.html\n";exit(1);}
    std::string htmlContent((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
    std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(htmlContent.size()) + "\r\n\r\n" + htmlContent;
    // Send HTML content to the client
    int bytesSent = send(csocketfd, httpResponse.c_str(), httpResponse.size(), 0);
    if (bytesSent < 0) {std::cerr << "Error sending HTML content\n";clientid--;exit(1);} 
        
      }
      else{

           std::ifstream htmlFile("html/error.html");
    if (!htmlFile.is_open()) {std::cerr << "Failed to open error.html\n";exit(1);}
    std::string htmlContent((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
    std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(htmlContent.size()) + "\r\n\r\n" + htmlContent;
    // Send HTML content to the client
    int bytesSent = send(csocketfd, httpResponse.c_str(), httpResponse.size(), 0);
    if (bytesSent < 0) {std::cerr << "Error sending HTML content\n";clientid--;exit(1);} 



      }
      


      }
      else if (sVectorStringfirstline[0] == "HEAD")
      {

            std::ifstream htmlFile("html/get_index.html");
     if (!htmlFile.is_open()) {std::cerr << "Failed to open index.html\n";exit(1);}
    std::string htmlContent((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
    std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(htmlContent.size());
    // Send HTML content to the client
    int bytesSent = send(csocketfd, httpResponse.c_str(), httpResponse.size(), 0);
    if (bytesSent < 0) {std::cerr << "Error sending HTML content\n";clientid--;exit(1);} 

      }
      else if (sVectorStringfirstline[1] == "/afile")
      {
    std::ifstream htmlFile("html/afile.html");
    if (!htmlFile.is_open()) {std::cerr << "Failed to open afile.html\n";exit(1);}
    std::string htmlContent((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
    std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(htmlContent.size());
    // Send HTML content to the client
    int bytesSent = send(csocketfd, httpResponse.c_str(), httpResponse.size(), 0);
    if (bytesSent < 0) {std::cerr << "Error sending HTML content\n";clientid--;exit(1);} 

        
      }
      else{

    std::ifstream htmlFile("html/error.html");
    if (!htmlFile.is_open()) {std::cerr << "Failed to open error.html\n";exit(1);}
    std::string htmlContent((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
    std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(htmlContent.size());
    // Send HTML content to the client
    int bytesSent = send(csocketfd, httpResponse.c_str(), httpResponse.size(), 0);
    if (bytesSent < 0) {std::cerr << "Error sending HTML content\n";clientid--;exit(1);} 



      }
      
      }
      else{

      }
    

      delete recvbuff;
    } //while inside pid==0
            }   // end of pid == 0 if loop

} //end master socker while loop

printf("done.\n");
return(0);
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