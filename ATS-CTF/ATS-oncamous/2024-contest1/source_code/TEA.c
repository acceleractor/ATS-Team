#include <stdio.h>
#include <stdint.h>

void encrypt (uint32_t* v, uint32_t* k) {                       // 主要加密函数，试着搞定它
	uint32_t v0 = v[0], v1 = v[1], sum = 0;
	uint32_t delta = 0xd33b470;
	for (int i = 0; i < 32; i++) {
		sum += delta;
		v0 += ((v1<<4) + k[0]) ^ (v1 + sum) ^ ((v1>>5) + k[1]);
		v1 += ((v0<<4) + k[2]) ^ (v0 + sum) ^ ((v0>>5) + k[3]);
	}
	v[0] = v0;
	v[1] = v1;
}
void decrypt(uint32_t* v, uint32_t* k){
	
	uint32_t v0 = v[0], v1 = v[1], sum = 0;
	uint32_t delta = 0xd33b470;
	sum=delta*32;
	for (int i = 0; i < 32; i++) {
		v1 -= ((v0<<4) + k[2]) ^ (v0 + sum) ^ ((v0>>5) + k[3]);
		v0 -= ((v1<<4) + k[0]) ^ (v1 + sum) ^ ((v1>>5) + k[1]);
		sum -= delta;
	}
	v[0] = v0;
	v[1] = v1;
	
	
}
int main() {
	uint32_t k[4] = {1, 2, 3, 4};
	int8_t input[33] = {0};
	uint8_t chip[33]={0x2d, 0xd3, 0x22, 0xde, 0xf6, 0x99, 0x64, 0x3c, 0x5c, 0x12, 0x3c, 0x36, 0xac, 0x50, 0xed, 0x30, 0xea, 0xe, 0x94, 0xac, 0x7d, 0x87, 0xbe, 0x9f, 0x61, 0x9c, 0x8a, 0x8a, 0x5c, 0x48, 0x2d, 0x6e};	
//	scanf("%32s", input);
	for (int i = 0; i < 32; i+=8) {
		uint32_t v[2] = {*(uint32_t *)&chip[i], *(uint32_t *)&chip[i+4]};
		decrypt(v, k);
		for (int j = 0; j < 2; j++) {                           // 这一段主要是把 v 按单字节输出，另外可以了解一下 “大小端序” 在这题是如何体现的
			for (int k = 0; k < 4; k++) {
//				if((v[j] & 0xff) != chip[i+j*4+k]){
//					printf("please leave, you are not deserve a cup of a tea");
//					return 0;
//				}

				printf("%c",v[j]);	
//				printf("%x,",v[j] & 0xff);
//				printf("%x,",chip[i+j*4+k]);
//				printf("%d\n",chip[i+j*4+k]==(v[j] & 0xff));
				v[j] >>= 8;
			}
		}
	}
	
	printf("Good job, you got the tea!!!!");
	return 0;
}
