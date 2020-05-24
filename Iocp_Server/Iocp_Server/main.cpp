#include"Iocp.h"

Iocp* g_iocp = nullptr;
std::priority_queue<EVENT_ST> g_timer_queue;
std::mutex timer_l;

void ClearPlayerSocket(const char id);

int main() {
	g_iocp = new Iocp();

	//Create worker_thread;
	std::vector<std::thread>worker_threads;

	//Create iocp object
	g_iocp->iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	//Check Number cpu
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	for (int i = 0; i < (int)si.dwNumberOfProcessors; ++i) {
		worker_threads.emplace_back(std::thread{ Worker_thread });
	}

	//Create Accpet_thread;
	std::thread accept_thread{ Accept_thread };

	//Create Timer_thread;
	//std::thread timer_thread{ Timer_thread };

	accept_thread.join();
	//	timer_thread.join();
	for (auto& th : worker_threads) {
		th.join();
	}

	if (g_iocp != nullptr)
		delete g_iocp;
}

void Accept_thread() {

	WSADATA wsa;
	SOCKADDR_IN sockAddr;
	SOCKET listen_sock;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cout << "WSAStartup ERROR" << std::endl;
		return;
	}
	listen_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, NULL, WSA_FLAG_OVERLAPPED);
	if (listen_sock == INVALID_SOCKET) {
		std::cout << "INVALID_SOCK ERROR" << std::endl;
		return;

	}

	ZeroMemory(&sockAddr, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockAddr.sin_port = htons(SERVER_PORT);

	if (bind(listen_sock, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr)) == SOCKET_ERROR) {
		std::cout << "Bind ERROR" << std::endl;
		return;
	}

	if (listen(listen_sock, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Listen ERROR" << std::endl;
		return;
	}

	SOCKADDR_IN clientAddr;
	int addrlen = sizeof(clientAddr);
	ZeroMemory(&clientAddr, sizeof(clientAddr));
	SOCKET client_sock;
	DWORD recvByte = 0;
	DWORD sendByte = 0;
	DWORD flags = 0;

	int count = 0;
	while (1) {

		client_sock = accept(listen_sock, reinterpret_cast<SOCKADDR*>(&clientAddr), &addrlen);
		if (client_sock == INVALID_SOCKET) {
			std::cout << "Accept ERROR" << std::endl;
		}

		std::cout << reinterpret_cast<char*>(&clientAddr.sin_addr) << "접속" << std::endl;

		int new_id = -1;

		for (int i = 0; i < MAX_USER; ++i) {

			if (g_clients[i].m_used == false) {
				new_id = i;
				break;
			}

		}

		//유저가 꽉차면
		if (-1 == new_id) {
			std::cout << "MAX USER overflow" << std::endl;
			continue;
		}

		g_clients[new_id].m_socket = client_sock;

		static float number = 0.0f;

		//임시로 여기서 위치값 할당
		g_clients[new_id].m_x = number;
		g_clients[new_id].m_y = 0.0f;
		g_clients[new_id].m_z = 200.0f;

		number += 250.0f;

		//iocp객체에 소켓등록
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_sock), g_iocp->iocp, new_id, 0);



		g_iocp->do_recv(new_id);
	}

	closesocket(listen_sock);
	WSACleanup();



}
void Worker_thread() {

	while (1) {
		DWORD io_byte = 0;
		ULONGLONG i_key = 0;
		OVER_EX* over_ex;


		int is_error = GetQueuedCompletionStatus(g_iocp->iocp, &io_byte, &i_key,
			reinterpret_cast<LPWSAOVERLAPPED*>(&over_ex), INFINITE);

		char key = static_cast<char>(i_key);

		if (is_error == 0) {
			std::cout << "Worker_thread:: Error GQCS" << std::endl;
			ClearPlayerSocket(key);
		}

		if (io_byte == 0) {
			ClearPlayerSocket(key);
		}

		if (over_ex->ev == EV_RECV) {

			char* ptr = over_ex->buffer;
			int rest = io_byte;
			char packet_size = 0;

			if (0 < g_clients[key].m_prev_size)
				packet_size = g_clients[key].m_recv_packet_buf[0];

			// 패킷 조립
			while (0 < rest) {
				// 이전 기록이 없다면 패킷 사이즈를 ptr의 맨 처음으로 알 수 있다.
				if (0 == packet_size) packet_size = ptr[0];
				int required = packet_size - g_clients[key].m_prev_size;
				if (required <= rest) {

					memcpy(g_clients[key].m_recv_packet_buf + g_clients[key].m_prev_size, ptr, required);
					g_iocp->PacketProcess(key, g_clients[key].m_recv_packet_buf);

					rest -= required;
					ptr += required;

					packet_size = 0;
					g_clients[key].m_prev_size = 0;
				}
				else {
					memcpy(g_clients[key].m_recv_packet_buf + g_clients[key].m_prev_size, ptr, rest);
					rest = 0;
					g_clients[key].m_prev_size += rest;


				}

			}

			g_iocp->do_recv(key);


		}
		//send

		else if (over_ex->ev == EV_SEND) {
			delete over_ex;
		}

		else {
			std::cout << "not defined event_type to worker thread" << std::endl;
			while (1);
		}

	}
}
void Timer_thread() {

	while (1) {
		std::this_thread::sleep_for(10ms);
		while (1) {

			timer_l.lock();
			if (g_timer_queue.empty()) {
				timer_l.unlock();
				break;
			}
			auto ev = g_timer_queue.top();
			timer_l.unlock();

			if (ev.start_time > high_resolution_clock::now()) {
				break;
			}

			timer_l.lock();
			g_timer_queue.pop();
			timer_l.unlock();

			OVER_EX* over_ex = new OVER_EX;
			over_ex->id = ev.id;
			over_ex->ev = ev.type;

			PostQueuedCompletionStatus(g_iocp->iocp, 1, over_ex->id, &over_ex->overlapped);
		}
	}
}

void ClearPlayerSocket(const char id) {
	//삭제
	g_clients[id].player_l.lock();
	closesocket(g_clients[id].m_socket);
	g_clients[id].m_used = false;
	g_clients[id].m_mainLevel = false;
	g_clients[id].player_l.unlock();
	std::cout << "[" << (int)id << "] Close Socket" << std::endl;
}