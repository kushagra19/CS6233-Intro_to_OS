#include<stdio.h>
#include<time.h>
#include<string.h>

void read_file_name(char *file_name)
{		
	printf("Enter filename: ");
	scanf("%s", file_name);

	printf("Entered filename: %s\n", file_name);
}

int routine(char *input_file, char *output_file)
{
	FILE *input_file_ptr, *output_file_ptr;
	char buffer[512];	//buffer to store data from input file
	
	//open file
	input_file_ptr = fopen(input_file, "r");

	//check of file opened withour error
	
	if(input_file_ptr == NULL)
	{
		perror("ERROR");
		fclose(input_file_ptr);
		return 0;
	}

	//open file
	
	output_file_ptr = fopen(output_file, "wx");
	
	//check if file already exists
	if(output_file_ptr == NULL)
	{
		perror("ERROR");
		fclose(output_file_ptr);
		return 0;
	}
	
	//Reading contents of file line by line

	while(fgets(buffer, sizeof(buffer), input_file_ptr))
	{
		fputs(buffer, output_file_ptr);
	}
	
	printf("\nContents of %s transferred successfully to %s.\n", input_file, output_file);

	fclose(output_file_ptr);
	fclose(input_file_ptr);
}


int main()
{
	char input_file_name[100];	//maximum characters in filename
	//read input file
	read_file_name(input_file_name);
        
	char output_file_name[100];	//maximum characters in filename
	//read output file
	read_file_name(output_file_name);
	
	//call routine
	int test = routine(input_file_name, output_file_name);

	return 0;
}
