#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <Windows.h>

#define LEN_MIN 15
#define LEN_MAX 50
#define PROB_MIN 10
#define PROB_MAX 90

bool error = false;
int length, prob = 0;
bool escaped = false, attacked = false;			// 성공 값 변수
int citizen, zombie, MDS = 0;		// 좌표값 변수
int move_cit, move_zom = 0;			// 현재 좌표값 변수
int turn = 2;					// 좀비 턴 휴식 값 변수
int rollnum = 0;					// roll 저장용
int previous_citizen = 0;
int previous_zombie = 0;

void intro(void) {
	printf("==========부산헹==========\n좀비로부터 시민을 지켜라!\n==========================\n\n");
	return;
}
void train_print(length, citizen, zombie, MDS) {
	for (int i = 1; i <= length; i++) {				// 1행
		printf("#");
	}
	printf("\n");

	printf("#");
	for (int i = 1; i <= citizen - 1; i++) {			// 2행
		printf(" ");
	}
	printf("C");
	for (int i = 1; i <= zombie - citizen - 1; i++) {
		printf(" ");
	}
	printf("Z");
	for (int i = 1; i <= MDS - zombie - 1; i++) {
		printf(" ");
	}
	printf("M");
	printf("#\n");

	for (int i = 1; i <= length; i++) {				// 3행
		printf("#");
	}
	printf("\n\n");
}
int train_length_scan(void) {
	printf("Train length (15~50) >> ");
	scanf_s("%d", &length);
	if (length > LEN_MAX || length < LEN_MIN) {
		error = true;
		printf("잘못된 값을 입력했습니다.\n프로그램을 종료합니다.\n");
	}
	if (error == true) {
		exit;
		return 0;
	}
}

int main(void) {


	// 인트로
	intro();

	// 열차 길이, 확률 입력
	train_length_scan();

	printf("\n확률을 정해주세요. 입력한 숫자 값이 높을수록 난이도가 높아집니다.\n");
	printf("Percentile probability 'p' (10~90) >> ");
	scanf_s("%d", &prob);
	if (prob > PROB_MAX || prob < PROB_MIN) {
		error = true;
		printf("잘못된 값을 입력했습니다.\n프로그램을 종료합니다.\n");
	}
	if (error == true) {
		exit;
		return 0;
	}

	// 열차의 초기 상태 출력
	printf("\n\n");

	for (int i = 1; i <= length; i++) {				// 1행
		printf("#");
	}
	printf("\n");

	printf("#");
	for (int i = 1; i <= length - 7; i++) {			// 2행
		printf(" ");
	}
	printf("C");
	for (int i = 1; i <= 2; i++) {
		printf(" ");
	}
	printf("Z");
	printf("M");
	printf("#\n");

	for (int i = 1; i <= length; i++) {				// 3행
		printf("#");
	}
	printf("\n\n");
	printf("==== START! ====");
	printf("\n\n\n");

	Sleep(2000);


	citizen = length - 6;
	zombie = length - 3;
	MDS = length - 2;

	// 반복 시작
	while (1) {
		previous_citizen = citizen;
		previous_zombie = zombie;

		// 시민 이동
		rollnum = rand() % 100;

		if (rollnum < 100 - prob) {
			citizen = citizen - 1;
		}
		if (citizen == 1) {
			escaped = true;
		}

		// 좀비 이동
		if (turn % 2 == 0) {
			rollnum = rand() % 100;
			if (rollnum < prob) {
				zombie = zombie - 1;
			}
		}
		if (zombie == citizen + 1) {
			attacked = true;
		}

		// 열차 출력
		for (int i = 1; i <= length; i++) {				// 1행
			printf("#");
		}
		printf("\n");

		printf("#");
		for (int i = 1; i <= citizen - 1; i++) {			// 2행
			printf(" ");
		}
		printf("C");
		for (int i = 1; i <= zombie - citizen - 1; i++) {
			printf(" ");
		}
		printf("Z");
		for (int i = 1; i <= MDS - zombie - 1; i++) {
			printf(" ");
		}
		printf("M");
		printf("#\n");

		for (int i = 1; i <= length; i++) {				// 3행
			printf("#");
		}
		printf("\n\n");


		// 시민, 좀비 상태 출력
		// 시민
		if (previous_citizen == citizen) {
			printf("citizen: stay %d\n", citizen);
		}
		else {
			printf("citizen: %d -> %d\n", previous_citizen, citizen);
		}

		// 좀비
		if (turn % 2 != 0) {
			printf("zombie: stay %d (cannot move)\n\n\n\n", zombie);
		}
		else {
			if (previous_zombie == zombie) {
				printf("zombie: stay %d\n\n\n\n", zombie);
			}
			if (previous_zombie != zombie) {
				printf("zombie: %d -> %d\n\n\n\n", previous_zombie, zombie);
			}
		}


		// 탈출 조건 검사
		// 시민이 탈출에 성공하거나 공격당했다면 탈출
		if ((escaped == true) || (attacked == true)) {
			break;
		}

		turn++;

		Sleep(4000);
	}

	if (escaped == true) {
		printf("\n==========================\n");
		printf("========탈출 성공!========\n");
		printf("=== >> 다음 열차로! >> ===\n");
		printf("==========================\n");
	}
	if (attacked == true) {
		printf("\n==========================\n");
		printf("=========탈출 실패========\n");
		printf("=좀비에게 공격당했습니다!=\n");
		printf("==========================\n");
	}


}
