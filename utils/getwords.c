#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
/*
get all words without special characters
*/
int main()
{
	/*
		write to length index files. discard single letter words. 
	*/
	//largest len
	size_t n = 2, len; //no one letter words
	const size_t sz = 23;
	char  w[sz]; 
	char buf[sz] ;

	FILE * out = fopen("words.txt", "w");
	FILE * f = fopen("/usr/share/dict/words", "r");
  	assert(f);
next:
	while(fgets(buf,sz, f) )
	{
		len = strlen(buf);
		if(buf[len-1]=='\n'){
			buf[len-1] = '\0';
			len--;
		} 

		if(len >= 2)
		{
			int i = -1;
			while(++i < len)
			{
				if(buf[i] < 'a' || buf[i] > 'z')
					goto next;
			}
			if(buf[0] >= 'a' && buf[0] <='z')
			{
				fwrite(buf,1,len,out);
				fputc('\n', out);
				if(len > n)
				{
					n = len;
					strcpy(w, buf);
				}
			}

		}


	}
	printf("Longest word %s %d\n",w, n);
	fclose(out);
	fclose(f);
}