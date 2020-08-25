#define KEY_VALUE (key_t)1234
#define MAX_TEXT 512

struct my_msg_st{
	//메시지큐의 구조체에서 첫번째 데이터 타입 ling int는 반드시 필요
	long int my_msg_type;
	char some_text[BUFSIZ];
};
