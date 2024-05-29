// READ ME!!
// 교수님께 : 커밋 및 푸시 사용법을 제대로 몰라 커밋 진행이 안된 터라 과정이 없습니다 ㅠㅠ.
// 부산헹 (1) 코드 자체에 수정을 해서 부산헹 (2)를 만들었습니다. 그래서 프로젝트 이름이 busanheg(1) 입니다...
// 부산헹 (1) 이 아닌 부산헹 (2) 가 맞습니다. 이름을 바꾸기에는 파일이 혹시나 날라갈까봐 이대로 진행합니다!

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <Windows.h>

// 기본 범위
#define LEN_MIN 15 //기차 길이
#define LEN_MAX 50
#define STM_MIN 0 //마동석 체력
#define STM_MAX 5
#define PROB_MIN 10 // 확률
#define PROB_MAX 90
#define AGGRO_MIN 0 //어그로 범위
#define AGGRO_MAX 5

// 마동석 이동 방향
#define MOVE_LEFT 1
#define MOVE_STAY 0

// 좀비의 공격 대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// 마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int length, prob, MDS_stamina = 0;
bool escaped = false, attacked_citizen = false;	attacked_MDS = false;		// 성공 값 변수
bool PULL_SUCCESS = false;				// 마동석 밀당 변수
int citizen, zombie = 0, MDS = 0;		// 좌표값 변수
int move_cit, move_zom, move_MDS = 0;			// 현재 좌표값 변수
int turn = 2;					// 좀비 턴 휴식 값 변수
int trun_print = 1;				// 출력용 턴 값
int rollnum = 0;					// roll 저장용
int previous_citizen = 0;			// 시민 이전 위치
int previous_zombie = 0;			// 좀비 이전 위치
int previous_MDS = 0;				// 마동석 이전 위치
int previous_MDS_stamina = 0;		// 마동석 이전 체력
int aggro_citizen, aggro_MDS = 0;	// 어그로 값
int previous_aggro_citizen, previous_aggro_MDS = 0;		// 어그로값 과거 값
int action_MDS = 0;		// 마동석 액션

void intro(void) {
	printf("==========부산헹==========\n좀비로부터 시민을 지켜라!\n==========================\n\n");
	return;
}
void outro(bool escaped, bool attacked_citizen, bool attacked_MDS) {
	if (escaped == true) {
		printf("\n==========================\n");
		printf("========탈출 성공!========\n");
		printf("=== >> 다음 열차로! >> ===\n");
		printf("==========================\n");
	}
	if (attacked_citizen == true) {
		printf("\n==========================\n");
		printf("=========탈출 실패========\n");
		printf("=좀비에게 공격당했습니다!=\n");
		printf("==========================\n");
	}
	if (attacked_MDS == true) {
		printf("\n==========================\n");
		printf("=========탈출 실패========\n");
		printf("==마동석이 쓰러졌습니다!==\n");
		printf("==========================\n");
	}
}

void train_print(length) {
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
}
void train_print_ing(length, citizen, zombie, MDS) {
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
	for (int i = 1; i <= length - MDS - 2; i++) {
		printf(" ");
	}
	printf("#\n");

	for (int i = 1; i <= length; i++) {				// 3행
		printf("#");
	}
	printf("\n\n");
}

int train_length_scan(void) {
	while (length > LEN_MAX || length < LEN_MIN) {
		printf("기차의 길이를 정해주세요.\n");
		printf("Train length (15~50) >> ");
		scanf_s("%d", &length);
	}
	return length;
}
int train_prob_scan(void) {
	while (prob > PROB_MAX || prob < PROB_MIN) {
		printf("확률을 정해주세요. 입력한 숫자 값이 높을수록 난이도가 높아집니다.\n");
		printf("Percentile probability 'p' (10~90) >> ");
		scanf_s("%d", &prob);
	}
	return prob;
}
int MDS_stamina_scan(void) {
	while (1) {
		printf("마동석의 체력을 입력해주세요. 좀비를 저지하는데 사용됩니다.\n");
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
	PULL_SUCCESS = false;	//초기화
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
	// 인트로
	intro();

	// 열차 길이, 마동석 체력, 확률 입력
	length = train_length_scan();
	MDS_stamina = MDS_stamina_scan();
	prob = train_prob_scan();

	// 열차의 초기 상태 출력
	train_print(length);

	// 시작 선
	printf("\n\n");
	printf("==== START! ====");
	printf("\n\n");

	Sleep(2000);

	// 위치 값 조정
	citizen = length - 6;
	zombie = length - 3;
	MDS = length - 2;

	// 반복 시작
	while (1) {
		// 과거 값 저장
		previous_citizen = citizen;
		previous_zombie = zombie;
		previous_aggro_citizen = aggro_citizen;
		previous_aggro_MDS = aggro_MDS;
		previous_MDS = MDS;
		previous_MDS_stamina = MDS_stamina;

		printf("<< TRUN : %d >>\n", trun_print);
		printf("<turn : 이동>\n");

		// 시민 이동
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
		// 탈출 조건 T / F 1
		escaped = flag_escaped_cit(citizen);

		// 좀비 이동
		if (turn % 2 == 0) {
			rollnum = rand() % 100;
			if (aggro_MDS > aggro_citizen) {
				if (rollnum < prob) {
					zombie++; // 오른쪽 이동
				}
			}
			else {
				if (rollnum < prob) {
					zombie--; // 왼쪽 이동
				}
			}
		}

		// 열차 출력
		train_print_ing(length, citizen, zombie, MDS);
		print_citizen(previous_citizen, citizen, previous_aggro_citizen, aggro_citizen);
		print_zombie(turn, previous_zombie, zombie);

		// 탈출 조건 검사
		// 시민이 탈출에 성공하거나 시민 또는 마동석이 공격당했다면 반복문 탈출
		if ((escaped == true)) {
			break;
		}

		// 마동석 이동
		MDS = MDS_move_scan(zombie, MDS);
		printf("\n");
		printf("<turn : 행동>\n");

		// 행동 턴 프린트
		train_print_ing(length, citizen, zombie, MDS);
		print_MDS(move_MDS, previous_MDS, MDS, previous_aggro_MDS, aggro_MDS);
		printf("\n");
		print_citizen2();
		MDS_stamina = print_zombie2(zombie, citizen, attacked_citizen, MDS, previous_MDS_stamina, MDS_stamina, aggro_citizen, aggro_MDS);
		// 탈출 조건 검사
		if (zombie == citizen + 1 && (zombie + 1) != MDS || zombie == citizen + 1 && (zombie + 1) == MDS && aggro_citizen > aggro_MDS) {
			attacked_citizen = true;
		}
		if (MDS_stamina <= STM_MIN) {
			attacked_MDS = true;
		}
		// 시민이 탈출에 성공하거나 시민 또는 마동석이 공격당했다면 반복문 탈출
		if ((attacked_citizen == true || attacked_MDS == true)) {
			break;
		}

		// 마동석 행동
		MDS_action_scan(zombie, MDS);
		print_action_MDS(action_MDS, previous_aggro_MDS, aggro_MDS, previous_MDS_stamina, MDS_stamina);


		// 턴 수 증가
		turn++;
		trun_print++;
	}
	outro(escaped, attacked_citizen, attacked_MDS);
}