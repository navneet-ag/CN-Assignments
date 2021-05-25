// Navneet Agarwal
// 2018348
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define PORT 9002

int main()
{
	// Client Socket Descripor initialization
	int ClientSideSocket=0;

	// Client Socket Descripor assignment
	ClientSideSocket= socket(AF_INET, SOCK_STREAM,0);

	// strcture to store details of the server
	struct sockaddr_in server ;

	server.sin_family= AF_INET;
	server.sin_port= htons(PORT);
	server.sin_addr.s_addr= INADDR_ANY;

	// Flag to check if connection was successful or not
	int ConnectionFlag =connect(ClientSideSocket, (struct sockaddr *)&server,sizeof(server));

	if(ConnectionFlag<0)
	{
		// If does not connect then print the following message , close the client socket and exit
		printf("Can't Connect to server, Try again in some time\n");
		close(ClientSideSocket);
		exit(1);
	}

	// If the code reaches here , that means connection was successful
	printf("Connection Successful \n");

	// Character array/buffer for sending the messages
	char TextSend[4096];
	// Character array/buffer for receiving the messages	
	char TextRec[4096];
	// This message will be compared to TextRec if File was not found at server end
	char NotFoundMessage[]="Sorry ,File Not Found.Closing Connection";

	// Taking the file name from user and send it to server
	printf("Enter the file name that you need : ");
	scanf("%s",TextSend);
	send(ClientSideSocket,TextSend,strlen(TextSend),0);

	printf("File Name Sent to Server.\n");

	//Receive message about file was found or not 	
	read(ClientSideSocket, TextRec,4096);

	//If condition will be executed if the file was not found at the server's end 
	// Length(NotFoundMessage) =40
	if(strncmp(NotFoundMessage,TextRec,40)==0)
	{
		printf("%s\n",TextRec );
		close(ClientSideSocket);
		exit(1);
	}
	else
	{
		// Code reaches here if the server says, file found and it will start the transfer
		// Creating a file with the required name
		FILE *Writefile= fopen(TextSend,"wb");
		printf("Text Received : %s \n",TextRec);

		//The below statement is used to empty the buffer 
		memset(TextSend,0,sizeof(TextSend));

		// The following statements send message to user that the client is now ready to receive the files
		char FoundResponse[]="Ok";
		strncpy(TextSend,FoundResponse,strlen(FoundResponse));
		send(ClientSideSocket,TextSend,strlen(TextSend),0);

		// Clearing the buffer for further messages
		memset(TextRec,0,sizeof(TextRec));


		// The following variable store the number of characters read from the server in the variable NumberofcharactersRead

		int NumberofcharactersRead = read(ClientSideSocket,TextRec,4096);

		while(NumberofcharactersRead >0)
		{
			fwrite(TextRec,sizeof(char),NumberofcharactersRead,Writefile);
			memset(TextRec,0,sizeof(TextRec));
			NumberofcharactersRead = read(ClientSideSocket,TextRec,4096);
		}

		// Clsong the file pointer
		fclose(Writefile);

		// CLient Closing the Connection
		printf("File Received,Now Closing the Connection.\n");
		close(ClientSideSocket);

	}
	return 0;

}