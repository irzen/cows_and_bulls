// cows_bulls_console.cpp : Defines the entry point for the console application.
//

#include "cows_bulls.h"
#include <windows.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define KEY_BACK 8
#define KEY_ESC 27
#define KEY_ENTER 13

// the way to make life easer
// TODO: add varargs and pass them to printf
void print(COORD crd, const char *msg)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), crd);
	printf(msg);
}

// had been found somewhere in the internet
// stops and waits for user input
char readKey()
{
	DWORD        mode;
	HANDLE       hstdin;
	INPUT_RECORD inrec;
	DWORD        count;
	char         result = '\0';

	hstdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hstdin == INVALID_HANDLE_VALUE
		|| !GetConsoleMode(hstdin, &mode)
		|| !SetConsoleMode(hstdin, 0))
		return result;

	FlushConsoleInputBuffer(hstdin);

	// Wait for and get a single key PRESS 
	do ReadConsoleInput(hstdin, &inrec, 1, &count);
	while ((inrec.EventType != KEY_EVENT) || !inrec.Event.KeyEvent.bKeyDown);

	// Remember which key the user pressed
	result = inrec.Event.KeyEvent.uChar.AsciiChar;
	SetConsoleMode(hstdin, mode);

	return result;
}

// draw the field
// argument - width and height of the field
// resizes console window for no reason
// prints instructions how to interact
void drawTable(COORD size)
{
	DWORD cWritten;
	HANDLE con = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(con, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(con, &cursorInfo);
	
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 800, 480, TRUE);

	FillConsoleOutputCharacter(con, '+', 1, { 0, 3 }, &cWritten);
	FillConsoleOutputCharacter(con, '+', 1, { size.X, 3 }, &cWritten);
	FillConsoleOutputCharacter(con, '-', size.X - 1, { 1, 3 }, &cWritten);
	FillConsoleOutputCharacter(con, '_', 4, { 3, 1 }, &cWritten);

	for (int i = 0; i < size.Y; ++i)
	{
		if (i != 3)
		{
			FillConsoleOutputCharacter(con, '|', 1, { 0, i }, &cWritten);
			FillConsoleOutputCharacter(con, '|', 1, { size.X, i }, &cWritten);
		}
	}

	print({size.X + 2, 1}, "COWS AND BULLS");
	print({size.X + 2, 3}, "0-9 - add digit");
	print({size.X + 2, 4}, "Backspace - remove last digit");
	print({size.X + 2, 5}, "Enter - confirm");
	print({size.X + 2, 6}, "Esc - quit");
}

// clears screen to full black
// returns console cursor to top left corner
void clearScreen()
{
	DWORD cWritten;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 80 * 50, { 0, 0 }, &cWritten);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
}

int main()
{
	// not recommended to do something with sizes :(
	COORD size = {9, 8};
	drawTable(size);

	// task - aim, request - user input
	i4 task(generateTask());
	i4 request;

	// index to watch which digit user is entered
	// line - offset for the log output
	int index = 0, line = 4;
	
	bool bQuit = false;
	while (!bQuit)
	{
		char ch[] = {readKey(), '\0'};
		int code = (int)(ch[0]);

		// if input is digit
		if (code > 47 && code < 58 && index < 4)
		{
			int num = code - 48;
			bool bDublicate = false;
			for (int i = 0; i < index; ++i)
			{
				if (request[i] == num)
				{
					bDublicate = true;
					break;
				}
			}

			if (bDublicate)
				continue;

			request[index] = num;
			++index;
			print({2 + index, 1}, ch);
		}

		// if input is erase
		if (code == KEY_BACK && index > 0)
			print({2 + index--, 1}, "_");

		// if input is confirm
		if (code == KEY_ENTER && index == 4)
		{
			print({3, 1}, "____");
			i2 fb = getFeedback(request, task);

			// hard way to do simple thing
			char c[] = {
				request[0] + 48,
				request[1] + 48,
				request[2] + 48,
				request[3] + 48,
				' ',
				fb[0] + 48,
				':',
				fb[1] + 48,
				'\0'
			};
			print({1, line}, c);
			++line;
			index = 0;

			// make the table longer if needed
			if (line > size.Y-1)
			{
				print({0, size.Y}, "|");
				print({size.X, size.Y}, "|");
				++size.Y;
			}

			// win event
			if (fb[0] == 4)
			{
				for (int i = 0; i < size.X-1; ++i)
					print({1 + i, line}, "!");

				readKey();
				break;
			}
		}

		// if input is exit
		if (code == KEY_ESC)
			return 0;
		//printf("%d", code);
	}
	clearScreen();
	system("pause");
	return 0;
}