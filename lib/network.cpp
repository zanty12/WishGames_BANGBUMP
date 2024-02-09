#include "network.h"

namespace Network {
	Address::Address(int af, const char *addr, int port) {
		memset(&addr_in, 0, sizeof(addr_in));

		addr_in.sin_family = af;
		addr_in.sin_addr.s_addr = inet_addr(addr);
		addr_in.sin_port = htons(port);
	}

	Address::Address(int af, int addr, int port) {
		memset(&addr_in, 0, sizeof(addr_in));

		addr_in.sin_family = af;
		addr_in.sin_addr.s_addr = htonl(addr);
		addr_in.sin_port = htons(port);
	}

	Address::Address(int af, ULONG addr, int port) {
		memset(&addr_in, 0, sizeof(addr_in));

		addr_in.sin_family = af;
		addr_in.sin_addr.s_addr = addr;
		addr_in.sin_port = htons(port);
	}

	char *Address::ToString() {
		return inet_ntoa(addr_in.sin_addr);
	}

	int Socket::Bind(Address addr) {
		sockaddr_in addr_in = addr;
		return bind(sockfd, (sockaddr *)&addr_in, sizeof(addr_in));
	}



	void FD::Add(Socket socket) {
		FD_SET(socket, &fds);
		if (max_fd < socket) max_fd = socket;
	}

	int FD::Contains(Socket socket) {
		return FD_ISSET(socket, &fds);
	}

	void FD::Remove(Socket socket) {
		FD_CLR(socket, &fds);
		max_fd = -1;
		for (int i = 0; i < fds.fd_count; i++) {
			if (max_fd < fds.fd_array[i]) max_fd = fds.fd_array[i];
		}
	}


	int Startup(Version version, WSADATA &wsa) {
		WORD w_version = 0;
		switch (version)
		{
		case v1_0: w_version = MAKEWORD(1, 0); break;
		case v1_1: w_version = MAKEWORD(1, 1); break;
		case v2_0: w_version = MAKEWORD(2, 0); break;
		case v2_1: w_version = MAKEWORD(2, 1); break;
		case v2_2: w_version = MAKEWORD(2, 2); break;
		}
		return WSAStartup(MAKEWORD(2, 2), &wsa);
	}

	int Cleanup(void) {
		return WSACleanup();
	}

	int Listen(Socket server, int listenNum) {
		return listen(server, listenNum);
	}

	int Connect(Socket client, Address addr) {
		return connect(client, (sockaddr *)&addr, sizeof(addr));
	}

	Socket Accept(Socket server, Address *addr, int *addrLen) {
		return accept(server, (sockaddr *)addr, addrLen);
	}

	int Recv(Socket recvSocket, char *buff, int buffLen, int flag) {
		return recv(recvSocket, buff, buffLen, flag);
	}

	int Send(Socket sendSocket, char *buff, int buffLen, int flag) {
		return send(sendSocket, buff, buffLen, flag);
	}

	int RecvFrom(Socket recvSocket, char *buff, int buffLen, int flag, Address *addr, int *addrLen) {
		return recvfrom(recvSocket, buff, buffLen, flag, (sockaddr *)addr, addrLen);
	}

	int SendTo(Socket sendSocket, char *buff, int buffLen, int flag, Address addr) {
		return sendto(sendSocket, buff, buffLen, flag, (sockaddr *)&addr, sizeof(addr));
	}

	int Select(FD *readfds, FD *writefds, FD *exceptfds) {
		// ファイルディスクリプタの最大値を取得
		int max_fd = -1;
		if (readfds && max_fd < readfds->GetMaxFD()) max_fd = readfds->GetMaxFD();
		if (writefds && max_fd < writefds->GetMaxFD()) max_fd = writefds->GetMaxFD();
		if (exceptfds && max_fd < exceptfds->GetMaxFD()) max_fd = exceptfds->GetMaxFD();

		return select(max_fd + 1, (fd_set *)readfds, (fd_set *)writefds, (fd_set *)exceptfds, nullptr);
	}

	int Select(FD *readfds, FD *writefds, FD *exceptfds, long waitTimeSec, long waitTimeMicro) {
		// ファイルディスクリプタの最大値を取得
		int max_fd = -1;
		if (readfds && max_fd < readfds->GetMaxFD()) max_fd = readfds->GetMaxFD();
		if (writefds && max_fd < writefds->GetMaxFD()) max_fd = writefds->GetMaxFD();
		if (exceptfds && max_fd < exceptfds->GetMaxFD()) max_fd = exceptfds->GetMaxFD();

		// タイムアウトの指定
		timeval tv;
		tv.tv_sec = waitTimeSec;
		tv.tv_usec = waitTimeMicro;

		return select(max_fd + 1, (fd_set *)readfds, (fd_set *)writefds, (fd_set *)exceptfds, &tv);
	}
}
