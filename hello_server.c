#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void err(char *message);

int main(int argc, char *argv[]) {
	int serv_sock;
	int client_sock;
	struct sockaddr_in serv_addr; //sockaddr_in 구조체 설정. ip는 클라이언트에서 명시함
	struct sockaddr_in client_addr;
	socklen_t client_addr_size;
	char message[] = "hello world!";

	if(argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(0);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)err("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr)); //세팅하고자 하는 메모리의 시작 주소, 메모리에 세팅되고자 하는 값, 메모리의 크기
	serv_addr.sin_family = AF_INET; //sin_family:주소체계. AF_INET:IPv4 프로토콜을 사용하겠다는 뜻
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //sin_addr:존재하는 랜카드의 ip할당. INADDR_ANY:컴퓨터에 존재는 랜카드 중 사용가능한 랜카드의 ip주소를 사용
	serv_addr.sin_port = htons(atoi(argv[1])); //port할당. host to network short : host byte순서-> 네트워크byte순서. short는 2byte니까 port번호 전달
						   //htonl : host to network long. 4byte ip주소 전달
	
	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
		err("bind()error");

	if (listen(serv_sock, 5)==-1)
		err("listen()error");

	client_addr_size = sizeof(client_addr);
	client_sock = accept(serv_sock, (struct sockaddr*)&client_addr, &client_addr_size); //소켓 식별번호, client의 ip,port가 담기는 구조체, 구조체의 크
	if(client_sock == -1)err("accepct()error");
	
	write(client_sock, message, sizeof(message));
	close(client_sock);
	close(serv_sock);
	return 0;
}

void err(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

