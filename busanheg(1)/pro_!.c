// READ ME!!
// �����Բ� : Ŀ�� �� Ǫ�� ������ ����� ���� Ŀ�� ������ �ȵ� �Ͷ� ������ �����ϴ� �Ф�.
// �λ��� (1) �ڵ� ��ü�� ������ �ؼ� �λ��� (2)�� ��������ϴ�. �׷��� ������Ʈ �̸��� busanheg(1) �Դϴ�...
// �λ��� (1) �� �ƴ� �λ��� (2) �� �½��ϴ�. �̸��� �ٲٱ⿡�� ������ Ȥ�ó� ���󰥱�� �̴�� �����մϴ�!

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <Windows.h>

// �⺻ ����
#define LEN_MIN 15 //���� ����
#define LEN_MAX 50
#define STM_MIN 0 //������ ü��
#define STM_MAX 5
#define PROB_MIN 10 // Ȯ��
#define PROB_MAX 90
#define AGGRO_MIN 0 //��׷� ����
#define AGGRO_MAX 5

// ������ �̵� ����
#define MOVE_LEFT 1
#define MOVE_STAY 0

// ������ ���� ���
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// ������ �ൿ
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int length, prob, MDS_stamina = 0;
bool escaped = false, attacked_citizen = false;	attacked_MDS = false;		// ���� �� ����
bool PULL_SUCCESS = false;				// ������ �д� ����
int citizen, zombie = 0, MDS = 0;		// ��ǥ�� ����
int move_cit, move_zom, move_MDS = 0;			// ���� ��ǥ�� ����
int turn = 2;					// ���� �� �޽� �� ����
int trun_print = 1;				// ��¿� �� ��
int rollnum = 0;					// roll �����
int previous_citizen = 0;			// �ù� ���� ��ġ
int previous_zombie = 0;			// ���� ���� ��ġ
int previous_MDS = 0;				// ������ ���� ��ġ
int previous_MDS_stamina = 0;		// ������ ���� ü��
int aggro_citizen, aggro_MDS = 0;	// ��׷� ��
int previous_aggro_citizen, previous_aggro_MDS = 0;		// ��׷ΰ� ���� ��
int action_MDS = 0;		// ������ �׼�

void intro(void) {
	printf("==========�λ���==========\n����κ��� �ù��� ���Ѷ�!\n==========================\n\n");
	return;
}
void outro(bool escaped, bool attacked_citizen, bool attacked_MDS) {
	if (escaped == true) {
		printf("\n==========================\n");
		printf("========Ż�� ����!========\n");
		printf("=== >> ���� ������! >> ===\n");
		printf("==========================\n");
	}
	if (attacked_citizen == true) {
		printf("\n==========================\n");
		printf("=========Ż�� ����========\n");
		printf("=���񿡰� ���ݴ��߽��ϴ�!=\n");
		printf("==========================\n");
	}
	if (attacked_MDS == true) {
		printf("\n==========================\n");
		printf("=========Ż�� ����========\n");
		printf("==�������� ���������ϴ�!==\n");
		printf("==========================\n");
	}
}

void train_print(length) {
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
}
void train_print_ing(length, citizen, zombie, MDS) {
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
	for (int i = 1; i <= length - MDS - 2; i++) {
		printf(" ");
	}
	printf("#\n");

	for (int i = 1; i <= length; i++) {				// 3��
		printf("#");
	}
	printf("\n\n");
}

int train_length_scan(void) {
	while (length > LEN_MAX || length < LEN_MIN) {
		printf("������ ���̸� �����ּ���.\n");
		printf("Train length (15~50) >> ");
		scanf_s("%d", &length);
	}
	return length;
}
int train_prob_scan(void) {
	while (prob > PROB_MAX || prob < PROB_MIN) {
		printf("Ȯ���� �����ּ���. �Է��� ���� ���� �������� ���̵��� �������ϴ�.\n");
		printf("Percentile probability 'p' (10~90) >> ");
		scanf_s("%d", &prob);
	}
	return prob;
}
int MDS_stamina_scan(void) {
	while (1) {
		printf("�������� ü���� �Է����ּ���. ���� �����ϴµ� ���˴ϴ�.\n");
		printf("madongseokstamina (0~5) >> ");
		scanf_s("%d", &MDS_stamina);
		if (MDS_stamina >= STM_MIN && MDS_stamina <= STM_MAX) {
			break;
		}
		else {
			continue;
		}
	}
	return MDS_stamina;
}
int MDS_move_scan(zombie, MDS) {
	if (zombie == (MDS - 1)) {
		while (1) {
			printf("\nmadongseok move(0:stay) >> ");
			scanf_s("%d", &move_MDS);
			if (move_MDS == MOVE_STAY) {
				break;
			}
		}
	}
	else {
		while (1) {
			printf("\nmadongseok move(0:stay, 1:left) >> ");
			scanf_s("%d", &move_MDS);
			if (move_MDS == MOVE_LEFT) {
				MDS--;
			}
			if (move_MDS == MOVE_STAY || move_MDS == MOVE_LEFT) {
				break;
			}
		}
	}
	return MDS;
}
int MDS_action_scan(zombie, MDS) {
	while (1) {
		if (MDS == (zombie + 1)) {
			printf("madongseok action (0.rest, 1.provoke, 2.pull) >> ");
		}
		else {
			printf("madongseok action (0.rest, 1.provoke) >> ");
		}
		scanf_s("%d", &action_MDS);
		if (action_MDS == ACTION_PROVOKE || action_MDS == ACTION_PULL || action_MDS == ACTION_REST) {
			break;
		}
	}
	return action_MDS;
}

bool flag_escaped_cit(citizen) {
	if (citizen == 1) {
		escaped = true;
	}
	return escaped;
}

void print_citizen(previous_citizen, citizen, previous_aggro_citizen, aggro_citizen) {
	if (previous_citizen == citizen) {
		printf("citizen: stay %d (aggro : %d -> %d)\n", citizen, previous_aggro_citizen, aggro_citizen);
	}
	else {
		printf("citizen: %d -> %d (aggro : %d -> %d)\n", previous_citizen, citizen, previous_aggro_citizen, aggro_citizen);
	}
}
void print_zombie(turn, previous_zombie, zombie) {
	if (turn % 2 != 0) {
		printf("zombie: stay %d (cannot move)\n", zombie);
	}
	else if (PULL_SUCCESS == true && turn % 2 == 0) {
		printf("zombie: stay %d (madongseok pulled)\n", zombie);
	}
	else {
		if (previous_zombie == zombie) {
			printf("zombie: stay %d\n", zombie);
		}
		if (previous_zombie != zombie) {
			printf("zombie: %d -> %d\n", previous_zombie, zombie);
		}
	}
}
int print_MDS(move_MDS, previous_MDS, MDS, previous_aggro_MDS, aggro_MDS) {
	if (move_MDS == MOVE_LEFT) {
		if (aggro_MDS < AGGRO_MAX) {
			aggro_MDS++;
		}
		printf("madongseok: stay %d (aggro : %d -> %d)\n", MDS, previous_aggro_MDS, aggro_MDS);
	}
	else if (move_MDS == MOVE_STAY) {
		if (aggro_MDS > AGGRO_MIN) {
			aggro_MDS--;
		}
		printf("madongseok: %d -> %d (aggro : %d -> %d)\n", previous_MDS, MDS, previous_aggro_MDS, aggro_MDS);
	}
	return aggro_MDS;
}
int print_action_MDS(action_MDS, previous_aggro_MDS, aggro_MDS, previous_MDS_stamina, MDS_stamina) {
	printf("\n");
	PULL_SUCCESS = false;	//�ʱ�ȭ
	if (action_MDS == ACTION_PROVOKE) {
		aggro_MDS = AGGRO_MAX;
		printf("madongseok provoked zombie!\n");
		printf("madongseok: (aggro: %d -> %d)\n", previous_aggro_MDS, aggro_MDS);
	}
	else if (action_MDS == ACTION_PULL) {
		rollnum = rand() % 100;
		MDS_stamina--;
		aggro_MDS += aggro_MDS + 2;
		if (aggro_MDS > AGGRO_MAX) {
			while (1) {
				if (aggro_MDS == AGGRO_MAX) {
					break;
				}
				aggro_MDS--;
			}
		}
		if (rollnum < 100 - prob) {
			printf("madongseok pulled zombie. Next turn, it can't move!\n");
			PULL_SUCCESS = true;
		}
		else {
			printf("madongseok failed to pull zombie.\n");
		}
	}
	else {
		if (MDS_stamina < STM_MAX) {
			MDS_stamina++;
		}
		if (aggro_MDS > AGGRO_MIN) {
			aggro_MDS--;
		}
		printf("madongseok rests...\n");
	}
	printf("madongseok: (aggro: %d -> %d, stamina: %d -> %d)\n\n", previous_aggro_MDS, aggro_MDS, previous_MDS_stamina, MDS_stamina);
	return MDS_stamina, aggro_MDS;
}

void print_citizen2(void) {
	printf("Citizen does nothing.\n");
}
int print_zombie2(zombie, citizen, attacked_citizen, MDS, previous_MDS_stamina, MDS_stamina, aggro_citizen, aggro_MDS) {
	if (zombie == citizen + 1 && (zombie + 1) != MDS) {
		printf("Zomibe attacked citizen!\n");
	}
	else if (zombie+1 == MDS) {
		MDS_stamina--;
		printf("Zomibe attacked madongseok (madongseok stamina: %d -> %d)\n", previous_MDS_stamina, MDS_stamina);
	}
	else if ((zombie-1) == citizen && (zombie + 1) == MDS) {
		if (aggro_citizen < aggro_MDS) {
			MDS_stamina--;
			printf("Zomibe attacked madongseok (aggro: %d vs. %d, madongseok stamina: %d -> %d)\n", aggro_citizen, aggro_MDS, previous_MDS_stamina, MDS_stamina);
		}
		else if (aggro_citizen > aggro_MDS) {
			printf("Zomibe attacked citizen! (aggro: %d vs. %d)\n", aggro_citizen, aggro_MDS);
		}
	}
	else {
		printf("zombie attacked nobody.\n");
	}
	return MDS_stamina;
}

int main(void) {
	srand((unsigned int)time(NULL));
	// ��Ʈ��
	intro();

	// ���� ����, ������ ü��, Ȯ�� �Է�
	length = train_length_scan();
	MDS_stamina = MDS_stamina_scan();
	prob = train_prob_scan();

	// ������ �ʱ� ���� ���
	train_print(length);

	// ���� ��
	printf("\n\n");
	printf("==== START! ====");
	printf("\n\n");

	Sleep(2000);

	// ��ġ �� ����
	citizen = length - 6;
	zombie = length - 3;
	MDS = length - 2;

	// �ݺ� ����
	while (1) {
		// ���� �� ����
		previous_citizen = citizen;
		previous_zombie = zombie;
		previous_aggro_citizen = aggro_citizen;
		previous_aggro_MDS = aggro_MDS;
		previous_MDS = MDS;
		previous_MDS_stamina = MDS_stamina;

		printf("<< TRUN : %d >>\n", trun_print);
		printf("<turn : �̵�>\n");

		// �ù� �̵�
		rollnum = rand() % 100;
		if (rollnum < 100 - prob) {
			citizen--;
			if (aggro_citizen < AGGRO_MAX) {
				aggro_citizen++;
			}
		}
		else {
			if (aggro_citizen > AGGRO_MIN) {
				aggro_citizen--;
			}
		}
		// Ż�� ���� T / F 1
		escaped = flag_escaped_cit(citizen);

		// ���� �̵�
		if (turn % 2 == 0) {
			rollnum = rand() % 100;
			if (aggro_MDS > aggro_citizen) {
				if (rollnum < prob) {
					zombie++; // ������ �̵�
				}
			}
			else {
				if (rollnum < prob) {
					zombie--; // ���� �̵�
				}
			}
		}

		// ���� ���
		train_print_ing(length, citizen, zombie, MDS);
		print_citizen(previous_citizen, citizen, previous_aggro_citizen, aggro_citizen);
		print_zombie(turn, previous_zombie, zombie);

		// Ż�� ���� �˻�
		// �ù��� Ż�⿡ �����ϰų� �ù� �Ǵ� �������� ���ݴ��ߴٸ� �ݺ��� Ż��
		if ((escaped == true)) {
			break;
		}

		// ������ �̵�
		MDS = MDS_move_scan(zombie, MDS);
		printf("\n");
		printf("<turn : �ൿ>\n");

		// �ൿ �� ����Ʈ
		train_print_ing(length, citizen, zombie, MDS);
		print_MDS(move_MDS, previous_MDS, MDS, previous_aggro_MDS, aggro_MDS);
		printf("\n");
		print_citizen2();
		MDS_stamina = print_zombie2(zombie, citizen, attacked_citizen, MDS, previous_MDS_stamina, MDS_stamina, aggro_citizen, aggro_MDS);
		// Ż�� ���� �˻�
		if (zombie == citizen + 1 && (zombie + 1) != MDS || zombie == citizen + 1 && (zombie + 1) == MDS && aggro_citizen > aggro_MDS) {
			attacked_citizen = true;
		}
		if (MDS_stamina <= STM_MIN) {
			attacked_MDS = true;
		}
		// �ù��� Ż�⿡ �����ϰų� �ù� �Ǵ� �������� ���ݴ��ߴٸ� �ݺ��� Ż��
		if ((attacked_citizen == true || attacked_MDS == true)) {
			break;
		}

		// ������ �ൿ
		MDS_action_scan(zombie, MDS);
		print_action_MDS(action_MDS, previous_aggro_MDS, aggro_MDS, previous_MDS_stamina, MDS_stamina);


		// �� �� ����
		turn++;
		trun_print++;
	}
	outro(escaped, attacked_citizen, attacked_MDS);
}