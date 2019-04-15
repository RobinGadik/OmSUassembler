#include "pch.h"
#include <iostream>
using namespace std;

const int m = 5;//кол-во элементов в массиве

struct Plane {
	//Рассматриваем плоскость как коэффиценты, поэтому пропустим смещение
	double x;
	double y;
	double z;

	Plane(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

double planesAreParallel(Plane A, Plane B) {
	double res=0;
	double	A_x, A_y, A_z;
	double a, b;
	_asm {
		finit

			lea			eax, A
			lea			ebx, B

			xor			esi, esi
			xor			edi, edi

		VECTORS_COORDINATES_SEARCH :
			cmp			esi, 3
			je			CHECK

			fld			qword ptr[ebx][esi * 8]
			fld			qword ptr[eax][esi * 8]
			fdivp		st(1), st(0)

			inc			esi
			cmp			edi, 0

			je			X_COORDINATES
			cmp			edi, 1
			je			Y_COORDINATES
			cmp			edi, 2
			je			Z_COORDINATES

		X_COORDINATES :
			fstp		qword ptr A_x
			inc			edi
			jmp			VECTORS_COORDINATES_SEARCH
		Y_COORDINATES :
			fstp		qword ptr A_y
			inc			edi
			jmp			VECTORS_COORDINATES_SEARCH
		Z_COORDINATES :
			fstp		qword ptr A_z
			inc			edi
			jmp			VECTORS_COORDINATES_SEARCH
		CHECK:
			fld			qword ptr A_x
			fcom		qword ptr A_y
			fstsw       ax
			sahf
			jne FIN
			fcom		qword ptr A_z
			fstsw       ax
			sahf
			jne FIN
		YES:
			fld1
			fstp qword ptr res

		FIN:

	}
	return res;
}

void printPlanes(Plane A, Plane B) {
	const char *x = "(%0.3f;%0.3f;%0.3f)\n";
	const char *f = "%s\n";
	const char* delimiter = "____________________";


	__asm {

		xor		esi, esi
		lea		eax, A

		CYCLE :

			fld		qword ptr[eax + 16]
			sub		esp, 8
			fstp	qword ptr[esp]

			fld		qword ptr[eax + 8]
			sub		esp, 8
			fstp	qword ptr[esp]

			fld		qword ptr[eax]
			sub		esp, 8
			fstp	qword ptr[esp]

			mov		eax, x
			push	eax
			call	printf
			add		esp, 28

			cmp		esi, 0
			je		NEXT_B
			cmp		esi, 1
			je		FIN


		NEXT_B :
			inc		esi
			lea		eax, B
			jmp		CYCLE
		FIN :
			mov		eax, delimiter
			push	eax
			mov		eax, f
			push	eax
			call	printf
			add		esp, 8
	}
}

void TwoPlanes(Plane planes[], int n) {
	_asm {

		lea		eax, planes
		mov		edx, n

		xor		ebx, ebx
		mov		ecx, ebx
		inc		ecx

		CYCLE_1 :
			cmp		ebx, edx
			je		FIN

		CYCLE_2 :
			cmp		ecx, edx
			je		NEXT

		
			push	ebx
			push	ecx
			
			imul	ebx, 24
			imul	ecx, 24

			mov		eax, planes
			add		eax, ecx

			fld		qword ptr[eax + 16]
			sub		esp, 8
			fstp	qword ptr[esp]
			fld		qword ptr[eax + 8]
			sub		esp, 8
			fstp	qword ptr[esp]
			fld		qword ptr[eax]
			sub		esp, 8
			fstp	qword ptr[esp]

			mov		eax, planes
			add		eax, ebx

			fld		qword ptr[eax + 16]
			sub		esp, 8
			fstp	qword ptr[esp]
			fld		qword ptr[eax + 8]
			sub		esp, 8
			fstp	qword ptr[esp]
			fld		qword ptr[eax]
			sub		esp, 8
			fstp	qword ptr[esp]

			call	planesAreParallel
			add		esp, 48

			pop		ecx
			pop		ebx

			ftst
			fstsw	 ax
			sahf
			jnz		IS_LIE

			mov		edx, n
			inc		ecx

			jmp CYCLE_2

			NEXT :
			inc		ebx
			mov		ecx, ebx
			inc		ecx
			jmp		CYCLE_1

			IS_LIE :
			push	ebx
			push	ecx

			imul	ebx, 24
			imul	ecx, 24

			mov		eax, planes
			add		eax, ecx

			fld		qword ptr[eax + 16]
			sub		esp, 8
			fstp	qword ptr[esp]
			fld		qword ptr[eax + 8]
			sub		esp, 8
			fstp	qword ptr[esp]
			fld		qword ptr[eax]
			sub		esp, 8
			fstp	qword ptr[esp]

			mov		eax, planes
			add		eax, ebx

			fld		qword ptr[eax + 16]
			sub		esp, 8
			fstp	qword ptr[esp]
			fld		qword ptr[eax + 8]
			sub		esp, 8
			fstp	qword ptr[esp]
			fld		qword ptr[eax]
			sub		esp, 8
			fstp	qword ptr[esp]

			call	printPlanes
			add		esp, 48

			pop		ecx
			pop		ebx
			mov		edx, n
			inc ecx

			jmp CYCLE_2
			FIN :
	}
}

int main() {
	Plane A(2, 2, 6);
	Plane B(2, 2, 6);
	Plane C(2, 1, 3);
	Plane D(4, 2, 6);
	Plane E(8, 4, 12);
	Plane points[] = {A,B,C,D,E};
	TwoPlanes(points,m);
	system("pause");
	return 0;
}

