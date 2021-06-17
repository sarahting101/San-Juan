#include <stdio.h>
#include <stdint.h>

int main(){
	uint8_t a = 14;
	char str[32] = "HELLO";
	uint8_t b = 4;
	char str2[32] = "A P P L E";
	a+=a/4;
	printf("%d",a);
	// printf("%8d: %-10s\t",a,str);
	// printf("%8d: %-10s\n",b,str2);
	// printf("%8d: %-10s\t",a,str2);
	// printf("%8d: %-10s\n",b,str2);
	// uint8_t x[2][3] = {{1},
	// 						{1},};
	// for(size_t i = 0 ; i < 2 ; i++){
	// 	for(size_t j = 0 ; j < 3 ; j++){
	// 		printf("%d\n",x[i][j]);
	// 	}
	// 	printf("\n");
	// }

	// uint8_t y = 123;
	// uint8_t z = 45;
	// printf("%04d\n",y);
	// printf("%04d\n",z);
	// char str[]="Hello";
	// char str2[]="A";
	// printf("%10s\n",str);
	// printf("%10s\n",str2);
	// uint8_t product[5][13]={0};
	// for(size_t i = 0 ; i < 5 ; i ++){
	// 	for(size_t j = 0 ; j < 13 ; j ++){
	// 		product[i][j]=30;
	// 		printf("%d, ",product[i][j]);
	// 	}
	// 	printf("\n");
	// }

	return 0;
}