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
* include宣言
------------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")

/*------------------------------------------------------------------------------
* define宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* extern宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototype宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* enum宣言
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
* class宣言
------------------------------------------------------------------------------*/
namespace Network {
	class Address {
	private:
		sockaddr_in addr_in;



	public:
		Address() { }

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="af">アドレスファミリ</param>
		/// <param name="addr">アドレス</param>
		/// <param name="port">ポート</param>
		Address(int af, const char *addr, int port);

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="af">アドレスファミリ</param>
		/// <param name="addr">アドレス</param>
		/// <param name="port">ポート</param>
		Address(int af, int addr, int port);

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="af">アドレスファミリ</param>
		/// <param name="addr">アドレス</param>
		/// <param name="port">ポート</param>
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
		/// バインド
		/// </summary>
		/// <param name="addr">アドレス</param>
		/// <returns></returns>
		int Bind(Address addr);

		/// <summary>
		/// クローズ
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
	/// スタートアップ
	/// </summary>
	/// <param name="version"></param>
	/// <param name="wsa"></param>
	/// <returns></returns>
	int Startup(Version version, WSADATA &wsa);

	/// <summary>
	/// クリーンアップ
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int Cleanup(void);

	/// <summary>
	/// 接続受付の開始（キューの作成をする）
	/// </summary>
	/// <param name="sockfd"></param>
	/// <param name="listenNum"></param>
	/// <returns></returns>
	int Listen(Socket server, int listenNum = SOMAXCONN);

	/// <summary>
	/// 接続要求
	/// </summary>
	/// <param name="sockfd"></param>
	/// <param name="addr"></param>
	/// <returns></returns>
	int Connect(Socket client, Address addr);

	/// <summary>
	/// 接続受付する
	/// </summary>
	/// <param name="sockfd"></param>
	/// <param name="addr"></param>
	/// <param name="addrLen"></param>
	/// <returns></returns>
	Socket Accept(Socket server, Address *addr, int *addrLen);

	/// <summary>
	/// 受信
	/// </summary>
	/// <param name="sockfd"></param>
	/// <param name="buff"></param>
	/// <param name="buffLen"></param>
	/// <param name="flag"></param>
	int Recv(Socket recvSocket, char *buff, int buffLen, int flag = 0);

	/// <summary>
	/// 送信
	/// </summary>
	/// <param name="sockfd"></param>
	/// <param name="buff"></param>
	/// <param name="buffLen"></param>
	/// <param name="flag"></param>
	int Send(Socket sendSocket, char *buff, int buffLen, int flag = 0);

	/// <summary>
	/// 受信
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
	/// 送信
	/// </summary>
	/// <param name="sendSocket"></param>
	/// <param name="buff"></param>
	/// <param name="buffLen"></param>
	/// <param name="flag"></param>
	/// <param name="addr"></param>
	/// <returns></returns>
	int SendTo(Socket sendSocket, char *buff, int buffLen, int flag, Address addr);

	/// <summary>
	/// ファイルディスクリプタの状態を調べる
	/// </summary>
	/// <param name="readfds">リードファイルディスクリプタ（読み込み）</param>
	/// <param name="writefds">ライトファイルディスクリプタ（書き込み）</param>
	/// <param name="exceptfds">例外ファイルディスクリプタ</param>
	/// <returns></returns>
	int Select(FD *readfds, FD *writefds, FD *exceptfds);

	/// <summary>
	/// ファイルディスクリプタの状態を調べる
	/// </summary>
	/// <param name="readfds">リードファイルディスクリプタ（読み込み）</param>
	/// <param name="writefds">ライトファイルディスクリプタ（書き込み）</param>
	/// <param name="exceptfds">例外ファイルディスクリプタ</param>
	/// <param name="waitTimeSec">待ち時間（秒）</param>
	/// <param name="waitTimeMicro">待ち時間（マイクロ秒）</param>
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
