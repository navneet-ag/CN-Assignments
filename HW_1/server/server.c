// Navneet Agarwal
// 2018348
#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>

#define PORT 9002

int main()
{
	// Server Socket Descripor initialization
	int Serversidesocket;

	// Server Socket Descripor assignment
	Serversidesocket=socket(AF_INET,SOCK_STREAM,0);

	// Structure to store details of the server
	struct sockaddr_in server ;
	int addLen= sizeof(server);
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;

	// Binding the server
	int BindSocketFlag=bind(Serversidesocket, (struct sockaddr *)&server,sizeof(server));

	// This might happen if the port is busy
	// Sometime happens due to TIME_WAIT
	// Even if we close the socket , the kernel reserves it for some time and does not
	// allow any program to use it due to which TIME_WAIT is present
	if(BindSocketFlag<0)
	{
		printf("Can't Bind to this port\n");
		close(Serversidesocket);
		return 0;
	}

	// Listen
	int ListenFlag=listen(Serversidesocket,0);
	if(ListenFlag<0)
	{
		printf("Can't listen, Restart the server\n");
		close(Serversidesocket);
		return 0;
	}

	// If the code reaches here, it means that the server is ready and will accept connections from client
	printf("Server is Ready now \n");
	
	// Accepting the connection from client and storing it in a descriptor
	// This client variable will be used to accept the connections from the client
	int client;
	client= accept(Serversidesocket,(struct sockaddr *)&server,(socklen_t *)&addLen);

	// Infinite loop starts
	// It breaks when prompted by the user using the server after a client disconnects
	while(1)
	{

		printf("Client Connected \n");

		// Receiving the file name that the client wants
		char TextRead[4096],TextSend[4096];
		read(client,TextRead,4096);
		printf("Client wants file: %s \n ",TextRead);

		
		FILE  *ReadFile=fopen(TextRead,"rb");

		// If the file is not found then send the apt. message to client regarding this
		if(ReadFile==NULL)
		{

			printf("File not found\n");
			char *TempMessage="Sorry ,File Not Found.Closing Connection";	
			memset(TextSend,0,sizeof(TextSend));
			memset(TextRead,0,sizeof(TextRead));

			strncpy(TextSend,TempMessage,strlen(TempMessage));
			send(client,TextSend,strlen(TextSend),0);
		}
		else
		{
			// Else condition means file was present in server
			memset(TextSend,0,sizeof(TextSend));
			memset(TextRead,0,sizeof(TextRead));

			// This message ensures that the server is ready to send the file
			char *TempMessage="File Found, starting transfer.";				
			strncpy(TextSend,TempMessage,strlen(TempMessage));
			send(client,TextSend,strlen(TextSend),0);

			// Reading from client to know if client is ready
			read(client,TextRead,4096);

			// If executed if client was ready
			if(strncmp(TextRead,"Ok",2)==0)
			{
					//Clearing the contents of the Textsend buffer 
					memset(TextSend,0,sizeof(TextSend));

					// This will store the Number of characters that the server will send for a file
					int NumberofCharsRead;

					// Start reading from a file
					NumberofCharsRead=fread(TextSend,sizeof(char),4096,ReadFile);
					
					// We read until the end of the file
					while(NumberofCharsRead>0)
					{
						send(client,TextSend,NumberofCharsRead,0);
						memset(TextSend,0,sizeof(TextSend));
						NumberofCharsRead=fread(TextSend,sizeof(char),4096,ReadFile);
					}

					// Closing the file pointer
					fclose(ReadFile);
					close(client);


			printf("File transfer Successful \n");

			memset(TextSend,0,sizeof(TextSend));
			close(client);

			}
			else
			{
				// client wasn't ready to accept the file.
				printf("Connection Lost\n");
				close(client);
			}
		}

		printf("Client Disconnected\n");

		int ClientFlag=1;

		// Ask the user if he wants to close the server or wait for another client
		
		printf("\nChoose from the following :\n");
		printf("Enter '1' to Shutdown the server\n");
		printf("or Enter '2' to Wait for another client\n");
		scanf("%d",&ClientFlag);
		
		// shutdown the server if client choses 1
		if(ClientFlag==1)
			break;
		
		// else wait for another client
		printf("\nSever is waiting for a client now \n");
		client= accept(Serversidesocket,(struct sockaddr *)&server,(socklen_t *)&addLen);

	}
	// Finally closing the server socket
		printf("Now closing server\n");
		close(Serversidesocket);

	return 0;

}


