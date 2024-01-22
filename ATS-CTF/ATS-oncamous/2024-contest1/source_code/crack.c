#include<stdio.h>
char a1[]="hi travler, i am glad to see you come to here\n";
char a2[]="you need to pass the exam i have gave to you\n";
char a3[]="if you have not learn about C Programming Language, you should learn about it\nor you will never crack our secret\n";
char a4[]="if you see this sentence , it means you have master how yo use ida ,that is a nice beginning\n";
char a5[]="and now , i can tell my secret to you,it is\n";
char chip[]={0x57,0x51,0x5a,0x54,0x60,0x62,0x6c,0x66,0x5c,0x55,0x5a,0x67,0x56,0x5c,0x68,0x66,0x58,0x58,0x56,0x68,0x68,0x57,0x66,0x51,0x51,0x62,0x5c,0x5b,0x6b,0x52,0x6f,0x54,0x5c,0x5b,0x5a,0x58,0x50,0x5c,0x69,0x55,0x56,0x5c,0x68,0x56,0x58,0x6b,0x56,0x69,0x7e};
signed main(){

 //	string a="flag{you_have_successfully_bring_back_the_secret}";
    printf("%s",a1);
    printf("%s",a2);
    printf("%s",a3);
    
    printf("tell me what you know\n");
    char flag[49];
    gets(flag);

    if(sizeof(flag)!=49){
        printf("you stupid outcomer\n");
    }
    else{

        for(int i=0;i<49;i++){
            
            flag[i]-=13;
            flag[i]^=14;
            if(chip[i]!=flag[i]){
                printf("\nit seems you have not reveal the secret in our country\n");
                return 0;
            }
        }
        printf("\ngerat, you have penetrate the mysteries!!!\n");

    }


}