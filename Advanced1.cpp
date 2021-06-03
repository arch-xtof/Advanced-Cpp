#include <iostream>
#include <fstream>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <list>
#include <condition_variable>
#include <string>
#include <thread>
#include <atomic>
#include <queue>
#include <sstream>
#include "Date.h"
#include "Item.h"
#include "Data.h"

using namespace std;

#define BUFSIZE 512
#define PIPE_TIMEOUT 30000

enum inputs { c, r, s, e };
map<char, string> input_map = { {'c', "connect"}, {'r', "ready"}, {'e', "exit"}, {'s', "stop"} };
bool svr_closed = true;
bool svr_err = false;
Data* ds = new Data();

class Read {
private:
	queue<inputs>& q;
	HANDLE& hPipe;
	HANDLE hExitEvent;
	HANDLE hHaveInput;
	mutex& mx;
	condition_variable& cv;
	unsigned long bytesRead = 0;
	char* reply = new char[BUFSIZE];

	void parseItem(string text) {
		map<string, int> months = { {"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4}, {"May", 5}, {"Jun", 6}, {"Jul", 7}, {"Aug", 8}, {"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12} };
		auto start = text.find('<');
		auto end = text.find('>');
		string name = text.substr(start + 1, end - start - 1);
		text = text.substr(0, start - 1) + text.substr(end + 1, text.length() - end - 1);

		char g;
		int sg, d, y;
		string m;

		stringstream ss(text);
		ss >> g >> sg >> d >> m >> y;
		Date date(d, months[m], y);
		ds->InsertItem(g, sg, name, date);
	}
public:
	Read(queue<inputs>& q, HANDLE& h1, mutex& mx, condition_variable& cv, HANDLE& h2, HANDLE& h3) : q(q), hPipe(h1), mx(mx), cv(cv), hHaveInput(h2), hExitEvent(h3) { }
	void operator() () {
		OVERLAPPED Overlapped;
		memset(&Overlapped, 0, sizeof Overlapped);
		Overlapped.hEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
		HANDLE hEvents[] = { Overlapped.hEvent, hExitEvent };
		bool NoData = true;
		bool exit = false;
		while (1) {
			if (svr_closed) {
				switch (WaitForSingleObject(hExitEvent, 1000)) {
				case WAIT_OBJECT_0:
					return;
				case WAIT_TIMEOUT:
					break;
				}
				continue;
			}
			if (!ReadFile(hPipe, reply, BUFSIZE, &bytesRead, &Overlapped))
			{
				int error = GetLastError();
				switch (error)
				{
				case ERROR_IO_PENDING:
					switch (WaitForMultipleObjects(2, hEvents, FALSE, PIPE_TIMEOUT))
					{
					case WAIT_OBJECT_0:
						GetOverlappedResult(hPipe, &Overlapped, &bytesRead, FALSE);
						NoData = false;
						break;
					case WAIT_OBJECT_0 + 1:
						cout << "Reading thread exits" << endl;
						exit = true;
						break;
					case WAIT_TIMEOUT:
						cout << "Timeout period of " << PIPE_TIMEOUT / 1000 << "seconds elapsed" << endl;
						exit = true;
						break;
					default:
						cout << "Reading error: " << GetLastError() << endl;
						exit = true;
						svr_err = true;
						break;
					}
					break;
				default:
					cout << "Reading error: " << GetLastError() << endl;
					exit = true;
					svr_err = true;
					break;
				}
			}
			else {
				NoData = false;
			}
			if (exit) {
				break;
			}
			if (!NoData) {
				parseItem(reply);

				unique_lock<mutex> lock(mx);
				q.push(r);
				lock.unlock();
				cv.notify_one();
				SetEvent(hHaveInput);
			}
		}
		CloseHandle(Overlapped.hEvent);
		delete reply;
	}
};

class Listen {
private:
	queue<inputs>& q;
	HANDLE& hPipe;
	HANDLE& hHaveInput;
	HANDLE& hExitEvent;
	mutex& mx;
	condition_variable& cv;
public:
	Listen(queue<inputs>& q, HANDLE& h1, mutex& m, condition_variable& c, HANDLE& h2, HANDLE& h3) : q(q), mx(m), cv(c), hPipe(h1), hHaveInput(h2), hExitEvent(h3) {};
	void operator() () {
		inputs i = c;
		string tmp = "";
		while (1) {
			if (svr_err) {
				cout << "Server error has occured, type \"exit\" to exit" << endl;
			}
			cout << "(connect/stop/exit): ";
			cin >> tmp;
			if (tmp == input_map['c']) {
				if (!svr_closed) {
					cout << "Client is already open" << endl;
				}
				i = c;
			}
			else if (tmp == input_map['s']) {
				if (svr_closed) {
					cout << "Client is already stopped" << endl;
				}
				i = s;
			}
			else if (tmp == input_map['e']) {
				i = e;
				SetEvent(hExitEvent);
				return;
			}
			else {
				cout << "Incorrect input" << endl;
				continue;
			}
			unique_lock<mutex> lock(mx);
			q.push(i);
			lock.unlock();
			cv.notify_one();
			SetEvent(hHaveInput);
		}
	}
};

class Write {
private:
	queue<inputs>& q;
	HANDLE& hPipe;
	HANDLE& hHaveInput;
	HANDLE& hExitEvent;
	mutex& mx;
	condition_variable& cv;
	unsigned long bytesWritten = 0;
public:
	Write(queue<inputs>& q, HANDLE& h1, mutex& m, condition_variable& c, HANDLE& h2, HANDLE& h3) : q(q), hPipe(h1), mx(m), cv(c), hHaveInput(h2), hExitEvent(h3) { };
	void operator() () {
		HANDLE hEvents[] = { hHaveInput, hExitEvent };
		bool NoData = true;
		bool exit = false;
		const char* input = nullptr;
		while (1) {
			switch (WaitForMultipleObjects(2, hEvents, FALSE, PIPE_TIMEOUT)) {
			case WAIT_OBJECT_0:
				NoData = false;
				break;
			case WAIT_OBJECT_0 + 1:
				cout << endl << "Writing thread exits" << endl;
				exit = true;
				break;
			case WAIT_TIMEOUT:
				cout << "Timeout period " << PIPE_TIMEOUT << "ms elapsed, nothing was received." << endl;
				exit = true;
				break;
			default:
				cout << "Writing error: " << GetLastError() << endl;
				svr_err = true;
				exit = true;
				break;
			}
			if (exit) {
				if (!svr_closed) {
					CloseHandle(hPipe);
				}
				break;
			}
			if (!NoData) {
				unique_lock<mutex> lock(mx);
				inputs i = q.front();
				switch (i) {
				case r:
					input = input_map['r'].c_str();
					break;
				case c:
					if (svr_closed) {
						hPipe = CreateFileA(
							"\\\\.\\pipe\\ICS0025",		// pipe name 
							GENERIC_READ |				// read/write access 
							GENERIC_WRITE,
							0,							// sharing 
							NULL,						// security attributes
							CREATE_ALWAYS,				// open state 
							FILE_FLAG_OVERLAPPED,		// asynchronous read/write
							NULL);						// template file
						if (hPipe == INVALID_HANDLE_VALUE) {
							cout << "Error while creating file: " << GetLastError() << endl;
							return;
						}
						svr_closed = false;
					}
					input = input_map['r'].c_str();
					break;
				case s:
					input = input_map['s'].c_str();
					svr_closed = true;
					break;
				}
				if (!svr_closed || input == input_map['s']) {
					if (!WriteFile(hPipe, input, strlen(input) + 1, &bytesWritten, NULL))
					{
						cout << "Error while writing into file: " << GetLastError() << endl;
						svr_err = true;
						return;
					}
				}
				q.pop();
				lock.unlock();
			}
		}
	}

};

int main() {
	HANDLE hPipe;
	queue<inputs> q;
	mutex mx;
	condition_variable cv;
	HANDLE hExitEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
	HANDLE hInputEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
	thread tRead{ Read(ref(q), ref(hPipe),  mx, cv, hInputEvent, hExitEvent) };
	thread tWrite{ Write(ref(q), ref(hPipe), mx, cv, hInputEvent, hExitEvent) };
	thread tListen{ Listen(ref(q), ref(hPipe), mx, cv, hInputEvent, hExitEvent) };
	tRead.join();
	tWrite.join();
	tListen.join();
	CloseHandle(hExitEvent);
	CloseHandle(hInputEvent);
	cout << ds->CountItems() << " items were written:\n";
	ds->PrintAll();
	return 0;

}