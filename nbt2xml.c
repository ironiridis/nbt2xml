#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char blank[1] = "";

void printindent(int i)
{
	while(i--) putchar(' ');
}

long readlong(void)
{
	long a = 0;
	long b = 0;
	a = (getchar() << 24) | (getchar() << 16) | (getchar() << 8) | getchar();
	b = (getchar() << 24) | (getchar() << 16) | (getchar() << 8) | getchar();
	return(a << 32 | b);
}

int readint(void)
{
	int i = 0;
	i = (getchar() << 24) | (getchar() << 16) | (getchar() << 8) | getchar();
	return(i);
}

int readshort(void)
{
	int i;
	i = (getchar() << 8) | getchar();
	return(i);
}

int readbyte(void)
{
	return(getchar());
}

void readname(char *buf)
{
	int l, r;
	l = getchar();
	r = fread(buf, 1, l, stdin);
	
	if (r < l)
		printf("\n--- failed to read tag name ---\n");
	
	buf[r] = '\0';
	
}

/*
int tagintrinsiclength(int t)
{
	switch(t)
	{
		case 1: return(1);
		case 2: return(2);
		case 3: return(4);
		case 4: return(8);
		case 5: return(4);
		case 6: return(8);
		default: return(0);
	}
}
*/

int readtag(int depth, int listtag)
{
	int tag;
	int len;
	int j;
	char *namebuf;
	namebuf = NULL;
	
	if (listtag)
	{
		// list items don't read their tag ID...
		tag = listtag;
		
		// ... and are prohibited from being named.
		namebuf = blank;
	}
	else
	{
		tag = getchar();
		if (tag)
			(void) getchar(); // null byte that's always discarded
		
		if (tag != 0) // compound end tags have no name
		{
			namebuf = calloc(256, 1);
			if (namebuf == NULL)
			{
				fprintf(stderr, "\n--- failed to allocate memory ---\n");
				exit(8);
			}
			readname(namebuf);
		}
	}
	
	if (tag) // only do indenting if it's not a compound end tag
	{
		j = depth;
		while (j--) putchar(' ');
	}
	
	switch(tag)
	{
		case 0: // compound end tag
		{
			// just exit
			break;
		}
		case 1:
		{
			if (strlen(namebuf))
				printf("<value name=\"%s\" type=\"byte\">", namebuf);
			else
				printf("<value type=\"byte\">");
			
			printf("%u</value>\n", readbyte());
			break;
		}
		case 2:
		{
			if (strlen(namebuf))
				printf("<value name=\"%s\" type=\"short\">", namebuf);
			else
				printf("<value type=\"short\">");
			
			printf("%u</value>\n", readshort());
			break;
		}
		case 3:
		{
			if (strlen(namebuf))
				printf("<value name=\"%s\" type=\"integer\">", namebuf);
			else
				printf("<value type=\"integer\">");
			
			printf("%u</value>\n", readint());
			break;
		}
		case 4:
		{
			if (strlen(namebuf))
				printf("<value name=\"%s\" type=\"long\">", namebuf);
			else
				printf("<value type=\"long\">");
			
			printf("%lu</value>\n", readlong());
			break;
		}
		case 5:
		{
			if (strlen(namebuf))
				printf("<value name=\"%s\" type=\"float\"><![CDATA[", namebuf);
			else
				printf("<value type=\"float\"><![CDATA[");
			
			len = 4;
			while(len--) putchar(getchar());
			
			printf("]]></value>\n");
			break;
		}
		case 6:
		{
			if (strlen(namebuf))
				printf("<value name=\"%s\" type=\"double\"><![CDATA[", namebuf);
			else
				printf("<value type=\"double\"><![CDATA[");
			
			len = 8;
			while(len--) putchar(getchar());
			
			printf("]]></value>\n");
			break;
		}
		case 7:
		{
			if (strlen(namebuf))
				printf("<value name=\"%s\" type=\"bytearray\"><![CDATA[", namebuf);
			else
				printf("<value type=\"float\" type=\"bytearray\"><![CDATA[");
			
			len = readint();
			while(len--) putchar(getchar());
			
			printf("]]></value>\n");
			break;
		}
		case 8:
		{
			if (strlen(namebuf))
				printf("<value name=\"%s\" type=\"string\"><![CDATA[", namebuf);
			else
				printf("<value type=\"string\"><![CDATA[");
			
			len = readshort();
			while(len--) putchar(getchar());
			
			printf("]]></value>\n");
			break;
		}
		case 9:
		{
			if (strlen(namebuf))
				printf("<list name=\"%s\">\n", namebuf);
			else
				printf("<list>\n");
			
			j = readbyte();
			len = readint();
			while(len--) readtag(depth+1,j);
			
			j = depth;
			while (j--) putchar(' ');
			printf("</list>\n");
			break;
		}
		case 10:
		{
			if (strlen(namebuf))
				printf("<compound name=\"%s\">\n", namebuf);
			else
				printf("<compound>\n");
			
			while(readtag(depth+1, 0) != 0) { }
			
			j = depth;
			while (j--) putchar(' ');
			printf("</compound>\n");
			break;
		}
	}
	
	if (namebuf != blank && namebuf != NULL) free(namebuf);
	return(tag);
}

int main(void)
{
	readtag(0, 0);
	
	return(0);
}
