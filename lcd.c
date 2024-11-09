#pragma nonrec
#pragma optimize space
#include <stdio.h>
#include "iotlib.h"
#include "glib.h"

pause(p)
int p;
{
	int *b;
	b = 0xFC9E;
	*b = 0;
	while(*b < p)
	{
		;
	}
}

send(rs,cm,node)
TINY rs;
TINY cm;
TINY *node;
{
	TINY *c1;

	*c1 = cm;
	*c1 = *c1 & 0xF0;
	*c1 = *c1 | 0x0C;
	*c1 = *c1 + rs;
	iotpts(node,c1);

	*c1 = *c1 & 0xF9;
	iotpts(node,c1);
}

comd(cm,node)
TINY cm;
TINY *node;
{
	send((TINY)0,cm,node);
	cm = cm << 4;
	send((TINY)0,cm,node);
}

data(cm,node)
TINY cm;
TINY *node;
{
	send((TINY)1,cm,node);
	cm = cm << 4;
	send((TINY)1,cm,node);
}

print(s,node)
TINY *s;
TINY *node;
{
	size_t	l;
	int	i,j,cnt;
	TINY	a[81];
	TINY	b;
	l = strlen(s);
	if (l > 20) l = 20;
	cnt = 0;
	for(i = 0;i < l;++i){
		b = s[i];
		for(j = 0;j < 2;++j){
			a[cnt] = b;
			a[cnt] = a[cnt] & 0xF0;
			a[cnt] = a[cnt] | 0x0C;
			a[cnt] = a[cnt] + 1;
			cnt++;
			a[cnt] = a[cnt - 1] & 0xF9;
			cnt++;
			b = b << 4;
		}
	}
	iotpts(node,a);
}

loc(x,y,node)
TINY x;
TINY y;
TINY *node;
{
	TINY y1;
	if(x > 19)x = 19;
	switch(y){
	case 0:
		y1 = 0x0;
		break;
	case 1:
		y1 = 0x40;
		break;
	case 2:
		y1 = 0x14;
		break;
	case 3:
		y1 = 0x54;
		break;
	default:
		y1 = 0x0;
	}
	y1 = y1 + x;
	y1 = y1 | 0x80; 
	comd(y1,node);
}

main(){
	TINY	node[20];
	TINY*	adr[5];
	TINY	txt[4][20];
	int	n,ad,i,j,q;
	TINY	rs,cm,c;
	TINY	test[81];
	strcpy(node,"device/i2c_a");
	ad = iotfds(node,adr);
	strcat(node,"/");
	strcat(node,adr[0]);
	rs = (char)0;
	send(rs,(char)0x30,node);
	pause(5);
	send(rs,(char)0x30,node);
	pause(1);
	send(rs,(char)0x30,node);
	send(rs,(char)0x20,node);
	comd((char)0x28,node);
	comd((char)0x06,node);
	comd((char)0x01,node);
	pause(2);
	comd((char)0x0F,node);
	pause(1);

	printf("\33j");
	puts("ABCDEFG\n12345678901234567890\nｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿ!\n");
	printf("%s\n",node);

	while(breakx()!=1){
		for(i = 0;i < 4;++i){
			loc((TINY)0,(TINY)i,node);
			for(j = 0;j < 20;j+=5){
				for(q = 0;q < 5;++q){
					test[q] = vpeek((NAT)i*80+j+q);
				}
				test[5] = 0;
				print(test,node);
			}
		
		}	
	}
}

