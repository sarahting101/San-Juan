#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define NONE "\033[m"
#define GRAY "\033[1;30m"

//[m][i-6]
uint8_t price[6][6] = {{1,2,2,3,3},{1,1,2,2,3},{1,2,2,2,3},{1,1,1,2,2},{1,1,2,2,2}};

//29 types of cards
//production 6~10
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
uint8_t point[]={1,1,1,1,2,2,1,1,2,2,3,1,2,1,2,1,1,2,1,2,2,3,3,4,5,0,0,0,0};
uint8_t piece[]={3,3,3,3,3,3,6,8,8,8,8,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2};
int8_t sum = 106;
//discard
uint8_t discard[111] = {0};
int8_t discard_sum = 0;

//player
char player_name[5][32]={"Player","Com 1 ","Com 2 ","Com 3 "};
uint8_t player_cardnum[5][13] = {0};
uint8_t player_sum[5] = {0};
uint8_t player_vps[5] = {0};
// player built
uint8_t built[5][13] = {0};
uint8_t built_sum[5] = {1,1,1,1};
uint8_t product[5][13] = {0};
uint8_t product_sum[5] = {0};

uint8_t com_level = 0;
uint8_t built_2[]={28,25,26,10,9,8,7,6};

void safe_flush(FILE *fp){
	int ch;
	while( (ch = fgetc(fp)) != EOF && ch != '\n' );     
}

uint8_t yourans(){
	char scan[32] = {0};
	uint8_t choose = 0;
	scanf("%s",scan);
	choose = atoi(scan);
	safe_flush(stdin);

	return choose;
}

uint8_t setCardNum(){
	int8_t x = 0;
	uint8_t i = 0;
	if(sum==0){
		for(size_t i = 0 ; i<discard_sum ; i++){
			piece[discard[i]]++;
		}
		sum = discard_sum;
		discard_sum = 0;
	}
	
	x = rand()%sum;
	while(x>=0){
		x = x - piece[i];
		i++; 
	
	}
	piece[i-1]--;
	sum--;
	return i-1;
}

uint8_t com_level2(uint8_t p, int8_t cost_card){
	int8_t cost_2 = 0;
	uint8_t flag = 0;
	for(int8_t i = 0 ; i < 8 ; i++){
		for(size_t j = 0 ; j < player_sum[p] ; j++){
			flag = 0;
			if(built_2[i]==player_cardnum[p][j]){
				if(player_cardnum[p][j]<6||player_cardnum[p][j]>10){
					for(size_t k = 0 ; k < built_sum[p] ; k++){
						if(built[p][k]==player_cardnum[p][j]){
							flag = 1;
							break;
						}
					}
				}
				if(flag==0){
					cost_2 = 0;
					for(size_t k = 0 ; k < built_sum[p] ; k++){
						if(i>=0&&i<=2&&built[p][k]==5){
							cost_2-=1;
						}
						else if(i>2&&built[p][k]==0){
							cost_2-=1;
						}
					}
					if(cost[player_cardnum[p][j]]<=player_sum[p]-1-cost_card-cost_2){
						
						return j;
					}
				}
			}
			// else{
			// 	break;
			// }

		}

	}
	return 30;
}


void builder(uint8_t governor,uint8_t p){
	int8_t cost_card = 0;
	//tobuild????????????player???????????????
	int8_t tobuild = 0;
	// sp or sp card to calculate cost
	if(p == governor){
		cost_card--;
		printf("Governor discount 1 card\n");
	}
	if(player_sum[p]==0){
		printf("%s builds nothing.\n",player_name[p]);
		return;
	}
	uint8_t crane_card = 0;
	uint8_t use_crane = 0;
	for(size_t j = 0 ; j < built_sum[p] ; j++){
		while(built[p][j]==2&& product_sum[p]>0){
			//Black market
			uint8_t newi[3] = {0};
			uint8_t n = 0;

			if(p==0){
				//player
				uint8_t choose=0;
				uint8_t choose2=0;

				printf("Please choose up to two goods to discard for black market.(or 0 to skip)\n");
				for(size_t i = 0 ; i<built_sum[0] ; i++){
					if(built[0][i]>=6 && built[0][i]<=10 && product[0][i]!=30){
						printf("%d: %s\n",n+1,name[built[0][i]]);
						newi[n] = i;
						n++;
					}
				}
				printf("Card1: ");
				choose = yourans();
				while(1){
					if(choose<0||choose>n){
						printf("input error\n");
						printf("Card1: ");
						choose = yourans();
					}
					else{
						break;
					}
				}
				if(choose==0){
					break;
				}
				else{
					product[0][newi[choose-1]] = 30;
					cost_card--;
					product_sum[0]--;
				}
				if(product_sum[0]>=1){
					printf("Card2: ");
					choose2 = yourans();
					while(1){
						if(choose2<0||choose2>n||choose==choose2){
							printf("input error\n");
							printf("Card2: ");
							choose2 = yourans();
						}
						else{
							break;
						}
					}
					if(choose2==0){
						break;
					}
					else{
						product[0][newi[choose2-1]] = 30;
						cost_card--;
						product_sum[0]--;

					}
				}
				
				break;

			}
			else{
				//com
				for(size_t i = 0 ; i<built_sum[p] ; i++){
					if(built[p][i]>=6 && built[p][i]<=10 && product[0][i]!=30){
						newi[n] = i;
						n++;
						if(n==2){
							break;
						}
					}
				}
				while(n>=0){
					product[p][newi[n-1]] = 30;
					cost_card--;
					product_sum[p]--;
					n--;

				}
			}
		}
		if(built[p][j]==3){
			//crane
			if(p==0){
				//player
				uint8_t choose = 0;
				printf("Do you want to use the crane?\n");
				printf("1: Yes\n");
				printf("2: No\n");
				printf("Your answer: ");
				choose = yourans();
				while(choose<=0||choose>2){
					printf("input error\n");
					printf("Your answer: ");
					choose = yourans();
				}
				if(choose==1){
					printf("Please choose a card to built over.\n");
					for(size_t i = 0 ; i < built_sum[0] ; i++){
						
						printf("%ld: %s\n",i+1,name[built[0][i]]);
					}
					printf("Your answer: ");
					choose = yourans();
					while(choose<=0||choose>built_sum[0]||built[0][choose-1]==3){
						printf("input error or cannot build over crane\n");
						printf("Your answer: ");
						choose = yourans();
					}
					crane_card = choose-1;
					// player_vps[0]-=point[built[0][crane_card]];
					// product[0][crane_card] = 30;
					cost_card-=cost[built[0][crane_card]];
					use_crane=1;


				}
				
			}
			else{
				//com 
				uint8_t x= rand()%2;
				if(x==0 && built_sum[p]>1){
					//yes
					// printf("%s use the crane.\n",player_name[p]);
					uint8_t y = rand()%built_sum[p];
					while(built[p][y]==3){
						y = rand()%built_sum[p];
					}
					crane_card = y;
					// player_vps[p]-=point[built[p][y]];
					// product[p][y] = 30;
					// cost_card-=cost[built[p][y]];

				}
			
			}

		}
		if(built[p][j]==21){
			printf("%s built Library\n",player_name[p]);
			cost_card-=2;
		}
	}
	
	
	// choose which card to build
	if(p==0){
		//player
		// char scan[32] = {0};
		int8_t cost_tmp = 0; 
		uint8_t choose = 0;
		uint8_t flag = 0;
		printf("Please choose a card to build or 0 to skip.\n");
		for(size_t i = 0 ; i < player_sum[0] ; i++){
			printf("%ld: %s\n",i+1,name[player_cardnum[0][i]]);
		}
		printf("Your answer: ");
		choose = yourans();
	
		while(flag!=1){
			flag = 0;
			tobuild = choose-1;
			if(choose==0){
				printf("Player builds nothing.\n");
				return;
			}

			if(choose<0 || choose>player_sum[0]){
				printf("input error\n");
				printf("Your answer: ");
				choose = yourans();
				flag=2;
			}
			else if(player_cardnum[0][tobuild]<6 || player_cardnum[0][tobuild]>10){
				for(size_t i = 0 ; i < built_sum[0] ; i++){
					if(built[0][i]==player_cardnum[0][tobuild]){
						printf("Violet building(%s) can only build one.\n",name[player_cardnum[0][tobuild]]);
						printf("Your answer: ");
						choose = yourans();
						flag=2;
						break;
					}
				}
				
			}
			
			
		
			if(flag == 0){
				cost_tmp = 0;
				for(size_t i = 0 ; i < built_sum[0] ; i++){
					if((player_cardnum[0][tobuild]<6 || player_cardnum[0][tobuild]>10)&&built[0][i]==5){
						cost_tmp-=1;
					}
					else if((player_cardnum[0][tobuild]>=6 && player_cardnum[0][tobuild]<=10)&&built[0][i]==0){
						cost_tmp-=1;
					}
				}
				
				if(cost[player_cardnum[0][tobuild]]>player_sum[0]-1-cost_card-cost_tmp){
					printf("input error\n");
					printf("Your answer: ");
					choose = yourans();
				}
				else{
					flag=1;
					cost_card+=cost_tmp;

					break;
				}
			}

		}
		
	
	}
	else{
		//com
		uint8_t flag_2 = 0;
		while(flag_2==0){
			if(com_level == 2 && player_sum[p]-1!=0){
				uint8_t tobuild_tmp = 0;
				tobuild_tmp = com_level2(p,cost_card);
				if(tobuild_tmp!=30){
					uint8_t cost_2 = 0;
					tobuild = tobuild_tmp;
					cost_2 = 0;
					for(int8_t i = 0 ; i < built_sum[p] ; i++){
						if(i>=0&&i<=2&&built[p][i]==5){
							cost_2-=1;
						}
						else if(i>2&&built[p][i]==0){
							cost_2-=1;
						}
					}
					cost_card-=cost_2;
					flag_2 = 1;
					break;
				}
			}
			uint8_t x =  rand()%2;
			if(x==0 || (player_sum[p]-1)==0){
				printf("%s builds nothing.\n",player_name[p]);
				return;
			}

			else{
				uint8_t flag = 0;
				int8_t cost_tmp = 0;
				tobuild = player_sum[p]-1;

				while(flag!=1){
					flag = 0;
					if(tobuild<0){
						printf("%s builds nothing.\n",player_name[p]);
						return;
					}
					if(player_cardnum[p][tobuild]<6||player_cardnum[p][tobuild]>10){
						for(size_t i = 0 ; i < built_sum[p] ; i++){
							if(built[p][i]==player_cardnum[p][tobuild]){
								flag=2;
								tobuild--;
								break;
							}
						}
						
					}
				

					if(flag==0){
						cost_tmp = 0;
						for(size_t i = 0 ; i < built_sum[p] ; i++){
							if((player_cardnum[p][tobuild]<6 || player_cardnum[p][tobuild]>10)&&built[p][i]==5){
								cost_tmp-=1;
							}
							else if((player_cardnum[p][tobuild]>=6 || player_cardnum[p][tobuild]<=10)&&built[p][i]==0){
								cost_tmp-=1;
							}
						}
						if(cost[player_cardnum[p][tobuild]]>player_sum[p]-1-cost_card-cost_tmp){
							tobuild--;
						}
						else{
							flag=1;
							flag_2 = 1;

							cost_card+=cost_tmp;

							break;
						}
					}
				}
			}
			
		}

	}
	//Carpenter
	for(size_t i = 0 ; i < built_sum[p] ; i++){
		if((player_cardnum[p][tobuild]<6 || player_cardnum[p][tobuild]>10)&&built[p][i]==4){
			printf("%s built Carpenter\n",player_name[p]);

			player_cardnum[p][player_sum[p]] = setCardNum();
			player_sum[p]++;
		}
	}
	printf("%s builds %s, ",player_name[p],name[player_cardnum[p][tobuild]]);

	uint8_t c_flag = 0;
	for(size_t i = 0 ; i < built_sum[p] ; i++){
		if(built[p][i]==3&&use_crane==1){
			//crane 
			player_vps[p]-=point[built[p][crane_card]];
			discard[discard_sum] = built[p][crane_card];
			discard_sum++;
			if(product[p][crane_card]!=30){
				product[p][crane_card] = 30;
				product_sum[p]--;
			}
			
			cost_card += cost[player_cardnum[p][tobuild]];
			built[p][crane_card] = player_cardnum[p][tobuild];
			player_vps[p]+=point[player_cardnum[p][tobuild]];
			for(size_t j = tobuild+1 ; j<player_sum[p] ; j++){
				player_cardnum[p][j-1] = player_cardnum[p][j];
			}
			player_sum[p]--;
			c_flag = 1;
			break;
		}
		
			
		
	}
	if(c_flag==0){
		//build card
		built[p][built_sum[p]] = player_cardnum[p][tobuild];
		built_sum[p]++;
		//delete the card which is built and row all card 
		cost_card += cost[player_cardnum[p][tobuild]];
		player_vps[p]+=point[player_cardnum[p][tobuild]];
		for(size_t j = tobuild+1 ; j<player_sum[p] ; j++){
			player_cardnum[p][j-1] = player_cardnum[p][j];
		}
		player_sum[p]--;
	}
	

	
	// use card to pay
	if(cost_card<=0){
		printf("need 0 card.\n");
		//poor house
		for(size_t i = 0 ; i < built_sum[p] ; i++){
			if(built[p][i]==1 && player_sum[p]<=1){
				player_cardnum[p][player_sum[p]] = setCardNum();
				player_sum[p]++;
			}
		}
	
		return;
	}
	printf("need %d card.\n",cost_card);
	if(cost_card==player_sum[p]){
	//all card are paid for the building
		for(size_t i = 0 ; i < player_sum[p] ; i++){
			discard[discard_sum] = player_cardnum[p][i];
			discard_sum++;
			player_cardnum[p][i] = 30;
		}
		player_sum[p]=0;
	}
	//how to pay
	else if(p==0){
		//player
		uint8_t choose = 0;
	
		uint8_t list[30]={0};
		for(size_t i = 0 ; i < player_sum[0] ; i++){
			list[i]=1;
		}
		printf("Please choose %d card to pay.\n",cost_card);
		for(size_t i = 0 ; i < player_sum[0] ; i++){
			printf("%ld: %s\n",i+1,name[player_cardnum[0][i]]);
		}
		
		for(size_t i = 0 ; i < cost_card ; i++){
			printf("Card%ld: ",i+1);
			choose = yourans();
			while( choose<1 || choose>player_sum[0] || list[choose-1]==0){
				printf("input error\n");
				printf("Card%ld: ",i+1);
				choose = yourans();
			}
			discard[discard_sum] = player_cardnum[0][choose-1];
			discard_sum++;
			player_cardnum[0][choose-1] = 30;
			list[choose-1]=0;
		}
		uint8_t n = 0;
		for(size_t i = 0 ; i < player_sum[0] ; i++){
			//delete card and row them
			if(player_cardnum[0][i]!=30){
				player_cardnum[0][n] = player_cardnum[0][i];
				n++;
			}
			if(n==player_sum[0]-cost_card){
				break;
			}
		}
		player_sum[0]-=cost_card;
		
	
	}
	else{
		//com
		int8_t cost_card2 = 0;
		cost_card2 = cost_card;
		for(int8_t i = player_sum[p]-1; i>=0 ; i--){
			discard[discard_sum] = player_cardnum[p][i];
			discard_sum++;
			player_cardnum[p][i]=30;
			cost_card--;
			
			if(cost_card==0){
				break;
			}
		}
		player_sum[p]-=cost_card2;
	}
	//poor house
	for(size_t i = 0 ; i < built_sum[p] ; i++){
		if(built[p][i]==1 && player_sum[p]<=1){
			player_cardnum[p][player_sum[p]] = setCardNum();
			player_sum[p]++;
		}
	}
	

	
	
	return;
}

void producer(uint8_t governor, uint8_t p){
	uint8_t mostproduce = 1;
	//calculate the most products can produce
	if(governor==p){
		//governor
		mostproduce++;
	}
	// findcard
	for(size_t i = 0 ; i < built_sum[p] ; i++){
		//Aqueduct
		if(built[p][i]==12){
			mostproduce++;
		}
		//Library
		if(built[p][i]==21){
			mostproduce = 3;
			break;
		}
	}
	// how many production
	uint8_t prodution = 0;
	uint8_t wantproduce = 0;

	for(size_t i = 0 ; i < built_sum[p] ; i++){
		if(built[p][i]>=6 && built[p][i]<=10){
			prodution++;
		}
	}
	// can produce number
	uint8_t canproduce = mostproduce;
	 
	if(prodution-product_sum[p]<=mostproduce){
		wantproduce = prodution-product_sum[p];
		for(size_t i = 0 ; i < built_sum[p] ; i++){
			if(built[p][i]>=6 && built[p][i]<=10 && product[p][i]==30){
				product[p][i] = setCardNum();
				product_sum[p]++;
			}
		}
	}

	else if(p==0){
		//player
		uint8_t choose = 0;
		char scan[32] = {0};
		//how many production
		uint8_t n = 0;
		uint8_t list[30]={0};
		//???built[0][?]
		uint8_t prodution_list[12]={0};
	
		printf("Please choose at most %d cards to produce goods.(write 0 to skip)\n",mostproduce);
		for(size_t i = 0 ; i<built_sum[0] ; i++){
			if(built[0][i]>=6 && built[0][i]<=10 && product[0][i]==30){
				printf("%d: %s\n",n+1,name[built[0][i]]);
				prodution_list[n]=i;
				n++;
				
			}
		}
		for(size_t i = 0 ; i < n ; i++){
			list[i]=1;
		}
		for(size_t i = 0 ; i < mostproduce ; i++){
			printf("Card%ld: ",i+1);
			choose = yourans();
			if(choose==0){
				break;
			}
			while(choose>n || choose<0 || list[choose-1]==0){
				
				printf("input error\n");
				printf("Card%ld: ",i+1);
				choose = yourans();
				if(choose==0){
					break;
				}
				
			}
			if(choose==0){
				break;
			}
			list[choose-1]=0;
			product[0][prodution_list[choose-1]]=setCardNum();
			product_sum[0]++;
			wantproduce++;
			
		}
	}
	else{
		//com
		wantproduce = canproduce;
		for(size_t i = 10 ; i >= 6 ; i--){
			for(size_t j = 0 ; j < built_sum[p] ; j++){
				if(built[p][j]==i && product[p][j]==30){
					product[p][j]=setCardNum();
					product_sum[p]++;

					canproduce--;
					if(canproduce==0){
						break;
					}
				}

			}
			if(canproduce==0){
				break;
			}
		}
	}

	if(wantproduce==1){
		printf("%s produce 1 good\n",player_name[p]);
	}
	else{
		//well
		for(size_t i = 0 ; i <built_sum[p] ; i++){
			if(built[p][i]==11){
				player_cardnum[p][player_sum[p]] = setCardNum();
				player_sum[p]++;
				break;
			}
		}

		printf("%s produce %d goods\n",player_name[p],wantproduce);
	}
}

void trader(uint8_t governor, uint8_t p,uint8_t m){
	if(product_sum[p]==0){
		printf("%s sells nothing.\n",player_name[p]);
		return;
	}
	uint8_t mostsell = 1;
	if(governor==p){
		//governor
		mostsell++;
	}
	// findcard
	for(size_t i = 0 ; i < built_sum[p] ; i++){
		//Library
		if(built[p][i]==21){
			mostsell = 3;
			break;
		}
		//Trading post
		if(built[p][i]==15){
			mostsell++;
		}
	}
	// real sell num
	uint8_t realsell = 0;
	// can sell num
	uint8_t cansell = 0;
	uint8_t getcard = 0;
	if(p==0){
		//player
		uint8_t choose = 0;
		//how many production
		uint8_t n = 0;
		uint8_t list[30]={0};
		//???built[0][?]
		uint8_t prodution_list[30]={0};
		if(product_sum[0]<mostsell){
			mostsell = product_sum[0];
		}
		printf("Please choose up to  %d good(s) to sell.(write 0 to skip)\n",mostsell);
		for(size_t i = 0 ; i<built_sum[0] ; i++){
			if(built[0][i]>=6 && built[0][i]<=10 && product[0][i]!=30){
				printf("%d: %s\n",n+1,name[built[0][i]]);
				prodution_list[n]=i;
				n++;
			}
		}
		for(size_t i = 0 ; i < n ; i++){
			list[i]=1;
		}
		for(size_t i = 0 ; i < mostsell ; i++){
			printf("Card%ld: ",i+1);
			choose =  yourans();
			if(choose==0){
				break;
			}
			while(choose>n || choose<0 || list[choose-1]==0){
				printf("input error\n");
				printf("Card%ld: ",i+1);
				choose =  yourans();
				if(choose==0){
					break;
				}
			}
			if(choose==0){
				break;
			}
			list[choose-1]=0;
			realsell++;
			discard[discard_sum] = product[0][prodution_list[choose-1]];
			discard_sum++;
			getcard+=price[m][built[0][prodution_list[choose-1]]-6];
			product[0][prodution_list[choose-1]] = 30;
		
		}
		product_sum[0] -=realsell;
	}
	else{
		//com
		if(mostsell>=product_sum[p]){
			//sell all
			realsell = product_sum[p];
			for(size_t i = 0 ; i < built_sum[p] ; i++){
				if(product[p][i]!=30){
					discard[discard_sum] = product[p][i];
					discard_sum++;
					getcard+=price[m][built[p][i]-6];
					product[p][i]=30;
					
				}
			}
			product_sum[p] = 0;
		}
		else{
			realsell = mostsell;
			uint8_t r = realsell;
			for(size_t i = 10 ; i >= 6 ; i--){
				for(size_t j = 0 ; j < built_sum[p] ; j++){
					if(built[p][j]==i && product[p][j]!=30){
						discard[discard_sum] = product[p][j];
						discard_sum++;
						getcard+=price[m][i-6];
						product[p][j]=30;
						r--;
						if(r==0){
							break;
						}

					}

				}
				if(r==0){
					break;
				}
			}
			product_sum[p]-=realsell;
		}

	}
	
	for(size_t i = 0 ; i < built_sum[p] ; i++){
		//Market stand
		if(built[p][i]==13 && realsell>=2){
			getcard++;
		}
		//Market hall
		if(built[p][i]==14 && realsell>=1){
			getcard++;
		}
	
	}
	printf("%s sells %d good(s), getting %d card(s).\n",player_name[p],realsell,getcard);

	while(getcard!=0){
		player_cardnum[p][player_sum[p]] = setCardNum();
		player_sum[p]++;
		getcard--;
	}
}

void prospector(uint8_t governor, uint8_t p){
	uint8_t getcard = 0;
	//Gold mine flag
	uint8_t flag = 0; 
	if(governor==p){
		//governor
		getcard++;
	}
	for(size_t i = 0 ; i < built_sum[p] ; i++){
		//Library
		if(built[p][i]==21){
			getcard =2;
		}
		//Gold mine
		if(built[p][i]==18){
			flag = 1;
		}
	}
	if(getcard==0 && flag==0){
		printf("%s gets no card.\n",player_name[p]);
		return;
	}
	if(getcard!=0){
		printf("%s draws %d card(s).\n",player_name[p],getcard);
		while(getcard!=0){
			player_cardnum[p][player_sum[p]] = setCardNum();
			player_sum[p]++;
			getcard--;
		}
	}
	
	if(flag == 1){
		uint8_t a[4] = {30};
		for(size_t i = 0 ; i < 4 ; i++){
			a[i] = setCardNum();
		}
		uint8_t cost_0 = cost[a[0]];
		uint8_t cost_1 = cost[a[1]];
		uint8_t cost_2 = cost[a[2]];
		uint8_t cost_3 = cost[a[3]];

		if(cost_0!=cost_1 && cost_0!=cost_2 && cost_0!=cost_3 && cost_1!=cost_2 && cost_1!=cost_3 && cost_2!=cost_3){
			printf("%s have found gold!\n",player_name[p]);
			if(p==0){
				//player
				uint8_t choose = 0;
				printf("Please choose one card to keep.(1-4) \n");
				for(size_t i = 0 ; i < 4 ; i++){
					printf("%ld: %s\n",i+1,name[a[i]]);
				}
				printf("Your answer: ");
				choose = yourans();
				while(choose-1>3 || choose-1<0){
					printf("input error\n");
					printf("Your answer: ");
					choose = yourans();
				}
				player_cardnum[0][player_sum[0]] = a[choose-1];
				player_sum[0]++;
				for(size_t i = 0 ; i < 4 ; i++){
					if(i!=choose-1){
						discard[discard_sum] = a[i];
						discard_sum++;
					}
				}

			}
			else{
				//com
				player_cardnum[p][player_sum[p]] = a[0];
				player_sum[p]++;
				for(size_t i = 1 ; i < 4 ; i++){
					discard[discard_sum] = a[i];
					discard_sum++;
				}
			}
		}
		else{
			printf("%s finds no gold.\n",player_name[p]);
			for(size_t i = 0 ; i < 4 ; i++){
				discard[discard_sum] = a[i];
				discard_sum++;
			}
		}
	
	}	
	


}

void counciler(uint8_t governor, uint8_t p){
	uint8_t draw = 0;
	uint8_t draw_card[9] = {0};
	uint8_t getcard = 1;
	uint8_t flag = 0;
	for(size_t i = 0 ; i < built_sum[p] ; i++){
		//draw
		if(built[p][i]==21){
			draw = 8;
			break;
		}
		else if(p==governor){
			draw = 5;
		}
		else{
			draw = 2;
		}
		//getchar
		if(built[p][i]==17){
			getcard += 1;
		}
		if(built[p][i]==16){
			getcard+=player_sum[p];
			flag = 1;
		}
	}
	printf("%s draws %d cards and keep %d.\n",player_name[p],draw,getcard);
	for(size_t i = 0 ; i < draw ; i++){
		draw_card[i] = setCardNum();
	}
	if(p==0){
		//player
		if(flag==1){
			//Archive
			printf("You have Archive ! Please choose %d card(s) to keep.\n",getcard);
			uint8_t choose = 0;
			uint8_t list[40]={0};
			for(size_t i = 0 ; i < player_sum[0] ; i++){
				printf("%ld: %s\n",i+1,name[player_cardnum[0][i]]);
			}
			for(size_t i = 0 ; i < draw ; i++){
				printf("%ld: %s\n",i+player_sum[0]+1,name[draw_card[i]]);
			}
			for(size_t i = 0 ; i < player_sum[0]+draw ; i++){
				list[i]=1;
			}
			//scanf
			for(size_t i = 0 ; i < getcard ; i++){
				printf("Card%ld: ",i+1);
				choose = yourans();
				while(choose<=0 || choose>draw+player_sum[0] || list[choose-1]==0){
					printf("input error\n");
					printf("Card%ld: ",i+1);
					choose = yourans();
				}
				list[choose-1]=0;
			}
			uint8_t n = 0;
			for(size_t i = 0 ; i < player_sum[0]+draw ; i++){
				if(list[i]==0){
					if(i<player_sum[0]){
						player_cardnum[0][n]=player_cardnum[0][i];
					}
					else{
						player_cardnum[0][n]=draw_card[i-player_sum[0]];
					}
					n++;
				}
				else{
					if(i<player_sum[0]){
						discard[discard_sum] = player_cardnum[0][i];
						discard_sum++;
					}
					else{
						discard[discard_sum] = draw_card[i];
						discard_sum++;
					}
				}
			}
			player_sum[0] = getcard;
		}
		else{
			//No Archive
			uint8_t choose = 0;
			uint8_t cardlist[3]={0};
			uint8_t listnum = 0;
			printf("Please choose %d card(s) to keep.\n",getcard);
			while(1){
				listnum=0;
				for(size_t i = 0 ; i < draw ; i++){
					printf("%ld: %s\n",i+1,name[draw_card[i]]);
				}
				for(size_t i = 0 ; i < getcard ; i++){
					printf("Card%ld: ",i+1);
					choose = yourans();
					while(choose<=0 || choose>draw){
						printf("input error\n");
						printf("Card%ld: ",i+1);
						choose = yourans();
					}
					cardlist[listnum] = choose-1;
					listnum++;
				
				}
				if(listnum==1){
					break;
				}
				if(cardlist[0]==cardlist[1]){
					printf("Connot choose the same number.\n");
				}
				else{
					break;
				}

			}


			//add player card
			for(size_t i = 0 ; i < getcard ; i++){
				
				player_cardnum[0][player_sum[0]]=draw_card[cardlist[i]];
				draw_card[cardlist[i]]=30;
				player_sum[0]++;
			}

			
			//discard
			for(size_t i = 0 ; i <draw ; i++){
				//draw_card
				if(draw_card[i]!=30){
					discard[discard_sum] = draw_card[i];
					discard_sum++;
					draw_card[i]=30;
				}
			}
			
		}

	}
	else{
		//com
		//only pick draw_card(from draw_card[0])
		if(flag==1){
			getcard -= player_sum[p];
		}
		for(size_t i = 0 ; i < getcard ; i++){
			player_cardnum[p][player_sum[p]] = draw_card[i];
			player_sum[p]++;
		}
		for(size_t i = getcard ; i < draw ; i++){
			discard[discard_sum] = draw_card[i];
			discard_sum++;
		}
	}

}

void printallcom(){
	uint8_t p = 1;
	uint8_t n = 0;
	uint8_t n2 = 0;
	printf("-");

	while(p<=3){
		n = 0;
		n2 = 0;
		printf("\n\n%s ",player_name[p]);
		printf("\tcards: %d\tVPs: %d\n",player_sum[p],player_vps[p]);
		
		for(size_t i = 0 ; i < built_sum[p] ; i++){
		
			if(product[p][i]<30){
				printf("\t%ld: %-15s(good)",i+1,name[built[p][i]]);

			}
			else{
				printf("\t%ld: %-15s",i+1,name[built[p][i]]);
			}
			n++;
			if(n%4==0){
				printf("\n");
			}
		}
		/*
		printf("\n\tcards:\n");

		for(size_t i = 0 ; i < player_sum[p] ; i++){
			printf("\t%ld: %-15s(%d/%d)",i+1,name[player_cardnum[p][i]],cost[player_cardnum[p][i]],point[player_cardnum[p][i]]);
			n2++;
			if(n2%4==0){
				printf("\n");
			}
		}
		*/
		p++;
	}
}

void printplayer(){
	uint8_t n = 0;
	uint8_t n2 = 0;
	printf("\n\n%s ",player_name[0]);
	printf("\tcards: %d\tVPs: %d\n",player_sum[0],player_vps[0]);
	for(size_t i = 0 ; i < built_sum[0] ; i++){
		if(product[0][i]<30){
				printf("\t%ld: %-15s(good)",i+1,name[built[0][i]]);

			}
		else{
			printf("\t%ld: %-15s",i+1,name[built[0][i]]);
		}
		n++;
		if(n%4==0){
			printf("\n");
		}
	}
	printf("\n\n-\n\n");
	printf("Your card(s):\n");
	for(size_t i = 0 ; i < player_sum[0] ; i++){
		printf("\t%ld: %-15s(%d/%d)",i+1,name[player_cardnum[0][i]],cost[player_cardnum[0][i]],point[player_cardnum[0][i]]);
		n2++;
		if(n2%4==0){
			printf("\n");
		}
		
	}
	printf("\n-\n");

}

void finalvps(){
	uint8_t win = 0;

	for(size_t i = 0 ; i < 4 ; i++){
		for(size_t k = 0 ; k < built_sum[i] ; k++){
			//Guild hall
			if(built[i][k]==25){
				for(size_t j = 0 ; j < built_sum[i] ; j++){
					if(built[i][j]>=6 && built[i][j]<=10){
						player_vps[i]+=2;
					}
				}
			}
			//City hall
			if(built[i][k]==26){
				for(size_t j = 0 ; j < built_sum[i] ; j++){
					if(built[i][j]<6 || built[i][j]>10){
						player_vps[i]++;
					}
				}
			}
			//Triumhal arch
			if(built[i][k]==27){
				uint8_t m = 0;
				for(size_t j = 0 ; j < built_sum[i] ; j++){
					if(phase[built[i][j]]==8){
						m++;
					}
				}
				if(m==1){
					player_vps[i]+=4;
				}
				else if(m==2){
					player_vps[i]+=6;
				}
				else if(m==3){
					player_vps[i]+=8;
				}
			}
			
		}
		for(size_t k = 0 ; k < built_sum[i] ; k++){
			//Palace
			if(built[i][k]==28){
				uint8_t bf_vps = player_vps[i];
				player_vps[i]+=bf_vps/4;
			}
		
		}
		if(player_vps[i]>player_vps[win]){
			win = i;
		}
		else if(player_vps[i]==player_vps[win]){
			if(product_sum[i]+player_sum[i]>player_sum[win]+product_sum[win]){
				win = i;
			}
		}
		
		printf("%s total: %d\n",player_name[i],player_vps[i]);
	
	}
	printf("%s is winner!\n",player_name[win]);
	return;
}




int main(){
	uint8_t n = 0;
	srand(time(NULL));

	//set card
	built[0][0] = 6;
	built[1][0] = 6;
	built[2][0] = 6;
	built[3][0] = 6;
	//product
	for(size_t i = 0 ; i < 5 ; i ++){
		for(size_t j = 0 ; j < 13 ; j ++){
			product[i][j]=30;
		}
	}
	for(size_t i = 0 ; i < 4 ; i++){
		player_sum[i] = 4;
		for(size_t j = 0 ; j < 4 ; j ++){
			player_cardnum[i][j] = setCardNum();
		}
	}
	printf("Start!\n");
	printf("Choose the computer agents level:\n");
	printf("1: Level 1\n");
	printf("2: Level 2\n");
	printf("Your answer: ");
	uint8_t choose = 0;
	choose = yourans();
	while(choose<=0||choose>2){
		printf("input error\n");
		printf("Your answer: ");
		choose = yourans();
	}
	com_level = choose;
	printf("Computer agents are level%d.\n",com_level);
	printallcom();
	printplayer();
	printf("Press ENTER to continue...");
	getchar();
	printf("-\n");

	
	uint8_t governor = rand()%4;
	char role[][32]={"Builder","Producer","Trader","Counciler","Prospector"};
	uint8_t role_num[]={1,1,1,1,1};
	uint8_t chooserole = 0;
	printf("This turn the governor is \"%s\"\n",player_name[governor]);	

	while(1){
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
				printf("Player chooses to be %s\n",role[role_choose-1]);
				role_num[role_choose-1] = 0;
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
			}
			//run role
			uint8_t p = 0;
			if(chooserole==1){
				// printf("builder\n");
				for(size_t i = 0 ; i<4 ; i++){
					p = governor + i;
					if(p>=4){
						p = p-4;
					}
					builder(governor,p);
				}

			}
			else if(chooserole==2){
				// printf("producer\n");
				for(size_t i = 0 ; i<4 ; i++){
					p = governor + i;
					if(p>=4){
						p = p-4;
					}
					producer(governor,p);
				}
			}
			else if(chooserole==3){
				// printf("trader\n");
				uint8_t m = rand()%5;
				printf("Price: ");
				for(size_t i = 0 ; i<5 ; i++){
					printf("%d",price[m][i]);

				}
				printf("\n");
				for(size_t i = 0 ; i<4 ; i++){
					p = governor + i;
					if(p>=4){
						p = p-4;
					}
					trader(governor,p,m);
				}
			}
			else if(chooserole==4){
				// printf("counciler\n");
				for(size_t i = 0 ; i<4 ; i++){
					p = governor + i;
					if(p>=4){
						p = p-4;
					}
					counciler(governor,p);
				}
			}
			else if(chooserole==5){
				// printf("prospector\n");
				for(size_t i = 0 ; i<4 ; i++){
					p = governor + i;
					if(p>=4){
						p = p-4;
					}
					prospector(governor,p);
				}
			}
		
			governor++;
			if(governor>3){
				governor = 0;
			}
			
			printallcom();
			printplayer();
			printf("Press ENTER to continue...");
			getchar();
			printf("-\n");
		}
		for(size_t i = 0 ; i < 4 ; i++){
			if(built_sum[i]==12){
				printf("Game Over\n");
				finalvps();
				return 0;
			}
		}
		
		for(size_t i = 0 ; i < 5 ; i++){
			role_num[i] = 1;
		}
		governor ++;
		if(governor>3){
			governor = 0;
		}
		printf("supply: %d, discard: %d\n",sum,discard_sum);
		printf("This turn the governor is \"%s\"\n",player_name[governor]);	

		//Tower and Chapel
		uint8_t dis = 0;
		//Tower
		for(size_t i = 0 ; i< 4 ; i++){

			if(player_sum[i]<=7){
				dis = 0;
			}
			else if(player_sum[i]>7 && player_sum[i]<=12){
				uint8_t t_flag = 0;
				for(size_t j = 0 ; j < built_sum[i] ; j++){
					if(built[i][j]==19){
						t_flag = 1;
						break;
					}
				}
				if(t_flag==0){
					dis = player_sum[i]-7;
				}
			}
			else{
				uint8_t t_flag = 0;
				for(size_t j = 0 ; j < built_sum[i] ; j++){
					if(built[i][j]==19){
						dis = player_sum[i]-12;
						t_flag = 1;
						break;
					}
				}
				if(t_flag==0){
					dis = player_sum[i]-7;
				}

			}

			if(dis==0){

			}
			//player or com
			else if(i==0){
				//player
				uint8_t choose = 0;
				uint8_t list[30] = {0};
				printf("Please choose %d card(s) to discard from your hand.\n",dis);
			
				for(size_t j = 0 ; j < player_sum[0] ; j++){
					list[j]=1;
					printf("%ld: %s\n",j+1,name[player_cardnum[0][j]]);
				}
				for(size_t j = 0 ; j < dis ; j++){
					printf("Card%ld: ",j+1);
					choose = yourans();
					while(choose<=0||choose>player_sum[0]||list[choose-1]==0){
						printf("input error\n");
						printf("Card%ld: ",j+1);
						choose = yourans();
					}
					discard[discard_sum] = player_cardnum[0][choose-1];
					discard_sum++;
					list[choose-1]=0;
				}
				//row
				uint8_t n = 0;
				for(size_t j = 0 ; j < player_sum[0] ; j++){
					if(list[j]==1){
						player_cardnum[0][n] = player_cardnum[0][j];
						n++;
					}
					if(n==player_sum[0]-dis){
						break;
					}
				}
				player_sum[0]-=dis;
				
			}
			else{
				//com
				printf("%s discards %d from the hand.\n",player_name[i],dis);
				for(size_t j = player_sum[i]-1 ; j >= player_sum[i]-dis  ; j--){
					discard[discard_sum] = player_cardnum[i][j];
					discard_sum++;
					player_cardnum[i][j] = 30;
				}
				
				player_sum[i] -= dis;
				
			}
		}
		//Chapel
		for(size_t i = 0 ; i < 4 ; i++){
			for(size_t j = 0 ; j < built_sum[i] ; j++){
				if(built[i][j]==20){
					if(i==0){
						//player
						uint8_t choose = 0;
						printf("Please choose one card to put under chapel or 0 to skip.\n");
						for(size_t j = 0 ; j< player_sum[0] ; j++){
							printf("%ld: %s\n",j+1,name[player_cardnum[0][j]]);
						}
					
						printf("Your answer: ");					
						choose = yourans();
						if(choose==0){
							break;
						}
						while(choose<=0||choose>player_sum[0]){
							printf("input error\n");
							printf("Your answer: ");					
							choose = yourans();
							if(choose==0){
								break;
							}
						}
						if(choose==0){
							break;
						}
						
						player_vps[0]++;
						//row the card in hand
						for(size_t j = choose-1 ; j< player_sum[0] ; j++){
							player_cardnum[0][j] = player_cardnum[0][j+1];
						}
						player_sum[0]--;
					}
					else{
						player_vps[i]++;
						player_sum[i]--;
					}
				}
			}
		
		}
	}
	return 0;
}
