#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define NONE "\033[m"
#define GRAY "\033[1;30m"

//29 types of cards
char name[][32]={"Smithy","Poor house","Black market","Crane","Carpenter","Quarry","Indigo plant","Sugar mill","Tobacco storage","Coffee roaster","Silver smelter","Well","Aqueduct","Market stand","Market hall","Trading post","Archive","Prefecture","Gold mine","Tower","Chapel","Library","Statue","Victory column","Hero","Guild hall","City hall","Triumhal arch","Palace"};

uint8_t cost[]={1,2,2,2,3,4,1,2,3,4,5,2,3,2,4,2,1,3,1,3,3,5,3,4,5,6,6,6,6};

uint8_t phase[]={1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,4,4,5,6,6,7,8,8,8,9,9,9,9};
/*
	1: builder phase
	2: producer phase
	3: trader phase
	4: councilor phase
	5: prospector phase
	6: during game
	7: library
	8: monument
	9: after finish
*/
uint8_t point[]={1,1,1,1,2,2,1,1,2,1,3,1,2,1,2,1,1,2,1,2,2,3,3,4,5,0,0,0,0};
uint8_t piece[]={3,3,3,3,3,3,6,8,8,8,8,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2};
uint8_t sum = 106;
//discard
uint8_t discard[111] = {0};
uint8_t discard_sum = 0;

//player
char player_name[5][32]={"Player","Com1","Com2","Com3"};
uint8_t player_cardnum[5][13] = {{0},{0}};
uint8_t player_sum[5] = {0,0,0,0};
uint8_t player_vps[5] = {0,0,0,0};
// player built
uint8_t built[5][13] = {{0},{0}};
uint8_t built_sum[5] = {1,1,1,1};

void safe_flush(FILE *fp){
 int ch;
 while( (ch = fgetc(fp)) != EOF && ch != '\n' );     
}

uint8_t setCardNum(){
	int8_t x = rand()%sum+1;
	uint8_t i = 0;
	// printf("x: %d\n",x);
	while(x>0){
		// printf("i: %d,",i);
		x = x - piece[i];
		// printf("x: %d\n",x);
		i++; 
	
	}
	// printf("i: %d\n",i);
	piece[i-1]--;
	// printf("piece[i]: %d\n",piece[i-1]);
	// num = i-1;
	sum--;
	// printf("sum: %d\n",sum);
	return i-1;
}

void comBuilt(uint8_t playernum, uint8_t governor){
	uint8_t x =  rand()%2;
	int8_t cost_card = 0;
	if( x == 0){
		printf("%s builds nothing.\n",player_name[playernum]);
		return;
	}
	else{
		uint8_t tobuild = 0;
		for(size_t i = 0 ; i<player_sum[playernum] ; i++){
			tobuild = player_cardnum[playernum][i]; 
			if(playernum==governor){
				cost_card-=1;
			}
			for(size_t j = 0 ; j<built_sum[playernum] ; j++){
				//Smithy
				if(built[playernum][j]==0 && tobuild>=6 && tobuild<=10){
					cost_card-=1;
				}
				//Quarry
				if(built[playernum][j]==5 && tobuild<6 && tobuild>10){
					cost_card-=1;
				}
				//Carpenter
				if(built[playernum][j]==5 && tobuild<6 && tobuild>10){
					player_cardnum[playernum][player_sum[playernum]] = setCardNum();
					player_sum[playernum]++;
				}
			}
			//blackmarket
			//crane
			cost_card +=cost[tobuild];
			//cost<=0
			if(cost_card<=0){
				player_cardnum[playernum][i]=0;
				for(size_t k = i+1 ; k<player_sum[playernum] ; k++){
					player_cardnum[playernum][k-1] = player_cardnum[playernum][k];
				}
				player_sum[playernum]--;
				built[playernum][built_sum[playernum]] = tobuild;
				built_sum[playernum]++;
				break;
			}
			if(cost_card<=player_sum[playernum]){
				player_cardnum[playernum][i]=0;
				player_sum[playernum]--;
				built[playernum][built_sum[playernum]] = tobuild;
				built_sum[playernum]++;
				uint8_t topay = 0;
				for(int8_t j = player_sum[playernum] ; j >= 0 ; j--){
					topay = player_cardnum[playernum][j];
					if(j!=i){
						discard[discard_sum] = topay;
						discard_sum++;
						player_cardnum[playernum][j] = 0;
						cost_card--;
					}
					if(cost_card==0){
						break;
					}
				}
				
				player_sum[playernum] -= cost_card; 
				//Poor house
				if(player_sum[playernum]<=1){
					player_cardnum[playernum][player_sum[playernum]] = setCardNum();
					player_sum[playernum]++;
				}
			}

		}
	}
	return;
}

void allRole(uint8_t num, uint8_t first){
	uint8_t governor = first;
	//first(0-3)
	uint8_t n = 0;
	char scan[32] = {0};
	uint8_t choose = 0;
	switch(num){
		case 1:
		// builder phase
			for(size_t i = 0 ; i<4 ; i++){
				if(first==0){
					for(size_t j = 0 ; j<built_sum[first] ; j++){
					
						uint8_t tobuild=0;
						int8_t cost_card=0;
						printf("Please choose a card to build or write skip.\n");
						for(size_t k = 0 ; k < player_sum[0] ; k++){
							printf("%ld: %s\n",k+1,name[player_cardnum[0][k]]);
						}
						printf("Your answer: \n");
						scanf("%s",scan);
						
						choose = atoi(scan);
						safe_flush(stdin);
						if( choose<1 || choose>=player_sum[0]){
							printf("input error\n");
							printf("Your answer: ");
							scanf("%s",scan);
							choose = atoi(scan);
							safe_flush(stdin);
						}
						if(strncmp(scan,"skip",strlen(scan))==0){
							break;
						}
						tobuild = player_cardnum[0][choose-1]; 
						if(0==governor){
							cost_card-=1;
						}
						for(size_t k = 0 ; k<built_sum[0] ; k++){
							//Smithy
							if(built[0][j]==0 && tobuild>=6 && tobuild<=10){
								cost_card-=1;
							}
							//Quarry
							if(built[0][j]==5 && tobuild<6 && tobuild>10){
								cost_card-=1;
							}
							//Carpenter
							if(built[0][j]==5 && tobuild<6 && tobuild>10){
								player_cardnum[0][player_sum[0]] = setCardNum();
								player_sum[0]++;
							}
						}
						//blackmarket
						//crane
						cost_card +=cost[tobuild];
						//cost<=0
						if(cost_card<=0){
							player_cardnum[0][choose-1]=0;
							for(size_t k = choose ; k<player_sum[0] ; k++){
								player_cardnum[0][k-1] = player_cardnum[0][k];
							}
							player_sum[0]--;
							built[0][built_sum[0]] = tobuild;
							built_sum[0]++;
							break;
						}
						if(cost_card<=player_sum[0]){
							player_cardnum[0][choose-1]=0;
							player_sum[0]--;
							built[0][built_sum[0]] = tobuild;
							built_sum[0]++;
							uint8_t topay = 0;
							for(int8_t k = player_sum[0] ; k >= 0 ; k--){
								topay = player_cardnum[0][k];
								if(k!=(choose-1)){
									discard[discard_sum] = topay;
									discard_sum++;
									player_cardnum[0][k] = 0;
									cost_card--;
								}
								if(cost_card==0){
									break;
								}
							}
							
							player_sum[0] -= cost_card; 
							//Poor house
							if(player_sum[0]<=1){
								player_cardnum[0][player_sum[0]] = setCardNum();
								player_sum[0]++;
							}
						}


					}
					
				}
				else{
					// point[player_cardnum[first][j]]
					comBuilt(first,governor);
				}
				
				first++;
				if(first>3){
					first = 0;
				}
			}
			break;
		case 2:
		// producer phase
	
			break;
		case 3:
		// trader phase
	
			break;
		case 4:
		// councilor phase
			break;
		case 5:
		// prospector phase
		// 持總督卡的玩家可從卡牌堆上方抽取1張卡牌加入手牌
			player_cardnum[first][player_sum[first]] = setCardNum();
			player_sum[first]++;
			// printf("%s\n",name[player_cardnum[first][player_sum[first]]]);
			printf("%s draws one card.\n",player_name[first]);
			for(size_t i = 0 ; i<4 ; i++){
				if(piece[18]==3){
					break;
				}
				for(size_t j = 0 ; j<built_sum[first] ; j++){
					if(built[first][j]==18){
						uint8_t a[4] = {0};
						for(size_t k = 0 ; k < 4 ; k++){
							a[k] = setCardNum();
						}
						if(a[0]!=a[1] && a[0]!=a[2] && a[0]!=a[3] && a[1]!=a[2] && a[1]!=a[3] && a[2]!=a[3]){
							printf("%s finds a gold.\n",player_name[first]);
							if(first==0){
								printf("You can choose a card: \n");
								for(size_t k = 0 ; k < 4 ; k++){
									printf("%ld: %s\n",k+1,name[a[k]]);
								}
								printf("Your answer(1-4): \n");
								scanf("%s",scan);
								choose = atoi(scan);
								safe_flush(stdin);
								while( choose<1 || choose>4){
									printf("input error\n");
									printf("Your answer: ");
									scanf("%s",scan);
									choose = atoi(scan);
									safe_flush(stdin);
								}
								player_cardnum[0][player_sum[0]] = a[choose-1];
								player_sum[0]++;
								sum+=3;
								for(size_t k = 0 ; k < 4 ; k++){
									if(k!=choose-1){
										piece[k]++;
									}
								}
							}
							else{
								player_cardnum[first][player_sum[first]] = a[0];
								player_sum[first]++;
								sum+=3;
								for(size_t k = 1 ; k < 4 ; k++){
									piece[k]++;
								}
							}
						}
						else{
							printf("%s finds no gold.\n",player_name[first]);
						}

					}
				}
				first++;
				if(first>3){
					first = 0;
				}
			}
			break;				

	}
	
	//Tower
	if(piece[19]!=3){

	}
	//Chapel
	if(piece[20]!=3){

	}
	//library
	if(piece[21]!=3){

	}
}

void printbuilding(uint8_t playernum){
	for(size_t i = 0 ; i < built_sum[playernum] ; i++){
		printf("\t%ld: %s\n",i+1,name[built[playernum][i]]);
	}
}

int main(){
	uint8_t n = 0;
	srand(time(NULL));
	// player[0]=setCardNum();
	// printf("player[0]: %d\n",player[0]);
	//set card
	built[0][0] = 6;
	built[1][0] = 6;
	built[2][0] = 6;
	built[3][0] = 6;

	printf("Com1 ");
	player_sum[1] = 4;
	printf("\tcards: %d\tVPs: %d\n",player_sum[1],player_vps[1]);
	for(size_t i = 0 ; i < 4 ; i++){
		player_cardnum[1][i] = setCardNum();
		// printf("%ld: %d\t",i+1,com1[i]);
	}
	printbuilding(1);
	printf("\n\nCom2 ");
	player_sum[2] = 4;
	printf("\tcards: %d\tVPs: %d\n",player_sum[2],player_vps[2]);
	for(size_t i = 0 ; i < 4 ; i++){
		player_cardnum[2][i] = setCardNum();
		// printf("%ld: %d\t",i+1,com2[i]);
	}
	printbuilding(2);
	printf("\n\nCom3 ");
	player_sum[3] = 4;
	printf("\tcards: %d\tVPs: %d\n",player_sum[3],player_vps[3]);
	for(size_t i = 0 ; i < 4 ; i++){
		player_cardnum[3][i] = setCardNum();
		// printf("%ld: %d\t",i+1,com3[i]);
	}
	printbuilding(3);
	printf("\n\nPlayer ");
	player_sum[0] = 4;
	printf(" cards: %d\tVPs: %d\n",player_sum[0],player_vps[0]);
	printbuilding(0);
	printf("\n\n--------------------------------------\n");
	printf("Your cards:\n");
	for(size_t i = 0 ; i < 4 ; i++){
		player_cardnum[0][i] = setCardNum();
		// printf("%ld: %d\t",i+1,player[i]);
		printf("\t%ld: %s\n",i+1,name[player_cardnum[0][i]]);
	}



	// for(size_t i = 0 ; i < 4 ; i++){
	// 	player[i] = setCardNum(n);
	// 	printf("player%ld: %d\t",i+1,player[i]);
	// 	com1[i] = setCardNum(n);
	// 	printf("com1%ld: %d\t",i+1,com1[i]);
	// 	com2[i] = setCardNum(n);
	// 	printf("com2%ld: %d\t",i+1,com2[i]);
	// 	com3[i] = setCardNum(n);
	// 	printf("com3%ld: %d\t",i+1,com3[i]);
	// }
	printf("--------------------------------------\n");
	uint8_t governor = rand()%4;
	char role[][32]={"Builder","Producer","Trader","Counciler","Prospector"};
	uint8_t role_num[]={1,1,1,1,1};
	uint8_t chooserole = 0;
	printf("This turn the governor is \"%s\"\n",player_name[governor]);	

	// while(1){
		for(size_t i = 0 ; i < 4 ; i++){
			
			if(governor==0){
				printf("Please choose a role for this turn:\n");
				for(size_t j = 0 ; j < 5 ; j++){
					if(role_num[j]==1){
						printf("%ld: %s\n",j+1,role[j]);
					}
					else{
						printf(GRAY"%ld: %s\n"NONE,j+1,role[j]);
					}
				}
				char role_scan[32] = {0};
				uint8_t role_choose = 0;
				printf("Your answer: ");
				scanf("%s",role_scan);
				role_choose = atoi(role_scan);
				safe_flush(stdin);

				while(role_num[role_choose-1]==0 || role_choose<1 || role_choose>5){
					printf("Didn't have the role.\n");
					printf("Your answer: ");
					scanf("%s",role_scan);
					role_choose = atoi(role_scan);
					safe_flush(stdin);
				}
				// printf("%d\n",role_choose);
				printf("Player chooses to be %s\n",role[role_choose-1]);
				role_num[role_choose-1] = 0;
				// allrole(role_choose,0);
				chooserole = role_choose;
			}
			else{
				uint8_t role_com = 0;
				uint8_t tmp = 0;
				while(1){
					tmp = rand()%5;
					if(role_num[tmp]==1){
						role_com = tmp;
						break;
					}
				}
				chooserole = tmp+1;
				printf("%s chooses to be %s\n",player_name[governor],role[role_com]);
				role_num[role_com] = 0;
				// allrole(role_com+1,governor);
			}
			allRole(chooserole, governor);
			governor++;
			if(governor>3){
				governor = 0;
			}
			
		}
	//governor<=4
	// governor++;
	// if(governor>3){
	// 	governor = 0;
	// }
	// printf("This turn the governor is %s\n",player_name[governor]);

	// }
	

	//sum of card
	// uint8_t sum = 0;
	// for(size_t i = 0 ; i < 29 ; i++){
	// 	sum = sum + piece[i];
	// 	printf("piece[%ld]: %d\t",i,piece[i]);
	// 	if(i%3==0){
	// 		printf("\n");
	// 	}
	// }
	// printf("sum: %d\n",sum);
	

	return 0;
}
