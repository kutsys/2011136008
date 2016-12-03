#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void print_elf_h(char* argv)
{
    char temp[100];
    sprintf(temp,"readelf -h %s",argv);
    system(temp);

}

void print_program_h(char* argv)
{
    char temp[100];
    sprintf(temp,"readelf -l %s",argv);
    system(temp);
}

void print_section_h(char* argv)
{
    char temp[100];
    sprintf(temp,"readelf -S %s",argv);
    system(temp);
}

void checkfile(char* argv)
{
    if(access(argv,F_OK)==-1)
    {
	printf("그런 파일은 없습니다\n");
	exit(1);
    }
}

void onlyfile(char* argv)
{
    print_elf_h(argv);
    print_program_h(argv);
    print_section_h(argv);
}

void parsecommand(char* argv1,char* argv2)
{
    if(strstr(argv1,"e") != NULL)
    {
	print_elf_h(argv2);
    }

    if(strstr(argv1,"p") != NULL)
    {
	print_program_h(argv2);
    }

    if(strstr(argv1,"s") != NULL)
    {
	print_section_h(argv2);
    }

}


int main(int argc,char* argv[])
{

    switch(argc)
    {
	case 2:
		checkfile(argv[1]);
		onlyfile(argv[1]);
		break;
	case 3:
		checkfile(argv[2]);
		parsecommand(argv[1],argv[2]);
		break;
	default:
		printf("명령어 입력이 잘못 되었습니다\n");
		break;
    }
	return 0;
}
