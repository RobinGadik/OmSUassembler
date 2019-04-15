#include "pch.h"
#include <iostream>
using namespace std;

// Remove first entry of substring

char* scanStr() {
	char* str;
	const char *f = " %[^\t\n]";
	_asm {
		mov		eax, 256
		push	eax
		call	malloc
		add		esp, 4

		cmp		eax, 0
		je		NO_MEMORY

		mov		str, eax
		push	eax
		mov		ebx, f
		push	ebx
		call	scanf
		add		esp, 8

		NO_MEMORY:
	}
	return str;
}

void printStr(char* s) {
	const char *f = "%s\n";
	__asm {
		mov		eax, s
		push	eax
		mov		ebx, f
		push	ebx
		call	printf
		add		esp, 8
	}
}

char* removeFirstEntry(char* str1, char* str2) {
	char* str3;
	_asm {
		mov		eax, 256
		push	eax
		call	malloc
		add		esp, 4
		cmp		eax, 0
		je		NO_MEMORY
		mov		str3, eax

		mov		esi, str1
		mov		edi, str2
		mov		edx, edi


		CYCLE1 :
		cmp		byte ptr[esi], 0
			je		FIN

			mov		ch, [edi]
			cmp		ch, 0
			je		SAVE

			cmp[esi], ch
			jne		SAVE_AND_NEXT
			jmp		NEXT_NEXT

			SAVE :
		mov		ch, [esi]
			mov[eax], ch
			inc		eax
			inc		esi
			jmp		CYCLE1
			NEXT_NEXT :
		inc		esi
			inc		edi
			jmp		CYCLE1
			SAVE_AND_NEXT :
		mov		ch, [esi]
			mov[eax], ch
			inc		eax
			inc		esi
			inc		ecx
			mov		edi, edx
			jmp		CYCLE1
			FIN :
		mov		byte ptr[eax], 0
			NO_MEMORY :
	}
	return str3;

}


int main() {
	char* str1 = scanStr();
	char* str2 = scanStr();
	char* str3 = removeFirstEntry(str1, str2);

	printStr(str3);

	system("pause");
	return 0;
}
