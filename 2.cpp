#include "pch.h"
#include <iostream>
using namespace std;

const int m = 5;//кол-во элементов в массиве

struct Date {
	int year;
	int month;
	int day;
	//12 byte
	Date(int year, int month, int day) {
		this->year = year;
		this->month = month;
		this->day = day;
	}
};

void sortDates(Date d[], int n) {

	__asm {
		mov		eax, d
		mov		edx, n

		xor		ebx, ebx
		xor		ecx, ecx
		inc		ecx

		CYCLE_1 :
		cmp		ebx, edx
			jge		FIN

		CYCLE_2 :
		cmp		ecx, ebx
			jge		NEXT

			push ecx
			push ebx

			imul ecx, 12
			imul ebx, 12

			mov esi, [eax][ebx + 4]
			cmp esi, [eax][ecx + 4]
			jg SWAP
			jl CONTINUE

			mov esi, [eax][ebx]
			cmp esi, [eax][ecx]
			jg SWAP
			jl CONTINUE

			mov esi, [eax][ebx + 8]
			cmp esi, [eax][ecx + 8]
			jg SWAP
			jl CONTINUE

			CONTINUE :
		pop ebx
			pop ecx
			inc ecx
			jmp CYCLE_2
		NEXT :
			xor ecx, ecx
			inc ebx
			jmp CYCLE_1

		SWAP :
			
			mov esi, [eax][ebx]
			mov edi, [eax][ecx]
			mov[eax][ecx], esi
			mov[eax][ebx], edi

			mov esi, [eax][ebx+4]
			mov edi, [eax][ecx+4]
			mov[eax][ecx+4], esi
			mov[eax][ebx+4], edi
				
			mov esi, [eax][ebx+8]
			mov edi, [eax][ecx+8]
			mov[eax][ecx+8], esi
			mov[eax][ebx+8], edi

			jmp CONTINUE

		FIN:
	}
}


void printDates(const Date d[], int n) {
	const char *x = "Year %d Month %d Day %d\n";
	
	__asm {

		xor		ebx, ebx

		CYCLE :
		mov		ecx, d
			mov		edx, n
			
			cmp		ebx, edx
			jge		FIN

			push ebx
			imul ebx, 12

			fild	dword ptr[ecx+ebx+8]
			sub		esp, 4
			fistp	dword ptr[esp]

			fild	dword ptr[ecx+ebx+4]
			sub		esp, 4
			fistp	dword ptr[esp]

			fild	dword ptr[ecx+ebx] 
			sub		esp, 4
			fistp	dword ptr[esp]

			mov eax, x
			push eax
			call printf
			add		esp, 16
			
			pop ebx
			inc ebx
			jmp CYCLE
		FIN:
	}
}

int main() {
	Date A(2010, 2, 6);
	Date B(2011, 2, 8);
	Date C(2011, 2, 2);
	Date D(2010, 1, 10);
	Date E(2017, 1, 6);
	Date d[] = { A,C,D,B,E };
	printDates(d, m);
	sortDates(d, m);
	printf("\n\n AFTER SORT \n\n");

	printDates(d, m);
	system("pause");
	return 0;
}
