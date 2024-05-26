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
bool escaped = false, attacked = false;			// ���� �� ����
int citizen, zombie, MDS = 0;		// ��ǥ�� ����
int move_cit, move_zom = 0;			// ���� ��ǥ�� ����
int turn = 2;					// ���� �� �޽� �� ����
int rollnum = 0;					// roll �����
int previous_citizen = 0;
int previous_zombie = 0;

void intro(void) {
	printf("==========�λ���==========\n����κ��� �ù��� ���Ѷ�!\n==========================\n\n");
	return;
}
void train_print(length, citizen, zombie, MDS) {
	for (int i = 1; i <= length; i++) {				// 1��
		printf("#");
	}
	printf("\n");

	printf("#");
	for (int i = 1; i <= citizen - 1; i++) {			// 2��
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

	for (int i = 1; i <= length; i++) {				// 3��
		printf("#");
	}
	printf("\n\n");
}
int train_length_scan(void) {
	printf("Train length (15~50) >> ");
	scanf_s("%d", &length);
	if (length > LEN_MAX || length < LEN_MIN) {
		error = true;
		printf("�߸��� ���� �Է��߽��ϴ�.\n���α׷��� �����մϴ�.\n");
	}
	if (error == true) {
		exit;
		return 0;
	}
}

int main(void) {


	// ��Ʈ��
	intro();

	// ���� ����, Ȯ�� �Է�
	train_length_scan();

	printf("\nȮ���� �����ּ���. �Է��� ���� ���� �������� ���̵��� �������ϴ�.\n");
	printf("Percentile probability 'p' (10~90) >> ");
	scanf_s("%d", &prob);
	if (prob > PROB_MAX || prob < PROB_MIN) {
		error = true;
		printf("�߸��� ���� �Է��߽��ϴ�.\n���α׷��� �����մϴ�.\n");
	}
	if (error == true) {
		exit;
		return 0;
	}

	// ������ �ʱ� ���� ���
	printf("\n\n");

	for (int i = 1; i <= length; i++) {				// 1��
		printf("#");
	}
	printf("\n");

	printf("#");
	for (int i = 1; i <= length - 7; i++) {			// 2��
		printf(" ");
	}
	printf("C");
	for (int i = 1; i <= 2; i++) {
		printf(" ");
	}
	printf("Z");
	printf("M");
	printf("#\n");

	for (int i = 1; i <= length; i++) {				// 3��
		printf("#");
	}
	printf("\n\n");
	printf("==== START! ====");
	printf("\n\n\n");

	Sleep(2000);


	citizen = length - 6;
	zombie = length - 3;
	MDS = length - 2;

	// �ݺ� ����
	while (1) {
		previous_citizen = citizen;
		previous_zombie = zombie;

		// �ù� �̵�
		rollnum = rand() % 100;

		if (rollnum < 100 - prob) {
			citizen = citizen - 1;
		}
		if (citizen == 1) {
			escaped = true;
		}

		// ���� �̵�
		if (turn % 2 == 0) {
			rollnum = rand() % 100;
			if (rollnum < prob) {
				zombie = zombie - 1;
			}
		}
		if (zombie == citizen + 1) {
			attacked = true;
		}

		// ���� ���
		for (int i = 1; i <= length; i++) {				// 1��
			printf("#");
		}
		printf("\n");

		printf("#");
		for (int i = 1; i <= citizen - 1; i++) {			// 2��
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

		for (int i = 1; i <= length; i++) {				// 3��
			printf("#");
		}
		printf("\n\n");


		// �ù�, ���� ���� ���
		// �ù�
		if (previous_citizen == citizen) {
			printf("citizen: stay %d\n", citizen);
		}
		else {
			printf("citizen: %d -> %d\n", previous_citizen, citizen);
		}

		// ����
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


		// Ż�� ���� �˻�
		// �ù��� Ż�⿡ �����ϰų� ���ݴ��ߴٸ� Ż��
		if ((escaped == true) || (attacked == true)) {
			break;
		}

		turn++;

		Sleep(4000);
	}

	if (escaped == true) {
		printf("\n==========================\n");
		printf("========Ż�� ����!========\n");
		printf("=== >> ���� ������! >> ===\n");
		printf("==========================\n");
	}
	if (attacked == true) {
		printf("\n==========================\n");
		printf("=========Ż�� ����========\n");
		printf("=���񿡰� ���ݴ��߽��ϴ�!=\n");
		printf("==========================\n");
	}


}
