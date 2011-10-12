#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("Please enter name of PNG file\n");
		return 1;
	}

	FILE *file = fopen(argv[1], "rb");

	if(file == NULL)
	{
		printf("PNG file not found\n");
		return 1;
	}

	

	fseek(file, 8, SEEK_SET);
	
	char buf[] = "0000";
	unsigned int length = 0;

	int count = 0;
	
	while(!feof(file))
	{
		buf[0] = '0';

		fread(&length, 1, 4, file);

		fread(buf, 1, 4, file);

		//printf("critical chunk = %s\n", buf);
		
		if(buf[0] >= 'A' && buf[0] <= 'Z')
		{
			count++;
			printf("%dth critical chunk = %s\n", count, buf);
		}

		fseek(file, length+4, SEEK_CUR);		
	}

	printf("count of critical chunk = %d\n", count);

	return 0;
}