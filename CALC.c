#include <LiquidCrystal.h>
#include <util/delay.h>

//string za citanje
char uneseno[16]; 
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

char ocitaj_taster()
{
  char taster = 0;
  unsigned char row;
  
  MCUCR &= ~0x10;
  PORTD = 0x0f; 
  
  for(row = 0x80; row != 0x08; row >>= 1)
  {
    DDRD = row;
	_delay_ms(2);
    
    switch(row | ((~PIND) & 0x0f))
    {
      case 0x88:
      	taster = '1'; break;
      case 0x84:
      	taster = '2'; break;
      case 0x82:
      	taster = '3'; break;
      case 0x81:
      	taster = '+'; break;
		
	  case 0x48:
      	taster = '4'; break;
      case 0x44:
      	taster = '5'; break;
      case 0x42:
      	taster = '6'; break;
      case 0x41:
      	taster = '-'; break;
		
	  case 0x28:
      	taster = '7'; break;
      case 0x24:
      	taster = '8'; break;
      case 0x22:
      	taster = '9'; break;
      case 0x21:
      	taster = '*'; break;
		
	  case 0x18:
      	taster = '='; break;
      case 0x14:
      	taster = '0'; break;
      case 0x12:
      	taster = '#'; break;
      case 0x11:
      	taster = '/'; break;
    }
  }
  
  return taster;
}

int ubroji(char a, char b)
{
  	int x = 0, p = 1;
  	char i = b;
  	while (i >= a)
    {
      	x = x + p * (uneseno[i]-'0'); //ASCII
      	p *= 10;
      	i--;
    }
    return x;
}

void ocitaj_brojeve(int *br, char *znaci, char i, char *k)
{
  	char j = 0;
  	char a = 0;
  	while(j < i)
    {
      	if(uneseno[j] == '+' || uneseno[j] == '-' || uneseno[j] == '*' || uneseno[j] == '/' || uneseno[j] == '=')
        {
          	znaci[*k] = uneseno[j];
          	br[(*k)++] = ubroji(a,j-1);
          	a = j+1;
        }
      j++;
    }
}

int izracunaj(char i)
{
  	char znaci[8];
  	int brojevi[8];
  	char q;
  	char k = 0;
  	ocitaj_brojeve(brojevi,znaci,i,&k);
  	char j = 0;
  	while(j < k)
    {
      	if(znaci[j] == '*')
        {
          	brojevi[j] = brojevi[j] * brojevi[j+1];
        }
        
        else if(znaci[j] == '/')
        {
          	brojevi[j] = brojevi[j] / brojevi[j+1];
        }
      
      	else 
        {
          	j++;
         	continue;
        }
      
        q = j+1;
        while(q < k)
        {
        	brojevi[q] = brojevi[q+1];
            znaci[q-1] = znaci[q]; 
            q++;
        }
      	k--;
    }
  	j = 0;
  	while(j < k)
    {
      	if(znaci[j] == '=') break;
          
      	if(znaci[j] == '+')
        {
          	brojevi[j] = brojevi[j] + brojevi[j+1];
        }
        
        else
        {
          	brojevi[j] = brojevi[j] - brojevi[j+1];
        }
      
        q = j+1;
        while(q < k)
        {
        	brojevi[q] = brojevi[q+1];
            znaci[q-1] = znaci[q]; 
            q++;
        }
      	k--;
    }
	return brojevi[0];
}

int main()
{
  	lcd.begin(16, 2);
  	 char i = 0;
	
	while(1)
    { 
	  char taster = ocitaj_taster();
      if (taster != 0 && taster != '#')
      {
        lcd.print(taster);
      	_delay_ms(500);
        //sledeca linija je za softv. diff.
        uneseno[i++] = taster;
        while(taster == ocitaj_taster());
      }
      if(taster == '=') 
      {
        lcd.print(izracunaj(i));
        while(1)
        {
          if(ocitaj_taster() == '#')  
          {
            lcd.clear();
            i = 0;
            break;
          }
        }
      }
    }
  
  	return 0;
}