/*==============================================================================

	Title		: Network
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _Network_H_
#define _Network_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")

/*------------------------------------------------------------------------------
* define�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* extern�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototype�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* enum�錾
------------------------------------------------------------------------------*/
namespace Network {
	enum Version {
		v1_0,
		v1_1,
		v2_0,
		v2_1,
		v2_2,
	};

	enum AddressFamily {
		IPV4 = AF_INET,
	};

	enum Type {
		TCP = SOCK_STREAM,
		UDP = SOCK_DGRAM,
	};
}


/*------------------------------------------------------------------------------
* class�錾
------------------------------------------------------------------------------*/
namespace Network {
	class Address {
	private:
		sockaddr_in addr_in;



	public:
		Address() { }

		/// <summary>
		/// ������
		/// </summary>
		/// <param name="af">�A�h���X�t�@�~��</param>
		/// <param name="addr">�A�h���X</param>
		/// <param name="port">�|�[�g</param>
		Address(int af, const char *addr, int port);

		/// <summary>
		/// ������
		/// </summary>
		/// <param name="af">�A�h���X�t�@�~��</param>
		/// <param name="addr">�A�h���X</param>
		/// <param name="port">�|�[�g</param>
		Address(int af, int addr, int port);

		/// <summary>
		/// ������
		/// </summary>
		/// <param name="af">�A�h���X�t�@�~��</param>
		/// <param name="addr">�A�h���X</param>
		/// <param name="port">�|�[�g</param>
		Address(int af, ULONG addr, int port);

		char *ToString();

		operator sockaddr_in() { return addr_in; }
	};

	class Socket {
	private:
		int sockfd = -1;



	public:
		Socket() { }
		Socket(int sock) : sockfd(sock) { }
		Socket(int af, int type, int protocol) { sockfd = socket(af, type, protocol); }

		/// <summary>
		/// �o�C���h
		/// </summary>
		/// <param name="addr">�A�h���X</param>
		/// <returns></returns>
		int Bind(Address addr);

		/// <summary>
		/// �N���[�Y
		/// </summary>
		/// <param name=""></param>
		void Close(void) { closesocket(sockfd); sockfd = -1; }

		bool IsEmpty(void) { return sockfd <= 0; }

		operator int() { return sockfd; }
	};

	class FD {
	private:
		fd_set fds;
		int max_fd = -1;



	public:

		FD() { FD_ZERO(&fds); max_fd = -1; }

		void Add(Socket socket);

		int Contains(Socket socket);

		void Remove(Socket socket);

		int GetMaxFD(void) { return max_fd; }

		operator fd_set() { return fds; }
	};

	/// <summary>
	/// �X�^�[�g�A�b�v
	/// </summary>
	/// <param name="version"></param>
	/// <param name="wsa"></param>
	/// <returns></returns>
	int Startup(Version version, WSADATA &wsa);

	/// <summary>
	/// �N���[���A�b�v
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int Cleanup(void);

	/// <summary>
	/// �ڑ���t�̊J�n�i�L���[�̍쐬������j
	/// </summary>
	/// <param name="sockfd"></param>
	/// <param name="listenNum"></param>
	/// <returns></returns>
	int Listen(Socket server, int listenNum = SOMAXCONN);

	/// <summary>
	/// �ڑ��v��
	/// </summary>
	/// <param name="sockfd"></param>
	/// <param name="addr"></param>
	/// <returns></returns>
	int Connect(Socket client, Address addr);

	/// <summary>
	/// �ڑ���t����
	/// </summary>
	/// <param name="sockfd"></param>
	/// <param name="addr"></param>
	/// <param name="addrLen"></param>
	/// <returns></returns>
	Socket Accept(Socket server, Address *addr, int *addrLen);

	/// <summary>
	/// ��M
	/// </summary>
	/// <param name="sockfd"></param>
	/// <param name="buff"></param>
	/// <param name="buffLen"></param>
	/// <param name="flag"></param>
	int Recv(Socket recvSocket, char *buff, int buffLen, int flag = 0);

	/// <summary>
	/// ���M
	/// </summary>
	/// <param name="sockfd"></param>
	/// <param name="buff"></param>
	/// <param name="buffLen"></param>
	/// <param name="flag"></param>
	int Send(Socket sendSocket, char *buff, int buffLen, int flag = 0);

	/// <summary>
	/// ��M
	/// </summary>
	/// <param name="recvSocket"></param>
	/// <param name="buff"></param>
	/// <param name="buffLen"></param>
	/// <param name="flag"></param>
	/// <param name="addr"></param>
	/// <param name="addrLen"></param>
	/// <returns></returns>
	int RecvFrom(Socket recvSocket, char *buff, int buffLen, int flag, Address *addr, int *addrLen);

	/// <summary>
	/// ���M
	/// </summary>
	/// <param name="sendSocket"></param>
	/// <param name="buff"></param>
	/// <param name="buffLen"></param>
	/// <param name="flag"></param>
	/// <param name="addr"></param>
	/// <returns></returns>
	int SendTo(Socket sendSocket, char *buff, int buffLen, int flag, Address addr);

	/// <summary>
	/// �t�@�C���f�B�X�N���v�^�̏�Ԃ𒲂ׂ�
	/// </summary>
	/// <param name="readfds">���[�h�t�@�C���f�B�X�N���v�^�i�ǂݍ��݁j</param>
	/// <param name="writefds">���C�g�t�@�C���f�B�X�N���v�^�i�������݁j</param>
	/// <param name="exceptfds">��O�t�@�C���f�B�X�N���v�^</param>
	/// <returns></returns>
	int Select(FD *readfds, FD *writefds, FD *exceptfds);

	/// <summary>
	/// �t�@�C���f�B�X�N���v�^�̏�Ԃ𒲂ׂ�
	/// </summary>
	/// <param name="readfds">���[�h�t�@�C���f�B�X�N���v�^�i�ǂݍ��݁j</param>
	/// <param name="writefds">���C�g�t�@�C���f�B�X�N���v�^�i�������݁j</param>
	/// <param name="exceptfds">��O�t�@�C���f�B�X�N���v�^</param>
	/// <param name="waitTimeSec">�҂����ԁi�b�j</param>
	/// <param name="waitTimeMicro">�҂����ԁi�}�C�N���b�j</param>
	/// <returns></returns>
	int Select(FD *readfds, FD *writefds, FD *exceptfds, long waitTimeSec, long waitTimeMicro);

	class Server {
		WSAData data = {};

	public:
		Server() { Startup(v2_2, data); }

		virtual ~Server() { Cleanup(); }
	};
}

#endif
