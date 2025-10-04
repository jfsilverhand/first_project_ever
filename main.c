#include <stdio.h>
#include <stdlib.h>

// Nuri Eren Güven 220201008
void birlestir(int t[4],int m[5], int sayi){

	int yeni[sayi];


	for(int i=0; i<4; i++){
		t[i]=yeni[i];
	}
	for(int j=0; j<5; j++){
		m[j]=yeni[j+4];
	}

	for(int i=0; i<8; i++){
		if(yeni[i]==yeni[i+1]){
			printf("ayni sayidan 2 tane var");
		}
	}

	int tut;

	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			if(yeni[i]>yeni[j]){
				tut = yeni[j];
				yeni[j]=yeni[i];
				yeni[i]=tut;
			}
		}
	}

	for(int i=0; i<9; i++){
		printf("%d ",yeni[i]);
	}

}

int main()
{
	int boy1, boy2;



	printf("dizilerin boyutunu girin");
	scanf("%d",&boy1);
	scanf("%d",&boy2);

	int ilk[boy1];
	int son[boy2];
	if(boy1>=4 && boy1<=15 && boy2>=4 && boy2<=15){
		printf("girdiginiz sayilar uygundur");
	}
	else
	{
	printf("boyutlar hatali tekrar girin");
}
	printf("birinci diziyi giriniz");
	for(int i=0; i<4; i++){

	scanf("%d",&ilk[boy1]);

}
	printf("ikinci diziyi giriniz");
	for(int i=0; i<5; i++){

	scanf("%d",&son[boy2]);
}

	for(int i=0; i<4; i++){
		if(ilk[i]>100){
			printf("hata");
		}
	}
	for(int i=0; i<5; i++){
		if(son[i]>100){
			printf("hata");
		}
	}

birlestir(ilk,son,9);


    return 0;
}
