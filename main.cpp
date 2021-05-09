
//========================================================================================

// Welcome to the FalconSQL monitor.  Commands do not end with
// Your FalconSQL connection id is 9
// Server version: v1.1
// Copyright (c) 2021, Falcon and/or its affiliates
// Falcon is a registered trademark of Falcon Corporation and/or its
// affiliates. Other names may be trademarks of their respective owners. just joking.

//========================================================================================

#include <bits/stdc++.h>
#include "inputBuffer.h"
#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int numberOfChildren(TreeNode<string>* root, string x)
{
    int numChildren = 0;

    if (root == NULL)
        return 0;
    queue<TreeNode<string>*> q;
    q.push(root);

    while (!q.empty()) {
        int n = q.size();

        while (n > 0) {
            TreeNode<string>* p = q.front();
            q.pop();
            if (p->name == x) {
                numChildren = numChildren + p->child.size();
                return numChildren;
            }
            for (int i = 0; i < p->child.size(); i++)
                q.push(p->child[i]);
            n--;
        }
    }
    return numChildren;
}


void print_prompt(){
    cout<<"falconSql> ";
}

char DbName[30];
char tName[20];

int DbNo;
int tNo;

int main(int argc, char *argv[]){
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[255];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");

    TreeNode<string>* root = new TreeNode<string>("root");
    InputBuffer* input_buffer = new InputBuffer();
    while(true){
        string to_send("");
        bzero(buffer,255);
        n = read(newsockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");
        // printf("%s",buffer);
        input_buffer->setBuffer(buffer);
        char* tempBuffer = input_buffer->getBuffer();
        if (strncmp(tempBuffer, ".hi",3) == 0) {
            // cout<<"Hi!"<<endl;
            to_send.append("Hi!");
        }
        else if (strncmp(tempBuffer, ".exit",5) == 0) {
            // cout<<"Bye!"<<endl;
            to_send.append("Bye");
        }
        else if (strncmp(tempBuffer,"use ",4) == 0){
            int args_assigned = sscanf(input_buffer->getBuffer(),"use %s",DbName);
            if (args_assigned == 1){
                int temp;
                temp = numberOfChildren(root,"root");
                for (int i=0;i<temp;i++){
                    if ((root->child[i])->name == DbName){
                        DbNo = i;
                    }
                }
                // cout<<"Database assigned"<<endl;
                to_send.append("Database assigned");
            }
            else{
                // cout<<"You have an error in your SQL syntax; check the manual that corresponds to your FalconSQL server version for the right syntax to use near "<<input_buffer->getBuffer()<<endl;
                to_send.append("You have an error in your SQL syntax; check the manual that corresponds to your FalconSQL server version for the right syntax to use near ").append(input_buffer->getBuffer());
            }
        }
        else if (strncmp(tempBuffer,"create database",15) == 0){
            
            int args_assigned = sscanf(input_buffer->getBuffer(),"create database %s",DbName);
            if (args_assigned == 1){
                (root->child).push_back(new TreeNode<string>(DbName));
                // cout<<"Database created"<<" "<<DbName<<endl;
                to_send.append("Database created ").append(DbName);
            }
            else{
                // cout<<"You have an error in your SQL syntax; check the manual that corresponds to your FalconSQL server version for the right syntax to use near "<<input_buffer->getBuffer()<<endl;
                to_send.append("You have an error in your SQL syntax; check the manual that corresponds to your FalconSQL server version for the right syntax to use near ").append(input_buffer->getBuffer());
            }
            

        }
        else if (strncmp(tempBuffer,"create table",12) == 0){
            
            int args_assigned = sscanf(input_buffer->getBuffer(),"create table %s",tName);\
            if (args_assigned == 1){
                (root->child[DbNo]->child).push_back(new TreeNode<string>(tName));
                // cout<<"Table created"<<endl;
                to_send.append("Table created");
            }
            // (root->child[DbNo]->child).push_back(new TreeNode<string>(tName));
            // cout<<"Table created\n"<<endl;
            else{
                // cout<<"You have an error in your SQL syntax; check the manual that corresponds to your FalconSQL server version for the right syntax to use near "<<input_buffer->getBuffer()<<endl;
                to_send.append("You have an error in your SQL syntax; check the manual that corresponds to your FalconSQL server version for the right syntax to use near ").append(input_buffer->getBuffer());
            }
        }

        else if (strncmp(tempBuffer,"show tables",11) == 0){
            int temp;
            //temp = (root->child[DbNo]->child2).size();
            temp = numberOfChildren(root,DbName);
            if (temp ==0 ){
              to_send.append("Empty set");
            }
            for (int i=0;i<temp;i++){
                // cout<<(root->child[DbNo]->child[i])->name<<endl;
                to_send.append((root->child[DbNo]->child[i])->name).append("\n");
            }
        }

        else if (strncmp(tempBuffer,"show databases",11) == 0){
            int temp;
            temp = numberOfChildren(root,"root");
            if (temp == 0){
              to_send.append("Empty set");

            }
            for (int i=0;i<temp;i++){
                // cout<<(root->child[i])->name<<endl;
                to_send.append((root->child[i])->name).append("\n");
            }
        }

        else if (strncmp(tempBuffer,"change database",15) == 0){
            int args_assigned = sscanf(input_buffer->getBuffer(),"change database %s",DbName);
            if (args_assigned == 1){
                int temp;
                temp = numberOfChildren(root,"root");
                for (int i=0;i<temp;i++){
                    if ((root->child[i])->name == DbName){
                        DbNo = i;
                    }
                }
                // cout<<"Database changed"<<endl;
                to_send.append("Database changed");
            }
            else{
                // cout<<"You have an error in your SQL syntax; check the manual that corresponds to your FalconSQL server version for the right syntax to use near "<<input_buffer->getBuffer()<<endl;
                to_send.append("You have an error in your SQL syntax; check the manual that corresponds to your FalconSQL server version for the right syntax to use near ").append(input_buffer->getBuffer());
            }
        }

        else if (strncmp(tempBuffer,"select *",8) == 0){
            int args_assigned = sscanf(input_buffer->getBuffer(),"select * from %s",tName);
            if (args_assigned == 1){
                int ttemp;
                ttemp = numberOfChildren(root,DbName);
                for (int i=0;i<ttemp;i++){
                    if ((root->child[DbNo]->child[i])->name == tName){
                        tNo = i;
                    }   
                }
                int temp;
                temp = (root->child[DbNo]->child[tNo]->child2).size();
                // cout<<temp<<endl;
                // temp = numberOfChildren(root,tName);
                for (int i=0;i<temp;i++){
                    to_send.append((root->child[DbNo]->child[tNo]->child2[i])->printList());
                }
                // cout<<"Query OK,"<<" "<<"rows in set "<<tName<<endl;
                to_send.append("Query OK.").append(" ").append(tName).append(" affected");
            }
            else{
                // cout<<"You have an error in your SQL syntax; check the manual that corresponds to your FalconSQL server version for the right syntax to use near \n"<<input_buffer->getBuffer()<<endl;
                to_send.append("You have an error in your SQL syntax; check the manual that corresponds to your FalconSQL server version for the right syntax to use near ").append(input_buffer->getBuffer());
            }
    
        }

        else if (strncmp(tempBuffer,"insert into",11) == 0){
            char e1[20],e2[20],e3[20],e4[20],e5[20];
            int args_assigned = sscanf(input_buffer->getBuffer(),"insert into %s %s %s %s %s %s",tName,e1,e2,e3,e4,e5);
            if (args_assigned == 6){
                
                int ttemp;
                ttemp = numberOfChildren(root,DbName);
                for (int i=0;i<ttemp;i++){
                    if ((root->child[DbNo]->child[i])->name == tName){
                        tNo = i;
                    }
                }
                (root->child[DbNo]->child[tNo]->child2).push_back(new LinkedList<string>(e1,e2,e3,e4,e5));

                // cout<<"Query OK, 1 row affected"<<endl;
                to_send.append("Query OK, 1 row affected");
            }
            else{
                // cout<<"You have an error in your SQL syntax; check the manual that corresponds to your FalconSQL server version for the right syntax to use near "<<input_buffer->getBuffer()<<endl;
                to_send.append("You have an error in your SQL syntax; check the manual that corresponds to your FalconSQL server version for the right syntax to use near ").append(input_buffer->getBuffer());
            }
            
        }

        else if (strncmp(tempBuffer,"delete last row from",20) == 0){
            sscanf(input_buffer->getBuffer(),"delete last row from %s",tName);
            int ttemp;
            ttemp = numberOfChildren(root,DbName);
            for (int i=0;i<ttemp;i++){
                if ((root->child[DbNo]->child[i])->name == tName){
                    tNo = i;
                }
            }
            (root->child[DbNo]->child[tNo]->child2).pop_back();
            // cout<<"Query OK, 1 row affected"<<endl;
            to_send.append("Query OK, 1 row affected");
        }

        else {
            // printf("Unrecognized command '%s'.\n", input_buffer->getBuffer());
            to_send.append("Unrecognized command ").append(input_buffer->getBuffer());
        }

        bzero(buffer,255);
        strcpy(buffer, to_send.c_str());
        // fgets(buffer,255,stdin);
        n = write(newsockfd,buffer,strlen(buffer));
        if (n < 0) error("ERROR writing to socket");
    }

    close(newsockfd);
    close(sockfd);
    return 0;
    
}


    // TreeNode<string>* root = new TreeNode<string>("root");
    // (root->child).push_back(new TreeNode<string>("test"));
    // (root->child[0]->child).push_back(new TreeNode<string>("student"));
    // (root->child[0]->child[0]->child2).push_back(new LinkedList<string>(5));
    // (root->child[0]->child[0]->child2[0])->printList();
