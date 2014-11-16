/*
C socket server example, handles multiple clients using threads
referred to the site
http://www.binarytides.com/server-client-example-c-sockets-linux/
*/

#include <errno.h>
#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h> //for threading, link with lpthread
#include <iostream>
#include "SlideWindow.h"

using namespace std;
//the thread function
void *connection_handler(void *);
void ErrorHandling(char *message);
int number_thread = 0; // 쓰레드 개수

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;

//Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);//IPV4 프로토콜, TCP/IP 프로토콜
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

//Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

//Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
//print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

//Listen
    listen(socket_desc , 3);//success returns zero


//Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
//Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
        cout << "#CLIENT CONNECTED IP : " << inet_ntoa(client.sin_addr) << " PORT : " << client.sin_port << endl;

        pthread_t sniffer_thread;
        new_sock = (int*)malloc(1);//to avoid invalid conversion
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

//Now join the thread , so that we dont terminate before the thread
//pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}

/*
* This will handle connection for each client
* */
void *connection_handler(void *socket_desc)
{
//Get the socket descriptor
    int sock = *(int*)socket_desc;

    cout << number_thread << "번째 쓰레드 생성" << endl;
    cout << "socket " << sock << "번이 client socket을 handling합니다." << endl;

// buffer to hold our recived data
    char CMD[40] = "";
// buffer to hold our sent data
    char sendMSG[80] = "";
// for error checking
    int res;
// our recv loop
    while(true)
    {
        res = recv(sock,CMD,sizeof(CMD),0); // recv bufs
        sleep(10);
        if(res == 0 || res == -1)//client socket disconnected on unintension
        {
            number_thread--;
            printf("Error : 클라이언트와 연결이 끊겼습니다.\n => 소켓과 스레드가 삭제됩니다. (스레드 %d개)\n", number_thread);
            free(socket_desc);//closesocket(current_client); // free the socket pointer
            //ExitThread(0);
        }
        if(strstr(CMD,"password"))//if client logs in to server to download files
        {
            printf("\n클라이언트로부터 파일다운로드 요청을 받았습니다. 다운로드 권한을 부여하겠습니다.\n");
//grant authentication to client
            strcpy(sendMSG, "Permitted");
            sleep(10);
            send(sock,sendMSG,strlen(sendMSG),0);
            strcpy(sendMSG, "");

//send the file list 직접 수동으로 파일 목록을 입력해야하는 번거로움이 있다.
            strcpy(sendMSG,"1. TCP_MULTI_THREAD_SERVER.cpp\n2. MyStruct.h\n");
            sleep(10);
            send(sock,sendMSG,strlen(sendMSG),0);
            strcpy(sendMSG, "");


//receive file name and check whether client wants to download
            char file_name[50] = "";
            recv(sock,file_name,sizeof(file_name),0);
            cout << "클라이언트가 다운로드할 파일이름 : " << file_name << endl;

//GET FILE_SIZE
            FILE *fp2;
            fp2 = fopen(file_name, "r");
            fseek(fp2, 0, SEEK_END);
            int file_size = ftell(fp2);
//GET FREQUENCY TO LOAD
            int frequency;
            if(file_size % (FILE_BUFSIZE+1) == 0 )
            {
                frequency = file_size / FILE_BUFSIZE;
            }
            else
            {
                frequency = file_size / FILE_BUFSIZE + 1;
            }

            int check_frequency = 0;

            if(strcmp(file_name, "TCP_MULTI_THREAD_SERVER.cpp") == 0 || strcmp(file_name, "MyStruct.h") == 0 || strcmp(file_name, "hello.hwp") == 0)
            {
//불러올 데이터의 크기가 0일 때는 고려하지 않았다.
//SEND FILE
                FILE *fp;
                fp = fopen(file_name,"r");
                if(fp == NULL)
                {
                    ErrorHandling("File open error");
                }
                SlideWindow s1;
                int TimeOutHappens = 0;
                int ReceivedNAK = 0;
                int len;
                int sequence_number = 0;

                int nErrorCode;
                int timeout = 10000; //in milliseconds. this is 10 seconds
                nErrorCode = setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(int)); //setting the receive timeout
//Timer setting error check
                int temp_start = 0;
                while(true)
                {
//DATA LOAD TO SLIDE WINDOW
                    char temp_data[FILE_BUFSIZE+1] = "";//NULL을 위한 크기 + 1
                    if(TimeOutHappens == 0 && ReceivedNAK == 0)
                    {
                        for(int i=0; i<window_size; i++)
                        {
                            if(s1.MyStructAccessor()[i].IsData() == false)
                            {
//데이터가 없으면 윈도우에 데이터를 로드 시킨다.
                                len=fread(temp_data, sizeof(char), FILE_BUFSIZE, fp);
                                check_frequency++;
                                if(temp_start == 0)
                                {
                                    s1.MyStructAccessor()[i].setFILE_START(true);
                                    temp_start++;
                                }

                                if(len == 0)
                                    break;
                                else if(len < FILE_BUFSIZE)
                                {
                                    cout << "#seq : " << sequence_number << endl;
//GRANT SEQUENCE NUMBER
                                    s1.SEQSTACKAddress().push(sequence_number);
                                    s1.MyStructAccessor()[i].setSequence(sequence_number++);
                                    s1.MyStructAccessor()[i].setBuf(temp_data);
//SET END BIT
                                    s1.MyStructAccessor()[i].setEND();//TRUE
                                    break;
                                }
                                else if(frequency == check_frequency)
                                {
                                    cout << "#seq : " << sequence_number << endl;
//GRANT SEQUENCE NUMBER
                                    s1.SEQSTACKAddress().push(sequence_number);
                                    s1.MyStructAccessor()[i].setSequence(sequence_number++);
                                    s1.MyStructAccessor()[i].setBuf(temp_data);
//SET END BIT
                                    s1.MyStructAccessor()[i].setEND();//TRUE
                                    break;

                                }
                                else
                                {
                                    cout << "#seq : " << sequence_number << endl;
//GRANT SEQUENCE NUMBER
                                    s1.SEQSTACKAddress().push(sequence_number);
                                    s1.MyStructAccessor()[i].setSequence(sequence_number++);
                                    s1.MyStructAccessor()[i].setBuf(temp_data);
                                }

                            }
                        }
//PARITY BIT ADD
//COUNT DATA BITS IF EVEN => ADD 0
//ELSE ADD 1
                        s1.set_parity_bit();
                    }

//SLIDE WINDOW DATA SEND TO PEER
                    getchar();
                    for(int i=0; i<window_size; i++)
                    {
                        if(s1.MyStructAccessor()[i].IsData() == true)
                        {
                            send(sock, (char*)&s1.MyStructAddress(i), sizeof(MyStruct), 0);
                        }
                    }
//WAITING FOR POSITIVE RESPONSE
                    int ret;
                    ret = recv(sock, s1.AccessorResponse(), 40, 0);//sizeof(s1.AccessorResponse())
                    cout << s1.AccessorResponse() << endl;
                    if(errno == ETIMEDOUT)//WSAGetLastError() == WSAETIMEDOUT)//타임아웃 발생시
                    {
                        cout << "TIMEOUT, SEND THE SEGMENT AGAIN" << endl;
                        cout << "#seq : " << s1.MyStructAccessor()[0].getSequence() << endl;
                        cout << "#seq : " << s1.MyStructAccessor()[1].getSequence() << endl;
                        cout << "#seq : " << s1.MyStructAccessor()[2].getSequence() << endl;
                        TimeOutHappens++;
                        continue;
                    }
//IF THERE IS NO TIMEOUT
                    TimeOutHappens = 0;

                    if(strcmp(s1.AccessorResponse(), "ACK") == 0)
                    {
                        s1.MyStructAccessor()[0].AccessorHasData() = false;
                        s1.MyStructAccessor()[1].AccessorHasData() = false;
                        s1.MyStructAccessor()[2].AccessorHasData() = false;
                        s1.MyStructAccessor()[0].setFILE_START(false);
                        s1.MyStructAccessor()[1].setFILE_START(false);
                        s1.MyStructAccessor()[2].setFILE_START(false);
                        s1.setResponse("");
                        ReceivedNAK = 0;
                        strcpy(s1.AccessorResponse(), "");
                        strcpy(s1.MyStructAccessor()[0].DataAccessor(), "");
                        strcpy(s1.MyStructAccessor()[1].DataAccessor(), "");
                        strcpy(s1.MyStructAccessor()[2].DataAccessor(), "");
                    }
                    else if(strcmp(s1.AccessorResponse(), "COMPLETE") == 0)
                    {
                        printf("#파일전송을 완료했습니다.\n");
                        s1.MyStructAccessor()[0].AccessorHasData() = false;
                        s1.MyStructAccessor()[1].AccessorHasData() = false;
                        s1.MyStructAccessor()[2].AccessorHasData() = false;
                        s1.setResponse("");
                        if(feof(fp))
                        {
                            fclose(fp);
                            strcpy(s1.AccessorResponse(), "");
                            break;
                        }
                    }
                    else // "NAK"
                    {
//SEND AGAIN THE PACKET
                        ReceivedNAK++;
                    }
                }
//FILE TRANSFER SUCCESS!
//RELEASE TIMEOUT RECV!! 타임아웃해제시키는거
                timeout = 0; //in milliseconds. this is 3000 seconds
                nErrorCode = setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout,sizeof(int)); //setting the receive timeout
            }
        }
        else if(strstr(CMD,"DISCONNECT")) //if client wants to disconnect
        {
// dissconnected this user
            number_thread--;
            printf("\n클라이언트가 연결을 종료했습니다. => 스레드와 스레드안의 소켓을 삭제합니다. (현재 스레드 개수 %d개)\n", number_thread);
            sleep(10);
// close the socket associted with this client and end this thread
            free(socket_desc);//closesocket(current_client);
            //ExitThread(0);
        }
        else
        {
            strcpy(sendMSG,"Warning! 유효하지 않은 명령어입니다.\n");
            sleep(10);
            send(sock,sendMSG,sizeof(sendMSG),0);
        }
// clear buffers
        strcpy(sendMSG,"");
        strcpy(CMD,"");
    }
    return 0;
}
void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}
