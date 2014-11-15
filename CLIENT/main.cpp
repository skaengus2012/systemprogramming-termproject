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

#define BUFSIZE 40

using namespace std;

void ErrorHandling(char *message);
int main(int argc, char **argv)
{

    int sock;
    struct sockaddr_in servAddr;

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");


    char server_address[20];
    int server_port;

    while(true)
    {
        printf("접속할 서버의 IP 주소를 입력하세요\n=> ");
        gets(server_address);
        if(strlen(server_address) > 16)//IPV4ÁÖŒÒÀÇ ÃÖŽë Å©±âºžŽÙ Å©ŽÙžé
        {
            printf("Error : IP주소를 다시 입력하세요. (PRESS ENTER TO CONTINUE)\n");
            getchar();
            system("cls");
            continue;
        }
        printf("접속할 서버의 PORT번호를 입력하세요.\n=> ");
        scanf("%d", &server_port);
        if(server_port < 0)
        {
            printf("Error : PORT번호를 다시 입력하세. (PRESS ENTER TO CONTINUE)\n");
            getchar();
            system("cls");
            continue;
        }
        //set the server address and port number you want to connect to
        servAddr.sin_addr.s_addr=inet_addr(server_address);
        servAddr.sin_port=htons(server_port);
        servAddr.sin_family = AF_INET;


        //if connect succeeds, it returns 0
        if(connect(sock,(struct sockaddr *)&servAddr,sizeof(servAddr))<0)
        {
            printf(" connect() error! (PRESS ENTER TO CONTINUE)");
            getchar();
            system("cls");
            continue;
        }
        break;
    }
    printf("#connected to server\r\n");

    int select;
    while(true)
    {
        printf("========================\n");
        printf("1. 파일다운로\n");
        printf("2. 연결 종료\n");
        printf("========================\n\n");
        printf("=> ");
        scanf("%d", &select);
        getchar();//scanfㅎ 입력 후 공백 제거
        if(select == 1)//Get authentication to download files
        {
            system("cls");
            printf("로그인이 필요합니다. 패스워드를 입력하세요.\n => ");
            char password[10];
            gets(password);
            send(sock, password, strlen(password), 0);
            //get response from server
            char Permitted[100] = "";
            recv(sock, Permitted, sizeof(Permitted), 0);

            //if permitted to download
            if(strcmp("Permitted", Permitted) == 0)
            {
                printf("축하합니다. 서버로부터 다운로드가 가능합니다.\n\n");
                //show the file list
                printf("[파일목록]\n");
                char list_of_files[1000] = "";
                recv(sock, list_of_files, sizeof(list_of_files), 0);//get the list
                cout << list_of_files << endl;//print the list

                //write the file name
                char file_name[50] = "";
                printf("\n파일 이름과 확장명을 입력하세요.(e.g. file.txt)\n");
                printf("=> ");
                gets(file_name);

                //ask for the file
                send(sock, file_name, sizeof(file_name), 0);
                SlideWindow s1;
                FILE* fp;
                fp = fopen("download.txt", "w");
                if(fp==NULL)
                {
                    printf("Error : Can not open file\n");
                    continue;
                }
                int test = 0;
                //DOWNLOAD : loop until download finished
                for(int i=0; i<window_size; i++)
                {
                    int Error;
                    Error =	recv(sock, (char *)&s1.MyStructAddress(i), sizeof(MyStruct), 0);

                    cout << s1.MyStructAccessor()[i].getFILE_START() << " " << s1.MyStructAccessor()[i].getEND() << " " << s1.MyStructAccessor()[i].IsData() << endl;

                    //윈도우 버퍼 첫번째가 파일의 마지막일 때
                    if(s1.MyStructAccessor()[0].getEND() == true && s1.MyStructAccessor()[0].IsData() == true)
                    {
                        //stack sequence
                        cout << "#seq : " << s1.MyStructAccessor()[0].getSequence() << endl;
                        s1.SEQSTACKAddress().push(s1.MyStructAccessor()[0].getSequence());
                        fwrite(s1.MyStructAccessor()[0].DataAccessor(), sizeof(char), strlen(s1.MyStructAccessor()[0].DataAccessor()),fp);

                        //FILE WRITE DONE
                        fclose(fp);
                        cout << "DOWNLOAD COMPLETE" << endl;
                        send(sock, "COMPLETE", 9, 0);
                        i=window_size-1;
                        continue;

                    }
                    else if(s1.MyStructAccessor()[1].getEND() == true && s1.MyStructAccessor()[1].IsData() == true)//두번째가 파일의 마지막일 때
                    {
                        //stack sequence
                        cout << "#seq : " << s1.MyStructAccessor()[0].getSequence() << endl;
                        cout << "#seq : " << s1.MyStructAccessor()[1].getSequence() << endl;
                        s1.SEQSTACKAddress().push(s1.MyStructAccessor()[0].getSequence());
                        s1.SEQSTACKAddress().push(s1.MyStructAccessor()[1].getSequence());
                        fwrite(s1.MyStructAccessor()[0].DataAccessor(), sizeof(char), strlen(s1.MyStructAccessor()[0].DataAccessor()),fp);
                        fwrite(s1.MyStructAccessor()[1].DataAccessor(), sizeof(char), strlen(s1.MyStructAccessor()[1].DataAccessor()),fp);
                        //FILE WRITE DONE
                        fclose(fp);
                        cout << "DOWNLOAD COMPLETE" << endl;
                        send(sock, "COMPLETE", 9, 0);
                        i=window_size-1;
                        continue;

                    }

                    //getchar();


                    //IF DISCONNECTED
                    //PROGRAM EXIT!

                    //SEGMENT CHANGED! => ERROR
                    /*
                    if(s1.MyStructAccessor()[0].getParity() == 0)
                    {
                    	s1.MyStructAccessor()[0].setParity(1);
                    }else
                    {
                    	s1.MyStructAccessor()[0].setParity(0);
                    }
                    */



                    if(i == window_size-1)
                    {
                        //start & finish & parity CHECK
                        if(s1.check_start_n_finish() == false || s1.parity_check() == false)
                        {
                            //send NAK RESPONSE
                            //DUMP ALL THE DATA RECEIVED
                            //RECEIVE AGAIN
                            send(sock, "NAK", 4, 0);
                            i = 0;
                            continue;
                        }
                        //sequence check
                        if(s1.sequence_check() == false)
                        {
                            //send NAK RESPONSE
                            //DUMP ALL THE DATA RECEIVED
                            //RECEIVE AGAIN
                            send(sock, "NAK", 4, 0);
                            i = 0;
                            continue;
                        }

                        if( ( s1.SEQSTACKAddress().peek() > s1.MyStructAccessor()[0].getSequence() ) && s1.MyStructAccessor()[i-2].getFILE_START() ==false)
                        {
                            //send NAK RESPONSE
                            //DUMP ALL THE DATA RECEIVED
                            //RECEIVE AGAIN
                            send(sock, "NAK", 4, 0);
                            getchar();
                            i = 0;
                            continue;
                        }

                        //NO ERROR! => WRITE FILE
                        for(int j=0; j<window_size; j++)
                        {
                            //stack sequence
                            cout << "#seq : " << s1.MyStructAccessor()[j].getSequence() << endl;
                            s1.SEQSTACKAddress().push(s1.MyStructAccessor()[j].getSequence());
                            fwrite(s1.MyStructAccessor()[j].DataAccessor(), sizeof(char), strlen(s1.MyStructAccessor()[j].DataAccessor()),fp);
                            if(s1.MyStructAccessor()[j].getEND() == true)
                            {
                                //FILE WRITE DONE
                                fclose(fp);
                                cout << "DOWNLOAD COMPLETE" << endl;
                                send(sock, "COMPLETE", 9, 0);
                                break;
                            }
                        }
                        //getchar();
                        cout << "SEND ACK" << endl << endl;
                        send(sock, "ACK", 4, 0);
                        strcpy(s1.MyStructAccessor()[0].DataAccessor(), "");
                        strcpy(s1.MyStructAccessor()[1].DataAccessor(), "");
                        strcpy(s1.MyStructAccessor()[2].DataAccessor(), "");
                        s1.MyStructAccessor()[0].setHasData(false);
                        s1.MyStructAccessor()[1].setHasData(false);
                        s1.MyStructAccessor()[2].setHasData(false);
                        i=-1;
                    }
                }
            }
            else //password incorrect
            {
                printf("Error : password incorrect\n");
                strcpy(Permitted,"");
                continue;
            }
        }
        else if(select == 2) //connection cancel
        {
            send(sock, "DISCONNECT", 11, 0);
            close(sock);//closesocket(hSocket);
            //WSACleanup(); in linux wsastartup and wsacleanup is to not do anything the sockets library is initizlized automatically.
            break;
        }
        else
        {
            system("cls");
        }
    }
    return 0;
}
void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}
