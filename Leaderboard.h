struct board{
	int score;
	char name[7];
};
void ShowLeaderboard(){
	int i;
	char x=0;
	struct board ranking[10]={0};
	FILE *fp;
	fp = fopen("leaderboard.dat","rb+");
	if(fp == NULL){
		fp = fopen("leaderboard.dat","wb+");
	}
	fread(ranking, sizeof(ranking), 1, fp);
	fclose(fp);
    system("cls");
	for(i=0;i<10;i++){
		printf("%2d. %6s, %09d\n", i+1, ranking[i].name, ranking[i].score);
	}
	printf("Press 'X' to Exit");
	while(!(x=='x'||x=='X'))x=getch();
	return titleScreen();
}
void CheckLeaderboard(int yourscore){
	struct board ranking[10]={0};
	int i=9, p;
	char yourname[7];
	FILE *fp;
	fp = fopen("leaderboard.dat","rb+");
	if(fp == NULL){
		fp = fopen("leaderboard.dat","wb+");
		fwrite(ranking, sizeof(ranking), 1, fp);
		fclose(fp);
		fread(ranking, sizeof(ranking), 1, fp);
		fclose(fp);
	}else{
		fread(ranking, sizeof(ranking), 1, fp);
		fclose(fp);
		if(ranking[9].score<yourscore){
			while(ranking[i-1].score<yourscore){
				i--;
				if(!i)break;
			}
			for(p=9;p>=i;p--){
				ranking[p]=ranking[p-1];
			}
			ranking[i].score=yourscore;
			ranking[i].name[0]='\0';
			pstr(34, 9, " HIGH SCORE ");
			for(p=0;p<10;p++){
				gotoxy(27,12+p);
				printf("%2d.%6s, %09d", p+1, ranking[p].name, ranking[p].score);
			}
			gotoxy(31, 12+i);
			for(p=0;p<5;p++){
				namewrite:;
				yourname[p]=getch();
				if(yourname[p]==8)if(p>0){
					p--;
					printf("%c %c",8,8);
					goto namewrite;
				}
				if(!(yourname[p]>='0'&&yourname[p]<='9'))goto namewrite;
				printf("%c", yourname[p]);
			}
			for(p=0;p<6;p++){
				ranking[i].name[p]=yourname[p];
			}
		}
	}
	fp = fopen("leaderboard.dat","wb+");
	fwrite(ranking, sizeof(ranking), 1, fp);
	fclose(fp);
	return;
}